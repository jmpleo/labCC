#include "cipher.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>


using namespace cipher;

Cipher::Cipher(uint64_t l1, uint64_t l2)
    : _lfsr1(l1, 0, 0)
    , _lfsr2(l2, 0, 0)
{ }


Cipher::Cipher(uint64_t poly1, uint64_t poly2, uint64_t l1, uint64_t l2)
    : _lfsr1(l1, poly1, 0)
    , _lfsr2(l2, poly2, 0)
{ }


Cipher::~Cipher()
{ }

Bytes Cipher::Enc(Bytes bytes, const uint64_t &k1, const uint64_t &k2)
{
    _lfsr1.setState(k1);
    _lfsr2.setState(k2);

    std::for_each(bytes.begin(), bytes.end(),
        [this](Byte &byte){ byte = EncByte(byte); });

    return bytes;
}

Bytes Cipher::Dec(Bytes bytes, const uint64_t &k1, const uint64_t &k2)
{
    _lfsr1.setState(k1);
    _lfsr2.setState(k2);

    std::for_each(bytes.begin(), bytes.end(),
        [this](Byte &byte){ byte = DecByte(byte); });

    return bytes;
}
Byte Cipher::EncByte(Byte byte)
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

Byte Cipher::DecByte(Byte byte)
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

void Cipher::g(Byte &byte, const uint64_t &gamma)
{
#define PHI(x) (((x) ^ ( ((x) >>2) & ((x) >>3) & ((x) >>7) )) &1u )
    byte = ((byte >>1) &0x7Fu)
         ^ ((PHI(byte) ^ (gamma &1u)) <<7);
}

void Cipher::inv_g(Byte &byte, const uint64_t &gamma)
{
    byte = ((byte <<1) &0xFEu)
         ^ PHI(((byte <<1) &0xFF) ^ ((byte >>7) &1u))
         ^ (gamma &1u);
}

void Cipher::G(Byte &byte, const uint64_t &gammas)
{
    g(byte, gammas >>0); g(byte, gammas >>1);
    g(byte, gammas >>2); g(byte, gammas >>3);
    g(byte, gammas >>4); g(byte, gammas >>5);
    g(byte, gammas >>6); g(byte, gammas >>7);
    g(byte, gammas >>8); g(byte, gammas >>9);
}

void Cipher::inv_G(Byte &byte, const uint64_t &gammas)
{
    inv_g(byte, gammas >>9); inv_g(byte, gammas >>8);
    inv_g(byte, gammas >>7); inv_g(byte, gammas >>6);
    inv_g(byte, gammas >>5); inv_g(byte, gammas >>4);
    inv_g(byte, gammas >>3); inv_g(byte, gammas >>2);
    inv_g(byte, gammas >>1); inv_g(byte, gammas >>0);
}


std::vector<Bytes> Cipher::computeG()
{
    std::vector<std::vector<Byte>> allpermG(1 <<10,
                std::vector<Byte>(1 <<8));

    for (uint64_t x, gammas = 0; gammas < (1ull <<10); ++gammas) {
        for (x = 0; x < (1ull <<8); ++x) {
            Byte y = x; G(y, gammas);
            allpermG[gammas][x] = y;
        }
    }

    return allpermG;
}

std::string Cipher::toHex(Bytes bytes)
{
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0');
    for (Byte &byte : bytes) {
        ss << std::setw(2)
           << static_cast<unsigned int>( byte );
    }
    return ss.str();
}

Bytes Cipher::fromHex(std::string hexStr)
{
    Bytes bytes;
    for (size_t i = 2; i < hexStr.size(); i += 2) {
        uint8_t byte = std::stoi(hexStr.substr(i, 2), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}



