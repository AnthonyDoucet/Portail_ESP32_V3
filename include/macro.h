#ifndef _MACRO_H_
#define _MACRO_H_

#if SHOW_DEBUG == true
    #define DEBUG(x) Serial.print(x)
    #define DEBUGLN(x) Serial.println(x)
#else
    #define DEBUG(x)
    #define DEBUGLN(x)
#endif

#define PRINT(x) Serial.print(x)
#define PRINTLN(x) Serial.println(x)
#define PRINTF(x) Serial.printf(x)

#ifndef DEFINE_GLOBAL_VARS
# define GLOBAL extern
# define _INIT(x)
# define _INIT_N(x)
#else
# define GLOBAL
# define _INIT(x) = x

//needed to ignore commas in array definitions
#define UNPACK( ... ) __VA_ARGS__
# define _INIT_N(x) UNPACK x
#endif

#endif