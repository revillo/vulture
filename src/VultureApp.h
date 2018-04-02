#pragma once

#include "Vulcro.h"
#include "util/VultureTime.h"
#include "util/VultureThread.h"
#include "VultureService.h"

class VultureApp {

	struct ServiceHandler {

		ServiceHandler(VultureServiceRef _service, uint32 _interval, Thread::Fut &_fut) :
			service(_service)
			,interval(_interval)
			,fut(std::move(_fut))
		{}

		VultureServiceRef service;
		uint32 interval;
		Thread::Fut fut;
	};

public:

	virtual void run();
	void runService(VultureServiceRef service, uint32 interval);

	virtual void killServices();
	
protected:

	VultureApp(ivec2 windowSize);

	~VultureApp();

	ivec2 _windowSize;

	Time::Seconds _appTime;

	Time::Seconds _appStartTime;

	VulkanWindow _window;

	vector<ServiceHandler> _services;
};