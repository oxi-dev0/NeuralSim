#include "Random.h"

namespace Random {
    RandomGenerator::RandomGenerator()
    {
        // Non-deterministic initialization.
        std::mt19937 generator(time(0));

        // Initialize Jenkins, but don't let any of the values be zero:
        a = 0xf1ea5eed;
        do { b = c = d = generator(); } while (b == 0);
    }

    uint32_t RandomGenerator::operator()()
    {
        // Jenkins algorithm
#define rot32(x,k) (((x)<<(k))|((x)>>(32-(k))))
        uint32_t e = a - rot32(b, 27);
        a = b ^ rot32(c, 17);
        b = c + d;
        c = d + e;
        d = e + a;
        return d;
    }

    unsigned RandomGenerator::operator()(unsigned min, unsigned max)
    {
        assert(max >= min);
        return ((*this)() % (max - min + 1)) + min;
    }
}