#ifndef _FN_DECLARATION_H_
#define _FN_DECLARATION_

//debug.cpp
void debug_loop();

//server.cpp
void initServer();
void initNTP();
void WiFiEvent(WiFiEvent_t event);
void WebSocketEvent();

//utils.cpp
void initIO();
void initEEPROM();
void initLCD();
void initRTC();
void readInputs();

#endif