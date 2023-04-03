#include <main.h>
#include <html_files.h>

//######## NETWORK ########
void initServer(){
    DEBUGLN("starting Ethernet");
    WiFi.onEvent(WiFiEvent);
    ETH.begin_ws5500(eth_mac);

    DEBUGLN("init AsyncWebServer");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      DEBUGLN("New client on /");
      request->send(200, "text/plain", "Projet Portail ^^");
    });

    server.on("/millis", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", "Millis since start: "+String(millis()));
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
      break;

    case ARDUINO_EVENT_ETH_STOP:
      DEBUGLN(F("ETH Stopped"));
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
      DEBUG(F(", Mask: "));
      DEBUG(ETH.subnetMask());
      DEBUG(F(", Gateway: "));
      DEBUGLN(ETH.gatewayIP());
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