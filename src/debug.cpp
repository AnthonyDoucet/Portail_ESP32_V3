#include <main.h>

uint32_t previousTimeLoop, loopCounter;
#define debug_loop_delay 5
void debug_loop(){
  loopCounter++;
  if(millis() >= previousTimeLoop + (debug_loop_delay*1000)){
    previousTimeLoop = millis();
    DEBUGLN();DEBUGLN("---------- main ----------");
    DEBUGLN("Loop per seconds: " + String(loopCounter/debug_loop_delay));
    loopCounter = 0;
    DEBUGLN("Ethernet: " + ETH.localIP().toString());
    DEBUGLN("WIFI: "     + WiFi.localIP().toString());
    DEBUGLN("HEAP: "     + String(ESP.getFreeHeap()));
    DEBUGLN("RTC: "      + getRTCDateStr() + " " + getRTCTimeStr());
  }
}