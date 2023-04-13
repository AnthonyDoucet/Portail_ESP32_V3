#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <portail_config.h>
#include <macro.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#if LCD_ENABLED
    #include <LiquidCrystal_I2C.h>
#endif
#include <RTClib.h>
#include <function_declaration.h>

//server
GLOBAL IPAddress myIP _INIT((192, 168, 1, 123));
GLOBAL IPAddress myGW _INIT((192, 168, 1, 254));
GLOBAL IPAddress mySN _INIT((255, 255, 255, 0));
GLOBAL IPAddress myDNS _INIT((8, 8, 8, 8));
GLOBAL byte eth_mac[] _INIT(MAC_ADDRESS);
GLOBAL Preferences eeprom;
GLOBAL AsyncWebServer server _INIT((HTTP_PORT));
GLOBAL String ethernet_status _INIT("");

//utils
#if LCD_ENABLED
    GLOBAL LiquidCrystal_I2C lcd _INIT_N(((LCD_ADDR, LCD_WIDTH, LCD_HEIGHT)));
#endif
GLOBAL RTC_DS1307 DS1307_RTC;
GLOBAL DateTime rtc_now;

//###### variables ######
GLOBAL uint32_t saved_uptime _INIT(0);
GLOBAL uint16_t nBat _INIT(0);
GLOBAL uint16_t vBat _INIT(0);
GLOBAL bool state_pin_btn1 _INIT(0);
GLOBAL bool state_btn1 _INIT(0);
GLOBAL bool state_pin_btn2 _INIT(0);
GLOBAL bool state_btn2 _INIT(0);
GLOBAL bool state_pin_btn3 _INIT(0);
GLOBAL bool state_btn3 _INIT(0);
GLOBAL bool state_pin_btn4 _INIT(0);
GLOBAL bool state_btn4 _INIT(0);
GLOBAL bool state_pin_secteur _INIT(0);
GLOBAL bool state_pin_cycle _INIT(0);
GLOBAL bool state_pin_force _INIT(0);
GLOBAL bool state_pin_ouvre _INIT(0);
GLOBAL uint32_t last_time_btn_pressed _INIT(0);



#endif