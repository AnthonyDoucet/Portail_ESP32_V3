#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <portail_config.h>
#include <macro.h>
#include <ADS1X15.h>
#include <Preferences.h>            //EEPROM
#include <ESPAsyncWebServer.h>      //Webserver
#include <Update.h>                 //OTA Update
#include <ESPmDNS.h>                //Multicast DNS "portail.local"
#if LCD_ENABLED
#include <LiquidCrystal_I2C.h>      //I2C LCD
#endif
#include <RTClib.h>                 //I2C RTC
#include <function_declaration.h>

/*---------- ESP ----------*/
GLOBAL Preferences eeprom;
GLOBAL bool shouldReboot _INIT(0);
GLOBAL bool serverStarted _INIT(0);
GLOBAL uint32_t saved_uptime _INIT(0);
GLOBAL uint16_t reboot_counter _INIT(0);

/*---------- SERVEUR ----------*/
GLOBAL AsyncWebServer server _INIT((HTTP_PORT));
GLOBAL String ethernet_status _INIT("");

/*---------- LCD ----------*/
#if LCD_ENABLED
GLOBAL LiquidCrystal_I2C lcd _INIT_N(((LCD_ADDR, LCD_WIDTH, LCD_HEIGHT)));
GLOBAL int16_t LCD_menu_pos _INIT(0);
GLOBAL bool LCD_oneTime _INIT(0);
#endif

/*---------- RTC ----------*/
GLOBAL RTC_DS1307 DS1307_RTC;
GLOBAL DateTime rtc_now;
GLOBAL uint8_t previous_day _INIT(0);

/*---------- BATTERY ----------*/
GLOBAL ADS1115 ADS _INIT((0x48));
GLOBAL int16_t nBat _INIT(0);
GLOBAL float vBat _INIT(0);

/*---------- BUTTONS ----------*/
GLOBAL bool state_pin_btn1 _INIT(0);
GLOBAL bool state_pin_btn2 _INIT(0);
GLOBAL bool state_pin_btn3 _INIT(0);
GLOBAL bool state_pin_btn4 _INIT(0);
GLOBAL bool state_btn1 _INIT(0);
GLOBAL bool state_btn2 _INIT(0);
GLOBAL bool state_btn3 _INIT(0);
GLOBAL bool state_btn4 _INIT(0);

GLOBAL bool state_pin_secteur _INIT(0);
GLOBAL bool state_pin_cycle _INIT(0);
GLOBAL bool state_pin_force _INIT(0);
GLOBAL bool state_pin_ouvre _INIT(0);

GLOBAL uint32_t last_time_btn_pressed _INIT(0);

/*---------------------------------------------------------------*/
//Infos
GLOBAL bool cycle_en_cours _INIT(0);
GLOBAL bool secteur _INIT(0);
GLOBAL String date_cycle[20] _INIT({"Aucun"});
GLOBAL String date_derniere_coupure[10] _INIT({"Aucune"});
GLOBAL String date_derniere_presence[10] _INIT({"Aucune"});
GLOBAL String date_dernier_cycle_jour _INIT("Aucun");
GLOBAL String date_dernier_cycle_nuit _INIT("Aucun");

GLOBAL bool cycleOneTime _INIT(0);
GLOBAL bool secteurOneTime _INIT(0);
GLOBAL bool forceOneTime _INIT(0);

GLOBAL uint16_t date_cycle_increment _INIT(1);
GLOBAL uint16_t date_derniere_coupure_increment _INIT(1);
GLOBAL uint16_t date_derniere_presence_increment _INIT(1);

//Controles
GLOBAL bool heure_hiver _INIT(0);
GLOBAL bool ouvre_force _INIT(0);
GLOBAL bool ouvre_normal _INIT(0);

//PLAGE HORAIRE JOUR
GLOBAL uint8_t horaireMatin_h  _INIT({HEURE_JOUR_DEBUT});
GLOBAL uint8_t horaireMatin_m  _INIT({0});
GLOBAL uint8_t horaireNuit_h _INIT({HEURE_JOUR_FIN});
GLOBAL uint8_t horaireNuit_m _INIT({0});

//COMPTEUR AVANT FORCE
GLOBAL String date_dernier_force[10] _INIT({"Aucune"});
GLOBAL uint16_t cmp_avant_force _INIT(0);
GLOBAL uint16_t seuil_avant_force _INIT(DEFAULT_SEUIL_AVANT_FORCE);
GLOBAL uint16_t date_dernier_force_increment _INIT(1);

//COMPTEURS JOURNALIER
GLOBAL uint16_t cmp_journalier_jour _INIT(0);
GLOBAL uint16_t cmp_journalier_nuit _INIT(0);
//COMPTEURS OUVERTURES
GLOBAL uint16_t cmp_ouvertures_jour _INIT(0);
GLOBAL uint16_t cmp_ouvertures_nuit _INIT(0);
//COMPTEURS AUXILIAIRE OUVERTURES
GLOBAL uint16_t cmp_aux_ouvertures_jour _INIT(0);
GLOBAL uint16_t cmp_aux_ouvertures_nuit _INIT(0);

//COMPTEUR COUPURES SECTEUR
GLOBAL uint16_t cmp_coupures _INIT(0);

#endif