#include <main.h>
#include <html_files.h>

//######## NETWORK ########
void initConnection(){
    DEBUGLN("starting Ethernet");
    WiFi.onEvent(networkEvent);
    ETH.begin( SPI_MISO_GPIO, SPI_MOSI_GPIO, SPI_SCLK_GPIO, SPI_CS0_GPIO, SPI_INT0_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST);
    //ETH.config(myIP, myGW, mySN, myDNS);
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
      AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html, processor);
      response->addHeader("Server","ESP Async Web Server");
      request->send(response);
    });

    server.on("/force", HTTP_GET, [](AsyncWebServerRequest *request){
      state_pin_force = !state_pin_force;
      request->send(200, "text/plain", "force: " + String(state_pin_force));
    });

    server.on("/ouvre", HTTP_GET, [](AsyncWebServerRequest *request){
      state_pin_ouvre = !state_pin_ouvre;
      request->send(200, "text/plain", "ouvre: " + String(state_pin_ouvre));
    });

    server.on("/reset_reboot_counter", HTTP_GET, [](AsyncWebServerRequest *request){
      uint16_t saved_reboot_counter = reboot_counter;
      eeprom.begin("data", false); //Read/Write
      eeprom.putUShort("reboot", 0);
      reboot_counter = 0;
      eeprom.end();
      request->send(200, "text/plain", "reboot_counter was " + String(saved_reboot_counter) + " , now set to ");
    });

    server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request){
      String str = "";
      str += "-- URL --:\n";
      str += "/force\n";
      str += "/ouvre\n";
      str += "/reset_reboot_counter\n";
      str += "/debug\n";
      str += "/reboot\n\n";

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

    server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", "ESP rebooting...");
      shouldReboot = true;
    });

    // Simple Firmware Update Form
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
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
      ethernet_status = "Link on";
      break;

    case ARDUINO_EVENT_ETH_DISCONNECTED:
      DEBUGLN(F("ETH Disconnected"));
      ethernet_status = "Link off";
      break;

    case ARDUINO_EVENT_ETH_GOT_IP:
      ethernet_status = "Ok";

      DEBUG(F("ETH MAC: "));
      DEBUG(ETH.macAddress());
      DEBUG(F(", IPv4: "));
      DEBUG(ETH.localIP());
      DEBUG(F(", Mask: "));
      DEBUG(ETH.subnetMask());
      DEBUG(F(", Gateway: "));
      DEBUGLN(ETH.gatewayIP());

      initServer(); //Start the webserver
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

void WebSocketEvent(){
    //todo
}

String processor(const String &var){

  if(var == "DATE"){
    return getRTCDateStr();
  }
  else if(var == "HEURE"){
    return getRTCTimeStr();
  }
  else if(var == "CYCLE"){
    return (state_pin_ouvre) ? "Oui" : "Non";
  }
  else if(var == "FORCE"){
    return (state_pin_force) ? "Oui" : "Non";
  }
  else if(var == "PRESENCE_SECTEUR"){
    return (state_pin_secteur) ? "Oui" : "Non";
  }
  else if(var == "VBAT"){
    return String(nBat);
  }
  else if(var == "DATE_COUPURE"){
    return String("Aucune");
  }
  else if(var == "DATE_PRESENCE"){
    return String("Aucune");
  }
  else if(var == "DATE_CYCLE_JOUR"){
    return String("Aucune");
  }
  else if(var == "DATE_CYCLE_NUIT"){
    return String("Aucune");
  }

  return String();
}