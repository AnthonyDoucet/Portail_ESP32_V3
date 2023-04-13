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

uint32_t previousDebounce[4] = {0};
bool oneTimeDebounce[4] = {true};
void debounceBtn(){

  //Btn1
  if(state_pin_btn1 == true && oneTimeDebounce[0] == true){
    oneTimeDebounce[0] = false;
    last_time_btn_pressed = millis();
    //todo
  }
  else if(state_pin_btn1 == false && oneTimeDebounce[0] == false){
    oneTimeDebounce[0] = true;
  }

  //Btn2
  if(state_pin_btn2 == true && oneTimeDebounce[1] == true){
    oneTimeDebounce[1] = false;
    last_time_btn_pressed = millis();
    //todo
  }
  else if(state_pin_btn2 == false && oneTimeDebounce[1] == false){
    oneTimeDebounce[1] = true;
  }

  //Btn3
  if(state_pin_btn3 == true && oneTimeDebounce[2] == true){
    oneTimeDebounce[2] = false;
    last_time_btn_pressed = millis();
    //todo
  }
  else if(state_pin_btn3 == false && oneTimeDebounce[2] == false){
    oneTimeDebounce[2] = true;
  }

  //Btn4
  if(state_pin_btn4 == true && oneTimeDebounce[3] == true){
    oneTimeDebounce[3] = false;
    last_time_btn_pressed = millis();
    //todo
  }
  else if(state_pin_btn4 == false && oneTimeDebounce[3] == false){
    oneTimeDebounce[3] = true;
  }

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

  debounceBtn();
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
  PRINTLN("init LCD");
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

void lcdMenu(){
  if(millis() >= last_time_btn_pressed + (LCD_BACKLIGHT_TIMEOUT*1000)){
    lcd.noBacklight();
  }
  else{
    lcd.backlight();
  }
}

#endif

//######## RTC ########
void initRTC(){
  if (!DS1307_RTC.begin()) {
    PRINTLN("Couldn't find RTC");
    return;
  }
  //DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  PRINTLN("RTC Started");
}

String getRTCDateStr(){
  if(!DS1307_RTC.isrunning()){
    return "RTC Not running";
  }
  return String(rtc_now.year()) + "/" + to2digit(rtc_now.month()) + "/" + to2digit(rtc_now.day());
}

String getRTCTimeStr(){
  if(!DS1307_RTC.isrunning()){
    return "RTC Not running";
  }
  return to2digit(rtc_now.hour()) + ":" + to2digit(rtc_now.minute()) + ":" + to2digit(rtc_now.second());
}

String to2digit(uint8_t nombre){
  String resultat;
  if(nombre < 10)
    resultat = "0";
  return resultat += String(nombre,DEC);  
}

//######## EEPROM ########
void initEEPROM(){

  //eeprom.begin("my-app", false); 

  // Remove all preferences under the opened namespace
  //preferences.clear();

  // Or remove the counter key only
  //preferences.remove("counter");

  // Get the counter value, if the key does not exist, return a default value of 0
  // Note: Key name is limited to 15 chars.
  //unsigned int counter = eeprom.getUInt("counter", 0);

  // Increase counter by 1
  //counter++;

  // Print the counter to Serial Monitor
  //Serial.printf("Current counter value: %u\n", counter);

  // Store the counter to the Preferences
  //eeprom.putUInt("counter", counter);

  // Close the Preferences
  //eeprom.end();

  // Wait 10 seconds
  //Serial.println("Restarting in 10 seconds...");
  //delay(10000);

  // Restart ESP
  //ESP.restart();

  PRINTLN("Init read EEPROM");
  eeprom.begin("data", false); //Read/Write
  reboot_counter = eeprom.getUShort("reboot", 0);
  reboot_counter++;
  eeprom.putUShort("reboot", reboot_counter);
  eeprom.end();

  readEEPROM();
}

void readEEPROM(){
  PRINTLN("Read EEPROM");
  eeprom.begin("data", true); //Read only
  saved_uptime = eeprom.getUInt("saved_uptime", 0);
  eeprom.end();
}

void writeEEPROM(){
  DEBUGLN("Write EEPROM");
  eeprom.begin("data", false); //Read/Write
  eeprom.putULong("saved_uptime",millis()/1000);
  eeprom.end();
}