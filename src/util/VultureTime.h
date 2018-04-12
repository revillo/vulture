#pragma once

#include <chrono>
#include <ctime>
#include <thread>


class Timer {

public:

	typedef double Seconds;
	typedef double Milliseconds;

	using Clock = std::chrono::high_resolution_clock;
	
	chrono::time_point<Clock> StartTimePoint;

	Timer() {
		StartTimePoint = Clock::now();
	}

	Seconds getSec() {
		chrono::time_point<Clock> n = Clock::now();
		return chrono::duration_cast<chrono::microseconds>(n - StartTimePoint).count() * 1e-6;
	}

	Milliseconds getMS() {
		chrono::time_point<Clock> n = Clock::now();
		return chrono::duration_cast<chrono::microseconds>(n - StartTimePoint).count() * 1e-3;
	}


	
};

