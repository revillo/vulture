#include "VultureApp.h"



VultureApp::VultureApp(ivec2 windowSize) :
	_windowSize(windowSize),
	_window(0, 0, _windowSize.x, _windowSize.y, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE)
	//,_runFut()
{
	
}

VultureApp::~VultureApp()
{
	
}

void VultureApp::run() {
	_window.run([=]() {
	
	});
}

void VultureApp::runService(VultureServiceRef service, uint32 interval)
{

	_services.emplace_back(service, interval, Thread::async([=]() {

		while (service->update()) {
			Time::pauseThreadMS(interval);
		}

	}));
	
}
