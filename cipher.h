#ifndef CIPHER_H
#define CIPHER_H

#include "lfsr.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#define BIN(s,x) (s <<((x>>10) & 1)<< ((x>>9) & 1)<< ((x>>8) & 1)<< ((x>>7) & 1) << ((x>>6) & 1) << ((x>>5) & 1) << ((x>>4) & 1) << ((x>>3) & 1) << ((x>>2) & 1) << ((x>>1) & 1) << ((x) & 1) )
#define HEX(s,x) (s <<(\
    ((x&0xF0) ==0x00) ? '0' :(\
    ((x&0xF0) ==0x10) ? '1' :(\
    ((x&0xF0) ==0x20) ? '2' :(\
    ((x&0xF0) ==0x30) ? '3' :(\
    ((x&0xF0) ==0x40) ? '4' :(\
    ((x&0xF0) ==0x50) ? '5' :(\
    ((x&0xF0) ==0x60) ? '6' :(\
    ((x&0xF0) ==0x70) ? '7' :(\
    ((x&0xF0) ==0x80) ? '8' :(\
    ((x&0xF0) ==0x90) ? '9' :(\
    ((x&0xF0) ==0xA0) ? 'a' :(\
    ((x&0xF0) ==0xB0) ? 'b' :(\
    ((x&0xF0) ==0xC0) ? 'c' :(\
    ((x&0xF0) ==0xD0) ? 'd' :(\
    ((x&0xF0) ==0xE0) ? 'e' :(\
    ((x&0xF0) ==0xF0) ? 'f' : ' '))))))))))))))))<<(\
    ((x&0xF) == 0x0) ? '0' :(\
    ((x&0xF) == 0x1) ? '1' :(\
    ((x&0xF) == 0x2) ? '2' :(\
    ((x&0xF) == 0x3) ? '3' :(\
    ((x&0xF) == 0x4) ? '4' :(\
    ((x&0xF) == 0x5) ? '5' :(\
    ((x&0xF) == 0x6) ? '6' :(\
    ((x&0xF) == 0x7) ? '7' :(\
    ((x&0xF) == 0x8) ? '8' :(\
    ((x&0xF) == 0x9) ? '9' :(\
    ((x&0xF) == 0xA) ? 'a' :(\
    ((x&0xF) == 0xB) ? 'b' :(\
    ((x&0xF) == 0xC) ? 'c' :(\
    ((x&0xF) == 0xD) ? 'd' :(\
    ((x&0xF) == 0xE) ? 'e' :(\
    ((x&0xF) == 0xF) ? 'f' : ' ')))))))))))))))))


namespace cipher {

using Byte = uint8_t;
using Bytes = std::vector<uint8_t>;

class Cipher
{
public:

    Cipher(uint64_t poly1, uint64_t poly2, uint64_t l1, uint64_t l2);
    ~Cipher();

    Bytes Enc(Bytes, const uint64_t &k1, const uint64_t &k2);
    Bytes Dec(Bytes, const uint64_t &k1, const uint64_t &k2);

    static std::vector<Bytes> computeG();

//private:
    Byte EncByte(Byte byte);//, const uint64_t &k1, const uint64_t &k2);
    Byte DecByte(Byte byte);//, const uint64_t &k1, const uint64_t &k2);
    static Bytes fromHex(std::string hexStr);
    static std::string toHex(Bytes bytes);

    static void g(Byte &byte, const uint64_t &gamma);
    static void G(Byte &byte, const uint64_t &gammas);
    static void inv_g(Byte &byte, const uint64_t &gamma);
    static void inv_G(Byte &byte, const uint64_t &gammas);

//private:
    LFSR _lfsr1;
    LFSR _lfsr2;
};

}

#endif
