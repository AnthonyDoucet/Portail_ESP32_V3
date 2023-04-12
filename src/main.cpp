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
    //initEEPROM(); //todo
    //readEEPROM(); //todo
    //initRTC();    //todo
    initServer();
    //lcdPrint(1,0,F("Attente d'une IP"));
    DEBUGLN();DEBUGLN("---------- SETUP END "+ String(millis()) +"ms ----------");DEBUGLN();
    delay(100); //for DHCP
}

//######## LOOP ########
unsigned previousMillis[10] = {0};
void loop(){
  debug_loop();

  if(millis() >= previousMillis[0] + 1000){
    previousMillis[0] = millis();
    bool eth_link = ETH.linkUp();
    lcdPrint(0,0,String(ulong(millis()/1000)));
  }

  writeOutputs();
  readInputs();
}