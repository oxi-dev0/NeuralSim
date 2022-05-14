#pragma once

#include <chrono>

namespace Tools {

    class Timer
    {
    public:
        Timer();
        void Reset();
        double Elapsed();

    private:
        typedef std::chrono::high_resolution_clock clock_;
        typedef std::chrono::duration<double, std::ratio<1> > second_;
        std::chrono::time_point<clock_> beg_;
    };

}