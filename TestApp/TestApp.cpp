#include "TestApp.h"



TestApp::TestApp() :
	VultureApp(ivec2(800, 800))
{
	auto gpuService = VultureGPUService::make(&_window);

	auto vctx = _window.getContext();

	auto diffuseTarget = vctx->makeImage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled, _windowSize, vk::Format::eR8G8B8A8Unorm);
	diffuseTarget->allocateDeviceMemory();
	diffuseTarget->createImageView();
	diffuseTarget->createSampler();

	gpuService->setupScene({
		diffuseTarget	
	});

	gpuService->setupComposite("shaders/composite_vert.spv", "shaders/composite_frag.spv");

	gpuService->setupCompute();

	auto testCompute = VultureMeshCompute::make(vctx, "shaders/test_comp.spv", 3 * sizeof(vec4), 1, 3);
	
	auto shader = vctx->makeShader("shaders/test_vert.spv", "shaders/test_frag.spv", {}, {
		testCompute->getUniformSetLayout()
	});
	
	auto tshader = vctx->makeTessShader(
		"shaders/tess/tess_vert.spv",
		"shaders/tess/tess_tesc.spv",
		"shaders/tess/tess_tese.spv",
		"shaders/tess/tess_geom.spv",
		"shaders/tess/tess_frag.spv",
		 {}, {
		testCompute->getUniformSetLayout()
	});

	auto testRender = VultureMeshRender::make(
		vctx->makePipeline(tshader, gpuService->getSceneRenderer(), 
			PipelineConfig({vk::PrimitiveTopology::ePatchList})),
	testCompute);

	gpuService->addMeshCompute(testCompute);
	gpuService->addMeshRender(testRender);
	 
	gpuService->recordComputeTasks();
	gpuService->recordSceneTasks();
	gpuService->recordCompositeTasks();

	runService(gpuService, 10);
	run();

}


TestApp::~TestApp() 
{
		
}

#include "graphics/VultureSplineCompute.h"

TestSplineApp::TestSplineApp() :
VultureApp(ivec2(800, 800))
{
	auto gpuService = VultureGPUService::make(&_window);

	auto vctx = _window.getContext();

	auto diffuseTarget = vctx->makeImage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled, _windowSize, vk::Format::eR8G8B8A8Unorm);
	diffuseTarget->allocateDeviceMemory();
	diffuseTarget->createImageView();
	diffuseTarget->createSampler();

	gpuService->setupScene({
		diffuseTarget
	});
	gpuService->setupComposite("shaders/composite_vert.spv", "shaders/composite_frag.spv");
	gpuService->setupCompute();



	auto testCompute = make_shared<VultureSplineCompute>(vctx);


	auto tshader = vctx->makeTessShader(
		"shaders/spline/spline_vert.spv",
		"shaders/spline/spline_tesc.spv",
		"shaders/spline/spline_tese.spv",
		"shaders/spline/spline_geom.spv",
		"shaders/spline/spline_frag.spv",
		 {}, {
			 testCompute->getUniformSetLayout()
		 });

	auto testRender = VultureMeshRender::make(
		vctx->makePipeline(tshader, gpuService->getSceneRenderer(),
			PipelineConfig({ vk::PrimitiveTopology::ePatchList })),
		testCompute);

	gpuService->addMeshCompute(testCompute);
	gpuService->addMeshRender(testRender);

	gpuService->recordComputeTasks();
	gpuService->recordSceneTasks();
	gpuService->recordCompositeTasks();

	runService(gpuService, 10);
	run();
}

TestSplineApp::~TestSplineApp()
{
}
