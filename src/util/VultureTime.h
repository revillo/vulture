#pragma once

#include <chrono>
#include <ctime>
#include <thread>


class Time {

public:

	typedef double Seconds;
	typedef long long Milliseconds;

	using Clock = std::chrono::high_resolution_clock;
	using Ms = std::chrono::milliseconds;
	using Sec = std::chrono::seconds;
	template<class Duration>
	using TimePoint = std::chrono::time_point<Clock, Duration>;

	static Seconds nowSec() {
		
		return nowMS() / 1000.0;

	}

	static Milliseconds nowMS() {
		auto t = Clock::now().time_since_epoch();
		return std::chrono::duration_cast<Ms>(t).count();
	}

	static void pauseThreadMS(uint32 milliseconds) {
		std::this_thread::sleep_for(Ms(milliseconds));
	}
};