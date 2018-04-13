#include "TestApp.h"

#include "graphics/VultureSplineCompute.h"


TestSplineApp::TestSplineApp() :
	VultureApp(ivec2(800, 800))
{
	gpuService = VultureGPUService::make(&_window);

	setupScene();
	
	//addTree();
	//addCreature();

	auto grid = Primitives::Grid(2, 2);

	_terrain = make_shared<TerrainView>(gpuService.get());
	_terrain->show();

	gpuService->recordComputeTasks();
	gpuService->recordSceneTasks();
	gpuService->recordCompositeTasks();

	runService(gpuService, 10);
	run();
}

TestSplineApp::~TestSplineApp()
{
}

void TestSplineApp::update() {
	float t = _timer.getSec();

	t *= 0.1f;
	float radius = 8.0;

	_sceneGlobals.viewPos = vec4(sin(t) * radius, 2.0, cos(t) * radius, 1.0);

	_sceneGlobals.view = lookAt(
		vec3(_sceneGlobals.viewPos),
		vec3(_sceneGlobals.viewPos) * 0.9f - vec3(0.0, 0.2, 0.0),
		vec3(0.0, 1.0, 0.0)
	);

	gpuService->setSceneGlobals(_sceneGlobals);
}

void TestSplineApp::setupScene()
{
	auto vctx = _window.getContext();

	auto diffuseTarget = vctx->makeImage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled, _windowSize * 2, vk::Format::eR8G8B8A8Unorm);
	diffuseTarget->allocateDeviceMemory();
	diffuseTarget->createImageView();
	diffuseTarget->createSampler();

	gpuService->setupScene({
		diffuseTarget
		});
	gpuService->setupComposite("shaders/composite_vert.spv", "shaders/composite_frag.spv");
	gpuService->setupCompute();

	_sceneGlobals.perspective = vulcro::glProjFixYZ * glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 1000.0f);
	_sceneGlobals.time.x = 0.0;
	_sceneGlobals.viewPos = vec4(0.0, 0.0, 5.0, 1.0);

	_sceneGlobals.view = glm::lookAt(
		vec3(_sceneGlobals.viewPos),
		vec3(0.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0)
	);

	_sceneGlobals.sunDirWorld = vec4(0.0, 1.0, 0.0, 0.0);

	gpuService->setSceneGlobals(_sceneGlobals);

}

struct Vertex {
	vec4 pos;
	vec4 quat;
};

struct Link {
	uint pi0;
	uint pi1;
	uint c0;
	uint c1;
};

struct Skeleton {
	vec4 info;
	mat4 modelMatrix;
	Vertex verts[20];
	Link links[20];
};


void TestSplineApp::addTree()
{

	Skeleton s2s[1];
	auto &sk = s2s[0];

	sk.modelMatrix = translate(mat4(1.0), vec3(0.0, 2.0, 0.0));


	auto vctx = _window.getContext();

	sk.verts[0] = { { 0.0, 0.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0, -radians(90.0f) } };
	sk.verts[1] = { { 0.0, 3.0, 0.0, 1.0 },{ 1.0, 0.0, 0.0, -radians(90.0f) } };
	sk.verts[2] = { { 3.0, 3.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0, radians(90.0f) } };
	sk.verts[3] = { { 0.0, 6.0, 0.0, 1.0 },{ 0.0, 1.0, 0.0, 0.0 } };
	sk.verts[4] = { { 0.0, 9.0, 0.0, 1.0 },{ 0.0, 1.0, 0.0, 0.0 } };

	sk.info.x = 1;

	sk.links[0].pi0 = 0;
	sk.links[0].pi1 = 1;
	sk.links[0].c0 = 10;
	sk.links[0].c1 = 10;

	sk.links[1].pi0 = 1;
	sk.links[1].pi1 = 2;
	sk.links[1].c0 = 22;
	sk.links[1].c1 = 0;

	sk.links[2].pi0 = 1;
	sk.links[2].pi1 = 3;
	sk.links[2].c0 = 21;
	sk.links[2].c1 = 10;

	sk.links[3].pi0 = 3;
	sk.links[3].pi1 = 4;
	sk.links[3].c0 = 10;
	sk.links[3].c1 = 0;


	auto skelCompute = VultureSplineCompute<Skeleton>::make(vctx, 1, 4, s2s);


	_skeletonComputeShader = vctx->makeTessShader(
		"shaders/spline/spline_vert.spv",
		"shaders/spline/spline_tesc.spv",
		"shaders/spline/spline_tese.spv",
		"shaders/spline/spline_geom.spv",
		"shaders/spline/spline_frag.spv",
		 {}, {
			 gpuService->getSceneGlobalsLayout(),
			 skelCompute->getUniformSetLayout()
		 });

	auto skelRender = MeshRender::make(
		vctx->makePipeline(_skeletonComputeShader, gpuService->getSceneRenderer(),
			PipelineConfig({ vk::PrimitiveTopology::ePatchList, 4 })),
		skelCompute);

	gpuService->addCompute(skelCompute);
	gpuService->addMeshRender(skelRender);

}



void TestSplineApp::addCreature() {

	Skeleton skel[1];
	auto &sk = skel[0];

	auto vctx = _window.getContext();

	//back
	sk.verts[0] = { { 0.0, 0.0, -2.0, 1.0 },{ 0.0, 1.0, 0.0, radians(0.0f) } };
	sk.verts[1] = { { 0.0, 0.0, 2.0, 1.0 },{  0.0,  1.0, 0.0, 0.0f} };


	float ls = 0.5;
	float lt = radians(90.0f);
	//forelegs
	sk.verts[2] = { { -2.0, 0.0,  2.0, ls },{ 0.0, 1.0, 0.0, radians(90.0f) } };
	sk.verts[3] = { { 2.0, 0.0,  2.0, ls }, { 0.0, 1.0, 0.0, -radians(90.0f) } };

	//backlegs
	sk.verts[4] = { { -2.0, 0.0,  -2.0, ls },{ 0.0, 1.0, 0.0, radians(90.0f) } };
	sk.verts[5] = { { 2.0, 0.0,  -2.0, ls },{ 0.0, 1.0, 0.0, -radians(90.0f) } };


	sk.info.x = 5;

	sk.links[0].pi0 = 0;
	sk.links[0].pi1 = 1;
	sk.links[0].c0 = 32;
	sk.links[0].c1 = 30;

	sk.links[1].pi0 = 1;
	sk.links[1].pi1 = 2;
	sk.links[1].c0 = 33;
	sk.links[1].c1 = 10;


	sk.links[2].pi0 = 1;
	sk.links[2].pi1 = 3;
	sk.links[2].c0 = 31;
	sk.links[2].c1 = 10;


	sk.links[3].pi0 = 0;
	sk.links[3].pi1 = 4;
	sk.links[3].c0 = 33;
	sk.links[3].c1 = 10;

	sk.links[4].pi0 = 0;
	sk.links[4].pi1 = 5;
	sk.links[4].c0 = 31;
	sk.links[4].c1 = 10;


	sk.modelMatrix = mat4(1.0);


	auto skelCompute = VultureSplineCompute<Skeleton>::make(vctx, 1, 20, skel);

	auto skelRender = MeshRender::make(
		vctx->makePipeline(_skeletonComputeShader, gpuService->getSceneRenderer(),
			PipelineConfig({ vk::PrimitiveTopology::ePatchList, 4 })),
		skelCompute);

	gpuService->computeOnce(skelCompute);
	gpuService->addMeshRender(skelRender);

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