#pragma once

#include "Vulcro.h"

class VultureApp {

protected:
	VultureApp();

	~VultureApp();



	unique_ptr<VulkanWindow> _window;

};