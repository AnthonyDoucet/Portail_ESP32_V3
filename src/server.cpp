#include <main.h>
#include <html_files.h>

//######## NETWORK ########
void initServer(){
    DEBUGLN("starting Ethernet");
    WiFi.onEvent(WiFiEvent);
    ETH.begin( SPI_MISO_GPIO, SPI_MOSI_GPIO, SPI_SCLK_GPIO, SPI_CS0_GPIO, SPI_INT0_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST);
    //ETH.config(myIP, myGW, mySN, myDNS);

    DEBUGLN("init AsyncWebServer");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      DEBUGLN("New client on /");
      request->send(200, "text/plain", "Projet Portail v3");
    });

    server.on("/millis", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", "Millis since start: "+String(millis()/1000));
    });

    server.on("/freeheap", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", "ESP FreeHeap: "+String(ESP.getFreeHeap()));
    });

    server.on("/force", HTTP_GET, [](AsyncWebServerRequest *request){
      state_pin_force = !state_pin_force;
      request->send(200, "text/plain", "force: " + String(state_pin_force));
    });

    server.on("/ouvre", HTTP_GET, [](AsyncWebServerRequest *request){
      state_pin_ouvre = !state_pin_ouvre;
      request->send(200, "text/plain", "ouvre: " + String(state_pin_ouvre));
    });

    server.on("/io", HTTP_GET, [](AsyncWebServerRequest *request){
      String str = "-- INPUT --\n";
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
      str += "state_pin_ouvre:" + String(state_pin_ouvre) + "\n";


      request->send(200, "text/plain", str);
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
      AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", favicon_ico_gz, favicon_ico_gz_len);
      response->addHeader("Content-Encoding", "gzip");
      request->send(response);
    });

    DEBUGLN("starting AsyncWebServer");
    server.begin();
}

void initNTP(){
    //todo
}

void WiFiEvent(WiFiEvent_t event){  //handle Ethernet connection event
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      DEBUGLN(F("ETH Started"));
      ethernet_status = "Started";
      break;

    case ARDUINO_EVENT_ETH_STOP:
      DEBUGLN(F("ETH Stopped"));
      ethernet_status = "Stopped";
      break;

    case ARDUINO_EVENT_ETH_CONNECTED:
      DEBUGLN(F("ETH Connected"));
      break;

    case ARDUINO_EVENT_ETH_DISCONNECTED:
      DEBUGLN(F("ETH Disconnected"));
      break;

    case ARDUINO_EVENT_ETH_GOT_IP:
      DEBUG(F("ETH MAC: "));
      DEBUG(ETH.macAddress());
      DEBUG(F(", IPv4: "));
      DEBUG(ETH.localIP());
      #if LCD_ENABLED
        lcdClear(1);
        lcdPrint(1,0,"IP:"+ETH.localIP().toString());
      #endif
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

void WebSocketEvent(){
    //todo
}