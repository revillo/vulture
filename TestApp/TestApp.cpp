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
	gpuService = VultureGPUService::make(&_window);

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

	_sceneGlobals.perspective = vulcro::glProjFixYZ * glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
	_sceneGlobals.time.x = 0.0;
	_sceneGlobals.viewPos = vec4(0.0, 0.0, 5.0, 1.0);

	_sceneGlobals.view = glm::lookAt(
		vec3(_sceneGlobals.viewPos),
		vec3(0.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0)
	);

	_sceneGlobals.sunDirWorld = vec4(0.0, 1.0, 0.0, 0.0);

	gpuService->setSceneGlobals(_sceneGlobals);

	struct Spline {
		vec4 points[2];
		vec4 normals[2];
		vec4 tangents[2];
	};

	#define MAX_SPLINES 2

	struct Skeleton {
		mat4 modelMatrix;
		uvec4 info;
		Spline splines[MAX_SPLINES];
	};

	auto skeleton = Skeleton();

	skeleton.modelMatrix = glm::mat4(1.0);
	skeleton.info.x = 2;



	skeleton.splines[0] = {
		{ {-1.3, 0.0, 0.0, 1.0}, {0.7, 0.0, 0.0, 1.0}  },
		{ {0.0, 1.0, 0.0, 1.0},  {-0.7, 0.7, 0.0, 1.0}  },
		{ {1.0, 0.0, 0.0, 1.0},  {0.7, 0.7, 0.0, 1.0 } }
	};
	skeleton.splines[1] = {
		{ skeleton.splines[0].points[1],  { 1.0, 1.9, 0.0, 1.0 } },
		{ skeleton.splines[0].normals[1], { -0.4, 0.0, 0.0, 1.0 } },
		{ skeleton.splines[0].tangents[1], { 0.0, 1.0, 0.0, 1.0 } }
	};


	auto skelCompute = VultureSplineCompute<Skeleton>::make(vctx, 1, MAX_SPLINES, &skeleton);


	auto skelShader = vctx->makeTessShader(
		"shaders/spline/spline_vert.spv",
		"shaders/spline/spline_tesc.spv",
		"shaders/spline/spline_tese.spv",
		"shaders/spline/spline_geom.spv",
		"shaders/spline/spline_frag.spv",
		 {}, {
			 gpuService->getSceneGlobalsLayout(),
			 skelCompute->getUniformSetLayout()
		 });

	auto skelRender = VultureMeshRender::make(
		vctx->makePipeline(skelShader, gpuService->getSceneRenderer(),
			PipelineConfig({ vk::PrimitiveTopology::ePatchList })),
		skelCompute);

	gpuService->addMeshCompute(skelCompute);
	gpuService->addMeshRender(skelRender);

	gpuService->recordComputeTasks();
	gpuService->recordSceneTasks();
	gpuService->recordCompositeTasks();

	startTime = Time::nowSec();

	runService(gpuService, 10);
	run();
}

TestSplineApp::~TestSplineApp()
{
}

void TestSplineApp::update() {
	float t = _sceneGlobals.time.x = static_cast<float>(Time::nowSec() - startTime);
	t *= 0.5f;

	_sceneGlobals.view = lookAt(
		vec3(sin(t), 0.0, cos(t)) * 5.0f,
		vec3(0.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0)
	);

	gpuService->setSceneGlobals(_sceneGlobals);
}

void TestSplineApp::run() {

	bool stillRunning = true;

	while (stillRunning) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			switch (event.type) {

			case SDL_QUIT:
				stillRunning = false;
				break;


			default:
				// Do nothing.
				break;
			}

		}

		update();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE]) {
			stillRunning = false;
		}
	}

	killServices();

}