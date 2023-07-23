#include "cipher.h"
#include <cstdint>


Cipher::Cipher(uint64_t poly1, uint64_t poly2, uint64_t l1, uint64_t l2)
    : _lfsr1(l1, poly1, 0)
    , _lfsr2(l2, poly2, 0)
{ }

Cipher::~Cipher()
{ }
