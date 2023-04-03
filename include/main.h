#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <portail_config.h>
#include <macro.h>
#include <ESPAsyncWebServer.h>
#include <function_declaration.h>

//debug

//server
GLOBAL byte eth_mac[6] _INIT(MAC_ADDRESS);
GLOBAL AsyncWebServer server _INIT((HTTP_PORT));

//utils


#endif