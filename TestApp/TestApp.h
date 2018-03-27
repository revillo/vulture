#pragma once
#include "VultureApp.h"


class TestService :
	public VultureService
{
public:
	bool update() {
		std::cout << counter++ << std::endl;
		return counter < 10;
	}
	TestService() {}
	~TestService() {}

private:

	int counter = 0;
};

class TestApp :
	public VultureApp
{
public:
	TestApp();
	~TestApp();
};

