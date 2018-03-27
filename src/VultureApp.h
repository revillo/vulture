#pragma once

#include "Vulcro.h"
#include "util/VultureTime.h"
#include "util/VultureThread.h"

class VultureApp {

public:

	virtual void run();
	
	

	VultureApp();

	~VultureApp();

private:

	Time::Seconds _appTime;

	Time::Seconds _appStartTime;

	VulkanWindow _window;

	vector<Thread::Fut> _runFuts;
};