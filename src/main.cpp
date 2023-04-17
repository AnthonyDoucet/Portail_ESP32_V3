#define DEFINE_GLOBAL_VARS //only in one source file
#include <main.h>

//######## SETUP ########
void setup(){
    Serial.begin(UART_BAUDRATE);
    DEBUGLN();DEBUGLN(F("---------- SETUP ----------"));DEBUGLN();
    initIO();
    blink(PIN_LED_BUILDIN, 100);
    readInputs();
    #if LCD_ENABLED
    initLCD();
    lcdClear(2);
    lcdPrint(0,3,F("PORTAIL V3"));
    lcdPrint(1,0,F(" initialisation "));
    #endif
    initEEPROM();
    initRTC();
    initConnection();
    //lcdPrint(1,0,F("Attente d'une IP"));
    DEBUGLN();DEBUGLN("---------- SETUP END "+ String(millis()) +"ms ----------");DEBUGLN();
    lcdClear(2);
}

//######## LOOP ########
unsigned previousMillis[10];
void loop(){
  
  if((millis() - previousMillis[0]) > 1000){     //1 sec
    previousMillis[0] = millis();
    readADC();
    updateTime();
    lcdMenu();
  }

  if((millis() - previousMillis[1]) > 216000000){   //6h
    previousMillis[2] = millis();
    writeEEPROM();
  }

  portail_process();
  writeOutputs();
  readInputs();
  debug_loop();

  if(shouldReboot)
    ESP.restart();
}