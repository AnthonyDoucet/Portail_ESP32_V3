#ifndef _FN_DECLARATION_H_
#define _FN_DECLARATION_

//######## main.cpp ########
void delayed_task();

//######## portail_process.cpp ########
void portail_process();
void cycle_process();

//######## debug.cpp ########
void debug_loop();

//######## server.cpp ########
void initServer();
void WiFiEvent(WiFiEvent_t event);
void WebSocketEvent();

//######## utils.cpp ########
//IO
void initIO();
void readInputs();
void writeOutputs();
void blink(int pin, int time);

//LCD
#if LCD_ENABLED
void initLCD();
void lcdPrint(uint8_t line, uint8_t pos, String str);
void lcdPrint(uint8_t line, String str);
void lcdClear(uint8_t line);
void lcdMenu();
#endif

//RTC
void initRTC();
String getRTCTimeStr();

//EEPROM
void initEEPROM();
void readEEPROM();
void writeEEPROM();

#endif