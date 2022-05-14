#include "Timer.h"

namespace Tools {

	Timer::Timer() {
		beg_ = clock_::now();
	}

	void Timer::Reset() {
		beg_ = clock_::now();
	}

	double Timer::Elapsed() {
		return std::chrono::duration_cast<second_>(clock_::now() - beg_).count();
	}

}