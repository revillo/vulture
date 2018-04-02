#pragma once
#include "VultureApp.h"
#include "graphics/VultureGPUService.h"

class TestApp :
	public VultureApp
{
public:
	TestApp();
	~TestApp();
};

class TestSplineApp :
	public VultureApp
{
public:
	TestSplineApp();
	~TestSplineApp();
	void run();
	void update();

protected:

	double startTime;
	VultureGPUService::SceneGlobals _sceneGlobals;
	shared_ptr<VultureGPUService> gpuService;
};

