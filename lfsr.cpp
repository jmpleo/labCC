#include "lfsr.h"
#include <cstdint>
#include <iostream>



LFSR::LFSR(uint64_t size, uint64_t poly, uint64_t seed)
    : _size(size)
    , _poly(poly)
    , _state(seed)
{}

LFSR::~LFSR()
{}

void LFSR::setState(uint64_t state) { _state = state; }

uint64_t LFSR::next()
{
    uint64_t
        poped = _state &1,
        pushed = (_state & _poly) & ((1ull <<_size) -1),
        shuffle = pushed;

    pushed &= 1;
    while (shuffle) {
        pushed ^= ((shuffle>>=1) &1);
    }
    _state = (_state>>1) ^ ((pushed &1) << (_size-1));
    return poped;
}

