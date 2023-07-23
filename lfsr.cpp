#include "lfsr.h"
#include <cstdint>



LFSR::LFSR(uint64_t size, uint64_t poly, uint64_t seed)
    : _size(size)
    , _poly(poly)
    , _state(seed)
{}

LFSR::~LFSR()
{}

uint64_t LFSR::next()
{
    uint64_t
        poped = _state & 1,
        pushed = _state ^ _poly,
        shuffle = pushed;

    while (shuffle) {
        pushed ^= (shuffle>>=1);
    }

    _state = (_state>>1) ^ (poped << (_size-1));
    return poped & 1;
}

void LFSR::reset(uint64_t state) { _state = state; }
