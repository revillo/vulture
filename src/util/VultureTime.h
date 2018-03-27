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
		auto t = Clock::now();
		long long time = t.time_since_epoch().count();
		return time / 1000.0;
	}

	static Milliseconds nowMS() {
		auto t = Clock::now();
		long long time = t.time_since_epoch().count();
		return time;
	}

};