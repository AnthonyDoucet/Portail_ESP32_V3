#ifndef _FN_DECLARATION_H_
#define _FN_DECLARATION_

//######## debug.cpp ########
void debug_loop();

//######## server.cpp ########
void initServer();
void initNTP();
void WiFiEvent(WiFiEvent_t event);
void WebSocketEvent();

//######## utils.cpp ########
//IO
void initIO();
void readInputs();
void writeOutputs();
void blink(int pin, int time);

//LCD
void initLCD();
void lcdPrint(uint8_t line, uint8_t pos, String str);
void lcdPrint(uint8_t line, String str);
void lcdClear(uint8_t line);

//RTC
void initRTC();

//EEPROM
void initEEPROM();
void readEEPROM();

#endif