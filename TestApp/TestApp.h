#pragma once
#include "VultureApp.h"
#include "graphics/VultureGPUService.h"
#include "views/VultureTerrainView.h"

class TestSplineApp :
	public VultureApp
{
public:
	TestSplineApp();
	~TestSplineApp();
	void run();
	void update();

	void setupScene();

	void addTree();
	void addCreature();

protected:

	Timer _timer;

	TerrainViewRef _terrain;
	double startTime;
	VulkanShaderRef _skeletonComputeShader;
	VulkanShaderRef _skeletonRenderShader;

	VultureGPUService::SceneGlobals _sceneGlobals;
	shared_ptr<VultureGPUService> gpuService;
};

