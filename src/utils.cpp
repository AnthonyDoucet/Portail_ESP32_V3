#include <main.h>

//######## IO ########
void initIO(){
    DEBUGLN("initIO");
    //Input
    pinMode(PIN_Bouton1, INPUT);
    pinMode(PIN_Bouton2, INPUT);
    pinMode(PIN_Bouton3, INPUT);
    pinMode(PIN_Bouton4, INPUT);
    pinMode(PIN_PresenceSecteur, INPUT);
    pinMode(PIN_Cycle, INPUT);

    //Output
    pinMode(PIN_LED_BUILDIN, OUTPUT);
    pinMode(PIN_Force, OUTPUT);
    pinMode(PIN_Ouvre, OUTPUT);
    pinMode(PIN_DEBUG, OUTPUT);
}

void readInputs(){
  nBat = analogRead(PIN_vBat);
  vBat = (double)nBat * 0.0068376068;

  state_pin_btn1 = !digitalRead(PIN_Bouton1);
  state_pin_btn2 = !digitalRead(PIN_Bouton2);
  state_pin_btn3 = !digitalRead(PIN_Bouton3);
  state_pin_btn4 = !digitalRead(PIN_Bouton4);

  state_pin_secteur = !digitalRead(PIN_PresenceSecteur);
  state_pin_cycle = !digitalRead(PIN_Cycle);
}

void writeOutputs(){
  digitalWrite(PIN_Force, state_pin_force);
  digitalWrite(PIN_Ouvre, state_pin_ouvre);
}

void blink(int pin, int time){
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);
  delay(time);
  digitalWrite(pin, LOW);
}

//######## LCD ########
#if LCD_ENABLED
void initLCD(){
  
  lcd.init();
  lcd.backlight();
  
  lcdClear(2);
}

void lcdPrint(uint8_t line, uint8_t pos, String str){
    if(pos < LCD_WIDTH && line < LCD_HEIGHT){
        lcd.setCursor(pos, line);
        lcd.print(str);
    }
}

void lcdPrint(uint8_t line, String str){
    if(line < LCD_HEIGHT){
        lcd.setCursor(0, line);
        lcd.print(str);
    }
}

void lcdClear(uint8_t line){
    if(line == 2){
        lcdPrint(0,"                ");
        lcdPrint(1,"                ");
    }
    else{
        lcdPrint(line,"                ");
    }
}
#endif

//######## RTC ########
void initRTC(){
  //todo
}

//######## EEPROM ########
void initEEPROM(){
  //todo
}

void readEEPROM(){
  //todo
}