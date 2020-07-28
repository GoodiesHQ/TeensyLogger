#include <Arduino.h>
#include "log.hpp"
#include "xor.hpp"

void setup()
{
    Serial.begin(9600);
    while(!Serial){}
    tl_log_init(BUILTIN_SDCARD);

    pinMode(LED_BUILTIN,OUTPUT);
}

void loop()
{
    digitalWriteFast(LED_BUILTIN,!digitalReadFast(LED_BUILTIN));
    delay(500);
}
