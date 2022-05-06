#pragma once
#ifndef RANDOM
#define RANDOM

#include <cstdint>
#include <cassert>
#include <cmath>
#include <random>
#include <chrono>
#include <climits>

namespace Random {
	constexpr uint32_t RANDOM_UINT_MAX = 0xffffffff;

    struct RandomGenerator {
    private:
        uint32_t a, b, c, d;
    public:
        RandomGenerator();
        uint32_t operator()();
        unsigned operator()(unsigned min, unsigned max);
    };
}

#endif