#include "cipher.h"
#include <cstdint>
#include <vector>


Cipher::Cipher(uint64_t poly1, uint64_t poly2, uint64_t l1, uint64_t l2)
    : _lfsr1(l1, poly1, 0)
    , _lfsr2(l2, poly2, 0)
{ }

Cipher::~Cipher()
{ }

#define bin(x) (std::cout <<((x>>10) & 1)<< ((x>>9) & 1)<< ((x>>8) & 1)<< ((x>>7) & 1) << ((x>>6) & 1) << ((x>>5) & 1) << ((x>>4) & 1) << ((x>>3) & 1) << ((x>>2) & 1) << ((x>>1) & 1) << ((x) & 1) << std::endl)

std::string Cipher::Enc(
    const char* bytes, size_t len,
    const uint64_t &k1,
    const uint64_t &k2
)
{
    _lfsr1.setState(k1);
    _lfsr2.setState(k2);

    std::string c;
    std::cout << std::endl;
    for (size_t i = 0; i < len; ++i) {
        auto ch = EncByte(bytes[i]);
        c.push_back( ch );
        std::cout << i << ".    bytes[i]  = "; bin(bytes[i]);
        std::cout << "  enc(bytes[i]) = "; bin(ch);
    }
    return c;
}

std::string Cipher::Dec(
    const char* bytes, size_t len,
    const uint64_t &k1,
    const uint64_t &k2
)
{
    _lfsr1.setState(k1);
    _lfsr2.setState(k2);

    std::string p;
    std::cout << std::endl;
    for (int i = len - 1; i >= 0; --i) {
        auto ch = DecByte( static_cast <unsigned> (bytes[i]));
        p.push_back( ch );
        std::cout << i << ".    bytes[i]  = "; bin(static_cast< unsigned > (bytes[i]));
        std::cout << "  dec(bytes[i]) = "; bin(ch);
    //p.push_back( DecByte( bytes[i] ));
    }
    return p;
}
uint8_t Cipher::EncByte(uint8_t byte)
{
    G(byte,
           (_lfsr1.next() ^ _lfsr2.next())
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<1)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<2)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<3)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<4)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<5)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<6)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<7)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<8)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<9)
    );
    return byte;
}

uint8_t Cipher::DecByte(uint8_t byte)
{
    inv_G(byte,
           (_lfsr1.next() ^ _lfsr2.next())
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<1)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<2)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<3)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<4)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<5)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<6)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<7)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<8)
        ^ ((_lfsr1.next() ^ _lfsr2.next()) <<9)
    );
    return byte;
}

void Cipher::g(uint8_t &byte, const uint64_t &gamma)
{
#define phi(x) (((x) ^ ( ((x) >>2) & ((x) >>3) & ((x) >>7) )) &1u )
    byte = ((byte >>1) &0x7Fu)
         ^ ((phi(byte) ^ (gamma &1u)) <<7);
}

void Cipher::inv_g(uint8_t &byte, const uint64_t &gamma)
{
    byte = ((byte <<1) &0xFEu)
         ^ phi(((byte <<1) &0xFF) ^ ((byte >>7) &1u))
         ^ (gamma &1u);
}

void Cipher::G(uint8_t &byte, const uint64_t &gammas)
{
    g(byte, gammas >>0); g(byte, gammas >>1);
    g(byte, gammas >>2); g(byte, gammas >>3);
    g(byte, gammas >>4); g(byte, gammas >>5);
    g(byte, gammas >>6); g(byte, gammas >>7);
    g(byte, gammas >>8); g(byte, gammas >>9);
}

void Cipher::inv_G(uint8_t &byte, const uint64_t &gammas)
{
    inv_g(byte, gammas >>9); inv_g(byte, gammas >>8);
    inv_g(byte, gammas >>7); inv_g(byte, gammas >>6);
    inv_g(byte, gammas >>5); inv_g(byte, gammas >>4);
    inv_g(byte, gammas >>3); inv_g(byte, gammas >>2);
    inv_g(byte, gammas >>1); inv_g(byte, gammas >>0);
}


std::vector<std::vector<uint8_t>> Cipher::computeG()
{
    std::vector<std::vector<uint8_t>> allpermG(1 <<10,
                std::vector<uint8_t>(1 <<8));

    for (uint64_t x, gammas = 0; gammas < (1ull <<10); ++gammas) {
        for (x = 0; x < (1ull <<8); ++x) {
            uint8_t y = x; G(y, gammas);
            allpermG[gammas][x] = y;
        }
    }

    return allpermG;
}
