
#ifndef TL_XOR_HPP
#define TL_XOR_HPP

#include <Arduino.h>

byte tl_xor_key_nxt(void);                          // get next byte in the key
void tl_xor_key_rst(void);                          // reset the rotating key index
void tl_xor_data(byte *data, size_t length);        // encrypt data provided

#endif//TL_XOR_HPP