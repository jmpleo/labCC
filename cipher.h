#ifndef CIPHER_H
#define CIPHER_H

#include "lfsr.h"

class Cipher
{
public:
    Cipher(uint64_t poly1, uint64_t poly2, uint64_t l1, uint64_t l2);
    ~Cipher();

    //void Enc(char* p, const uint64_t &k1, const uint64_t &k2);
    //void Dec(char* p, const uint64_t &k1, const uint64_t &k2);

    inline void EncByte(uint8_t &byte, const uint64_t &k1, const uint64_t &k2);
    inline void DecByte(uint8_t &byte, const uint64_t &k1, const uint64_t &k2);

private:
    static inline void g(uint8_t &byte, const uint64_t &gamma);
    static inline void G(uint8_t &byte, const uint64_t &gammas);
    static inline void inv_g(uint8_t &byte, const uint64_t &gamma);
    static inline void inv_G(uint8_t &byte, const uint64_t &gamma);

private:
    LFSR _lfsr1;
    LFSR _lfsr2;
};

inline void Cipher::EncByte(uint8_t &byte, const uint64_t &k1, const uint64_t &k2)
{
    _lfsr1.reset(k1);
    _lfsr2.reset(k2);

    G(byte,
           (_lfsr1.next() ^ _lfsr2.next())
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<1u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<2u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<3u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<4u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<5u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<6u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<7u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<8u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<9u)
    );
}

inline void Cipher::DecByte(uint8_t &byte, const uint64_t &k1, const uint64_t &k2)
{
    _lfsr1.reset(k1);
    _lfsr2.reset(k2);

    inv_G(byte,
           (_lfsr1.next() ^ _lfsr2.next())
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<1u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<2u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<3u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<4u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<5u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<6u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<7u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<8u)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<9u)
    );
}

inline void Cipher::g(uint8_t &byte, const uint64_t &gamma)
{
#define phi(x) (((x) ^ ( ((x) >>2) & ((x) >>3) & ((x) >>7) )) &1u )
    byte = ((byte >>1) &0x7Fu)
         ^ ((phi(byte) ^ (gamma &1u)) <<7);
}

inline void Cipher::inv_g(uint8_t &byte, const uint64_t &gamma)
{
    byte = ((byte <<1) &0xFEu)
         ^ phi(((byte <<1) &0xFF) ^ ((byte >>7) &1u))
         ^ (gamma &1u);
}

inline void Cipher::G(uint8_t &byte, const uint64_t &gammas)
{
    g(byte, gammas >>0); g(byte, gammas >>1);
    g(byte, gammas >>2); g(byte, gammas >>3);
    g(byte, gammas >>4); g(byte, gammas >>5);
    g(byte, gammas >>6); g(byte, gammas >>7);
    g(byte, gammas >>8); g(byte, gammas >>9);
}

inline void Cipher::inv_G(uint8_t &byte, const uint64_t &gammas)
{
    inv_g(byte, gammas >>9); inv_g(byte, gammas >>8);
    inv_g(byte, gammas >>7); inv_g(byte, gammas >>6);
    inv_g(byte, gammas >>5); inv_g(byte, gammas >>4);
    inv_g(byte, gammas >>3); inv_g(byte, gammas >>2);
    inv_g(byte, gammas >>1); inv_g(byte, gammas >>0);
}

#endif
