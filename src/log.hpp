#ifndef TL_LOG_HPP
#define TL_LOG_HPP

#include <Arduino.h>
#include <SD.h>

#define LOG_BYTE_LIMIT 1024UL * 1024UL * 50UL       // 50 mebibyte file limit
#define FNAME_BOOT "BOOT.ID"                        // File in which the Host/Boot ID is stored

bool tl_log_init(int chip = BUILTIN_SDCARD);        // initialize TeensyLogger's LOG module
bool tl_log_data(const byte *data, size_t size);    // write data to the log file
bool tl_log_byte(const byte datum);                 // write single byte to the log file

#endif//TL_LOG_HPP