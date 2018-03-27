#include "VultureApp.h"

void VultureApp::run()
{

	_appStartTime = Time::nowSec();

	_runFuts.push_back(Thread::async([=]() {

		_window.run([]() {

		});
		
	}));

	_appStartTime = Time::nowSec();
}

VultureApp::VultureApp() :
	_window(0, 0, 500, 500, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE)
	//,_runFut()
{
	
}

VultureApp::~VultureApp()
{

}
