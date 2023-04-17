#include <main.h>
#include <html_files.h>

//######## NETWORK ########
void initConnection(){
    #if ENABLE_SOFT_AP
    DEBUGLN("starting SOFTAP");
    WiFi.onEvent(networkEvent);
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(AP_SSID,AP_PASSWORD,1,AP_HIDDEN);
    #endif

    DEBUGLN("starting Ethernet");
    ETH.begin( SPI_MISO_GPIO, SPI_MOSI_GPIO, SPI_SCLK_GPIO, SPI_CS0_GPIO, SPI_INT0_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST);
    //ETH.config(myIP, myGW, mySN, myDNS);

    initServer(); //Start the webserver
}

void initServer(){

    if(serverStarted){
      return;
    }
    serverStarted = true;

    DEBUGLN("init mDNS");
    MDNS.begin("portail");

    DEBUGLN("init AsyncWebServer");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      #if WEB_ENABLE_LOGIN
      if(!request->authenticate(WEB_USERNAME,WEB_PASSWORD))
        return request->requestAuthentication();
      #endif
      decodeParameters(request);
      AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html, processor);
      response->addHeader("Server","ESP Async Web Server");
      request->send(response);
    });

    server.on("/historique", HTTP_GET, [](AsyncWebServerRequest *request){
      #if WEB_ENABLE_LOGIN
      if(!request->authenticate(WEB_USERNAME,WEB_PASSWORD))
        return request->requestAuthentication();
      #endif
      String str = "";
      int i;

      str += "-- date_ouvertures --:\n";
      for(i=0 ; i < 20 ; i++){
        str += "["+String(i)+"] " +  date_cycle[i] + "\n";
      }
      str += "\n\n";
      
      str += "-- date_derniere_coupure --:\n";
      for(i=0 ; i < 10 ; i++){
        str += "["+String(i)+"] " +  date_derniere_coupure[i] + "\n";
      }
      str += "\n\n";

      str += "-- date_derniere_presence --:\n";
      for(i=0 ; i < 10 ; i++){
        str += "["+String(i)+"] " +  date_derniere_presence[i] + "\n";
      }
      str += "\n\n";

      str += "-- date_dernier_force --:\n";
      for(i=0 ; i < 10 ; i++){
        str += "["+String(i)+"] " +  date_dernier_force[i] + "\n";
      }
      str += "\n\n";

      request->send(200, "text/plain", str);
    });

    server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request){
      #if WEB_ENABLE_LOGIN
      if(!request->authenticate(WEB_ADMIN_USERNAME,WEB_ADMIN_PASSWORD))
        return request->requestAuthentication();
      #endif

      String str = "";
      str += "-- URL --:\n";
      str += "portail.local/historique";
      str += "portail.local/update\n";
      str += "portail.local/reset_reboot_counter\n";
      str += "portail.local/reboot\n\n";

      str += "-- SET_VARIABLES --:\n";
      str += "portail.local?heure_hiver=true\n";
      str += "portail.local?sync_rtc=1681463715\n";
      str += "portail.local?avantForce_rst=0\n";
      str += "portail.local?cmp_principal_rst=0\n";
      str += "portail.local?cmp_principal_rst&jour=0&nuit=0\n";
      str += "portail.local?cmp_aux_rst&jour=0&nuit=0\n";
      str += "portail.local?cmp_coupures_rst=0\n\n";

      str += "-- INPUT --\n";
      str += "nBat:" + String(nBat) + "\n";
      str += "vBat:" + String(vBat) + "\n";
      str += "state_pin_btn1:" + String(state_pin_btn1) + "\n";
      str += "state_pin_btn2:" + String(state_pin_btn2) + "\n";
      str += "state_pin_btn3:" + String(state_pin_btn3) + "\n";
      str += "state_pin_btn4:" + String(state_pin_btn4) + "\n";
      str += "state_pin_secteur:" + String(state_pin_secteur) + "\n";
      str += "state_pin_cycle:" + String(state_pin_cycle) + "\n\n";

      str += "-- OUTPUT --\n";
      str += "state_pin_force:" + String(state_pin_force) + "\n";
      str += "state_pin_ouvre:" + String(state_pin_ouvre) + "\n\n";

      str += "-- DEBUG --\n";
      str += "RTC Time:" + getRTCDateStr() + " " + getRTCTimeStr() + "\n";
      str += "Uptime:" + String(millis()/1000) + "s\n";
      str += "Saved uptime:" + String(saved_uptime/3600) + "h\n";
      str += "ESP FreeHeap:" + String(ESP.getFreeHeap()) + "\n";
      str += "ESP Sketch Size:" + String(ESP.getSketchSize()) + "\n";
      str += "Reboot counter:" + String(reboot_counter) + "\n";

      request->send(200, "text/plain", str);
    });

    server.on("/reset_reboot_counter", HTTP_GET, [](AsyncWebServerRequest *request){
      #if WEB_ENABLE_LOGIN
      if(!request->authenticate(WEB_ADMIN_USERNAME,WEB_ADMIN_PASSWORD))
        return request->requestAuthentication();
      #endif
      uint16_t saved_reboot_counter = reboot_counter;
      eeprom.begin("data", false); //Read/Write
      eeprom.putUShort("reboot", 0);
      reboot_counter = 0;
      eeprom.end();
      request->send(200, "text/plain", "reboot_counter was " + String(saved_reboot_counter) + " , now set to " + String(reboot_counter) );
    });

    server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
      #if WEB_ENABLE_LOGIN
      if(!request->authenticate(WEB_ADMIN_USERNAME,WEB_ADMIN_PASSWORD))
        return request->requestAuthentication();
      #endif
      request->send(200, "text/plain", "ESP rebooting...");
      shouldReboot = true;
    });

    // Simple Firmware Update Form
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
      #if WEB_ENABLE_LOGIN
      if(!request->authenticate(WEB_ADMIN_USERNAME,WEB_ADMIN_PASSWORD))
        return request->requestAuthentication();
      #endif
      request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
    });
    server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
      shouldReboot = !Update.hasError();
      AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot?"OK":"FAIL");
      response->addHeader("Connection", "close");
      request->send(response);
    },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
      if(!index){
        Serial.printf("Update Start: %s\n", filename.c_str());
        //Update.runAsync(true);
        if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
          Update.printError(Serial);
        }
      }
      if(!Update.hasError()){
        if(Update.write(data, len) != len){
          Update.printError(Serial);
        }
      }
      if(final){
        if(Update.end(true)){
          Serial.printf("Update Success: %uB\n", index+len);
        } else {
          Update.printError(Serial);
        }
      }
    });

    server.onNotFound([](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", notFound_html);
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
      AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", favicon_ico_gz, favicon_ico_gz_len);
      response->addHeader("Content-Encoding", "gzip");
      request->send(response);
    });

    DEBUGLN("starting AsyncWebServer");
    server.begin();
    MDNS.addService("http", "tcp", 80);
}

void networkEvent(WiFiEvent_t event){  //handle Ethernet connection event
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      DEBUGLN(F("ETH Started"));
      break;

    case ARDUINO_EVENT_ETH_STOP:
      DEBUGLN(F("ETH Stopped"));
      break;

    case ARDUINO_EVENT_ETH_CONNECTED:
      DEBUGLN(F("ETH Connected"));
      ethernet_status = "Link on        ";
      break;

    case ARDUINO_EVENT_ETH_DISCONNECTED:
      DEBUGLN(F("ETH Disconnected"));
      ethernet_status = "Link off       ";
      break;

    case ARDUINO_EVENT_ETH_GOT_IP:
      DEBUG(F("ETH MAC: "));
      DEBUG(ETH.macAddress());
      DEBUG(F(", IPv4: "));
      DEBUG(ETH.localIP());
      DEBUG(F(", Mask: "));
      DEBUG(ETH.subnetMask());
      DEBUG(F(", Gateway: "));
      DEBUGLN(ETH.gatewayIP());
      ethernet_status = ETH.localIP().toString();
      break;

    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      DEBUG("WIFI AP MAC: ");
      DEBUG(WiFi.macAddress());
      DEBUG(", IPv4: ");
      DEBUGLN(WiFi.softAPIP());
      break;

    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      DEBUGLN("WIFI AP Disconnected");
      break;

    default:
      DEBUGLN("Default Wifi event: " + (String)event);
      break;
  }
}

void decodeParameters(AsyncWebServerRequest *request){

    // Controle
    if (request->hasParam("heure_hiver")) {
      String input = request->getParam("heure_hiver")->value();
      if(heure_hiver == false && input == "true"){
        setRTCunixtime(rtc_now.unixtime() - 3600);
        heure_hiver = true;
      }
      else if(heure_hiver == true && input == "false"){
        setRTCunixtime(rtc_now.unixtime() + 3600);
        heure_hiver = false;
      }
    }

    if (request->hasParam("sync_rtc")) {
      double val = request->getParam("sync_rtc")->value().toDouble();
      DEBUGLN("sync_rtc: " + String(val));
      val += 3600; //GMT+1
      if(!heure_hiver){
          val += 3600;  //GMT+2
      }
      setRTCunixtime(val);
    }

    if (request->hasParam("force")) {
      state_pin_force = !state_pin_force;
    }

    if (request->hasParam("ouvre")) {
      state_pin_ouvre = true;
    }

    // Plage Horaire
    if (request->hasParam("horaireMatin")) {
      horaireMatin_h = request->getParam("h")->value().toInt();
      horaireMatin_m = request->getParam("m")->value().toInt();
      DEBUGLN("horaireMatin: ");
    }

    if (request->hasParam("horaireNuit")) {
      horaireNuit_h = request->getParam("h")->value().toInt();
      horaireNuit_m = request->getParam("m")->value().toInt();
      DEBUGLN("horaireNuit: ");
    }

    // Ouverture forcée
    if (request->hasParam("avantForce_rst")) {
      cmp_avant_force = request->getParam("avantForce_rst")->value().toInt();
    }

    if (request->hasParam("avantForce_moin")) {
      if(seuil_avant_force > 2){
        seuil_avant_force--;
      }
    }

    if (request->hasParam("avantForce_plus")) {
      if(seuil_avant_force < 20){
        seuil_avant_force++;
      }
    }

    // Compteurs
    if (request->hasParam("cmp_principal_rst")) {
      cmp_ouvertures_jour = request->getParam("jour")->value().toInt();
      cmp_ouvertures_nuit = request->getParam("nuit")->value().toInt();
    }

    if (request->hasParam("cmp_aux_rst")) {
      cmp_aux_ouvertures_jour = request->getParam("jour")->value().toInt();
      cmp_aux_ouvertures_nuit = request->getParam("nuit")->value().toInt();
    }

    if (request->hasParam("cmp_coupures_rst")) {
      cmp_coupures = request->getParam("cmp_coupures_rst")->value().toInt();
    }

}

String processor(const String &var){

    //INFOS
    if(var == "DATE"){
      return getRTCDateStr();
    }
    else if(var == "HEURE"){
      return getRTCTimeStr();
    }
    else if(var == "CYCLE"){
      return (state_pin_cycle) ? "cVert" : "cGris";
    }
    else if(var == "FORCE"){
      if(state_pin_cycle)
        return "cBleu";
      return (state_pin_force) ? "cVert" : "cGris";
    }
    else if(var == "PRESENCE_SECTEUR"){
      return (state_pin_secteur) ? "Oui" : "Non";
    }
    else if(var == "VBAT"){
      return String(nBat) + "n  " + String(vBat) + "v";
    }
    else if(var == "DATE_COUPURE"){
      return date_derniere_coupure[date_derniere_coupure_increment - 1];
    }
    else if(var == "DATE_PRESENCE"){
      return date_derniere_presence[date_derniere_presence_increment - 1];
    }
    else if(var == "DATE_CYCLE_JOUR"){
      return date_dernier_cycle_jour;
    }
    else if(var == "DATE_CYCLE_NUIT"){
      return date_dernier_cycle_nuit;
    }

    //CONTROLES
    else if(var == "HEURE_HIVER"){
      return (heure_hiver) ? "checked" : " "; 
    }

    //PLAGE HORAIRE
    else if(var == "PLAGE_MATIN"){
      return to2digit(horaireMatin_h) + ":" + to2digit(horaireMatin_m); 
    }
    else if(var == "PLAGE_NUIT"){
      return to2digit(horaireNuit_h) + ":" + to2digit(horaireNuit_m); 
    }

    //OUVERTURE FORCEE
    else if(var == "DATE_FORCE"){
      return date_dernier_force[date_dernier_force_increment - 1];
    }
    else if(var == "CMP_AVANT_FORCE"){
      return String(cmp_avant_force);
    }
    else if(var == "SEUIL_AVANT_FORCE"){
      return String(seuil_avant_force);
    }

    //COMPTEURS
    else if(var == "CMP_JOURNALIER_JOUR"){
      return String(cmp_journalier_jour);
    }
    else if(var == "CMP_JOURNALIER_NUIT"){
      return String(cmp_journalier_nuit);
    }
    else if(var == "CMP_OUVERTURES_JOUR"){
      return String(cmp_ouvertures_jour);
    }
    else if(var == "CMP_OUVERTURES_NUIT"){
      return String(cmp_ouvertures_nuit);
    }
    else if(var == "CMP_AUX_JOUR"){
      return String(cmp_aux_ouvertures_jour);
    }
    else if(var == "CMP_AUX_NUIT"){
      return String(cmp_aux_ouvertures_nuit);
    }
    else if(var == "CMP_COUPURES"){
      return String(cmp_coupures);
    }

  return String();
}