#include <main.h>

void initIO(){

    DEBUGLN("Set INPUT Pin");
    pinMode(PIN_Bouton1, INPUT);
    pinMode(PIN_Bouton2, INPUT);
    pinMode(PIN_Bouton3, INPUT);
    pinMode(PIN_Bouton4, INPUT);
    pinMode(PIN_PresenceSecteur, INPUT);
    pinMode(PIN_Cycle, INPUT);

    DEBUGLN("Set OUTPUT Pin");
    pinMode(PIN_BUILDIN_LED, OUTPUT);
    pinMode(PIN_Force, OUTPUT);
    pinMode(PIN_Ouvre, OUTPUT);
    pinMode(PIN_DEBUG, OUTPUT);

}

void initEEPROM(){
  //todo
  //EEPROM.begin(1024);
  //readFromEEPROM();
}

void initLCD(){
  //todo
  //lcd.init();
  //lcd.backlight();
  //lcdClear(2);
}

void initRTC(){
  //todo
}

void readInputs(){
  //nBat = analogRead(PIN_vBat);
  //vBat = (double)nBat * 0.0068376068;

  //state_btn1 = !digitalRead(PIN_Bouton1);
  //state_btn2 = !digitalRead(PIN_Bouton2);
  //state_btn3 = !digitalRead(PIN_Bouton3);
  //state_btn4 = !digitalRead(PIN_Bouton4);

  //state_secteur = !digitalRead(PIN_PresenceSecteur);
  //state_cycle = !digitalRead(PIN_Cycle);
}