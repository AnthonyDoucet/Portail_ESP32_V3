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
void initConnection();
void initServer();
void networkEvent(WiFiEvent_t event);
void decodeParameters(AsyncWebServerRequest *request);
String processor(const String &var);

//######## utils.cpp ########
//IO
void initIO();
void readInputs();
void readADC();
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
void setRTCunixtime(uint32_t epoch);
String getRTCDateStr();
String getRTCTimeStr();
void updateTime();
String to2digit(uint8_t nombre);

//EEPROM
void initEEPROM();
void readEEPROM();
void writeEEPROM();

#endif