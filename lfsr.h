#ifndef LFSR_H
#define LFSR_H

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <random>


class LFSR
{
public:
    LFSR(uint64_t size, uint64_t poly, uint64_t seed);
    ~LFSR();

    uint64_t next();
    void reset(uint64_t seed);

private:
    uint64_t _size;
    uint64_t _poly;
    uint64_t _state;
};

#endif
