#ifndef CIPHER_H
#define CIPHER_H

#include "lfsr.h"

#include <cstdint>
#include <string>
#include <vector>


namespace cipher {

using Byte = uint8_t;
using Bytes = std::vector<uint8_t>;

class Cipher
{
public:
    Cipher(uint64_t l1, uint64_t l2);
    Cipher(uint64_t poly1, uint64_t poly2, uint64_t l1, uint64_t l2);
    ~Cipher();

    Bytes Enc(Bytes, const uint64_t &k1, const uint64_t &k2);
    Bytes Dec(Bytes, const uint64_t &k1, const uint64_t &k2);

    static std::vector<Bytes> computeG();
    static Bytes fromHex(std::string hexStr);
    static std::string toHex(Bytes bytes);

private:
    Byte EncByte(Byte byte);
    Byte DecByte(Byte byte);

    static void g(Byte &byte, const uint64_t &gamma);
    static void G(Byte &byte, const uint64_t &gammas);
    static void inv_g(Byte &byte, const uint64_t &gamma);
    static void inv_G(Byte &byte, const uint64_t &gammas);

private:
    LFSR _lfsr1;
    LFSR _lfsr2;
};

}

#endif
