#pragma once
#include <future>
#include <functional>

class Thread {


public:
	typedef std::future<void> Fut;

	static inline Fut async(std::function<void()> runnable) {
		return std::async(std::launch::async, runnable);
	}

	static void pauseMS(uint32 milliseconds) {
		std::this_thread::sleep_for(chrono::milliseconds(milliseconds));
	}

};