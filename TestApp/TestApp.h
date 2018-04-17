#pragma once
#include "VultureApp.h"
#include "graphics/VultureGPUService.h"
#include "views/VultureTerrainView.h"
#include "views/VultureTreeView.h"

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

	TerrainViewRef _terrainView;
	TreeViewRef _treeView;
	double startTime;
	VulkanShaderRef _skeletonComputeShader;
	VulkanShaderRef _skeletonRenderShader;

	VultureGPUService::SceneGlobals _sceneGlobals;
	shared_ptr<VultureGPUService> gpuService;
};

