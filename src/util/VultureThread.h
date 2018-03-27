#pragma once
#include <future>
#include <functional>

class Thread {


public:
	typedef std::future<void> Fut;

	static inline Fut async(std::function<void()> runnable) {
		return std::async(std::launch::async, runnable);
	}

};