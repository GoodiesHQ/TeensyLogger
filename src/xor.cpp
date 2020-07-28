#include "xor.hpp"

#define XOR_KEY "\x11\x22\x33\x44\x55"
#define XOR_KEY_LEN (sizeof(XOR_KEY) - 1)

static const byte xor_key[] = XOR_KEY;
static size_t xor_i = XOR_KEY_LEN;

/*
 * Reset XOR key rotation
 */
void tl_xor_key_rst(void){
    xor_i = 0;
}


/*
 * Get next byte in the XOR key, handle rotation
 */
byte tl_xor_key_nxt(void){
    if(xor_i == XOR_KEY_LEN){
        tl_xor_key_rst();
    }
    return xor_key[xor_i++];
}

/*
 * XOR encrypt data passed
 */
void tl_xor_data(byte *data, size_t length) {
    for(unsigned i = 0; i < length; ++i) {
        data[i] ^= tl_xor_key_nxt();
    }
}
