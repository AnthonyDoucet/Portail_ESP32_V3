#include <main.h>

#if CONFIG_RESTART_DEBUG_INFO && (CONFIG_RESTART_DEBUG_STACK_DEPTH > 0)
#include "esp_types.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_debug_helpers.h"
#include "soc/soc_memory_layout.h"
#include "soc/cpu.h" 
#endif // CONFIG_RESTART_STACK_DEPTH

uint32_t previousTimeLoop, loopCounter;
#define debug_loop_delay 1
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
    //lcd.clear();
  }
}