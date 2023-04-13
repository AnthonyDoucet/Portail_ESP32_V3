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
    delay(100); //for DHCP
}

//######## LOOP ########
void loop(){
  delayed_task();
  portail_process();
  writeOutputs();
  readInputs();
  debug_loop();

  if(shouldReboot)
    ESP.restart();
}

//######## TASK ########
unsigned previousMillis[10] = {0};
void delayed_task(){

  if(millis() >= previousMillis[0] + 1000){     //1 sec
    previousMillis[0] = millis();
    lcdPrint(0,0,String(millis()/1000));
    lcdMenu();
    rtc_now = DS1307_RTC.now();
  }

  if(millis() >= previousMillis[1] + 60000){   //1min
    previousMillis[1] = millis();
    //printLocalTime();
  }

  if(millis() >= previousMillis[2] + 3600000){   //1h
    previousMillis[2] = millis();
    writeEEPROM();
  }
}