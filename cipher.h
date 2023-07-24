#ifndef CIPHER_H
#define CIPHER_H

#include "lfsr.h"
#include <cstddef>
#include <cstdint>
#include <vector>

class Cipher
{
public:
    Cipher(uint64_t poly1, uint64_t poly2, uint64_t l1, uint64_t l2);
    ~Cipher();

    std::string Enc(const char* p, size_t len, const uint64_t &k1, const uint64_t &k2);
    std::string Dec(const char* p, size_t len, const uint64_t &k1, const uint64_t &k2);

    static std::vector<std::vector<uint8_t>> computeG();

//private:
    uint8_t EncByte(uint8_t byte);//, const uint64_t &k1, const uint64_t &k2);
    uint8_t DecByte(uint8_t byte);//, const uint64_t &k1, const uint64_t &k2);

    static void g(uint8_t &byte, const uint64_t &gamma);
    static void G(uint8_t &byte, const uint64_t &gammas);
    static void inv_g(uint8_t &byte, const uint64_t &gamma);
    static void inv_G(uint8_t &byte, const uint64_t &gammas);

//private:
    LFSR _lfsr1;
    LFSR _lfsr2;
};


#endif
