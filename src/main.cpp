#define DEFINE_GLOBAL_VARS //only in one source file
#include <main.h>

//######## SETUP ########
void setup(){
    Serial.begin(UART_BAUDRATE);
    DEBUGLN();DEBUGLN("---------- SETUP ----------");DEBUGLN();

    initIO();

    byte blink_delay = 100;
    digitalWrite(PIN_BUILDIN_LED, LOW);
    digitalWrite(PIN_BUILDIN_LED, HIGH);
    delay(blink_delay);
    digitalWrite(PIN_BUILDIN_LED, LOW);

    initServer();

    DEBUGLN();DEBUGLN("---------- SETUP END "+ String(millis() - blink_delay) +"ms ----------");DEBUGLN();
}

//######## LOOP ########
void loop(){
  debug_loop();
  delay(1);
}