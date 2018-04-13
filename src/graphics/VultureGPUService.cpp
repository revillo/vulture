#include "VultureGPUService.h"
#include "util/VultureTime.h"

#define SCENE_TASK_POOL 0
#define COMPOSITE_POOL 1
#define COMPUTE_TASK_POOL 2
#define COMPUTE_ONCE_POOL 3

VultureGPUService::VultureGPUService(VulkanWindow * window)
	:
	_ctx(window->getContext())
{
	
	_swapchain = _ctx->makeSwapchain(window->getSurface());
	_computeSemaphore = _ctx->getDevice().createSemaphore(vk::SemaphoreCreateInfo());

	_sceneSemaphore = _ctx->getDevice().createSemaphore(vk::SemaphoreCreateInfo());

}

void VultureGPUService::setupScene(vector<VulkanImageRef> targets) {
	_sceneTargets = targets;

	_sceneTasks = _ctx->makeTaskGroup(1, SCENE_TASK_POOL);
	_uScene = _ctx->makeUBO<SceneGlobals>(1);

	_sceneRenderer = _ctx->makeRenderer();
	_sceneRenderer->targetImages(_sceneTargets, true);

	_sceneGlobalLayout = _ctx->makeUniformSetLayout({
		_uScene->getLayout()
	});

	_sceneGlobalSet = _ctx->makeUniformSet(_sceneGlobalLayout);
	_sceneGlobalSet->bindBuffer(0, _uScene->getDBI());
}

void VultureGPUService::setupCompute()
{
	_computeTasks = _ctx->makeTaskGroup(1, COMPUTE_TASK_POOL);
}

void VultureGPUService::computeNTimes(ComputableRef compute, uint32 n)
{
	auto tempTask = _ctx->makeTask(COMPUTE_ONCE_POOL, false);
	tempTask->record([compute](vk::CommandBuffer * cmd) {
		compute->recordCompute(cmd);
	});
	for (int i = 0; i < n; i++) {
		tempTask->execute(true);
	}
}

void VultureGPUService::computeOnce(ComputableRef compute)
{
	auto tempTask = _ctx->makeTask(COMPUTE_ONCE_POOL, true);
	tempTask->record([compute](vk::CommandBuffer * cmd) {
		compute->recordCompute(cmd);
	});
	tempTask->execute(true);
}

void VultureGPUService::setupComposite(const char * vertPath, const char * fragPath) {

	_compositeRenderer = _ctx->makeRenderer();
	_compositeRenderer->targetSwapcahin(_swapchain, false);


	VertexUV vuvs[4] = {
		{ { -1.0, -1.0 },{ 0.0, 0.0 } },
	{ { -1.0, 1.0 },{ 0.0, 1.0 } },
	{ { 1.0, -1.0 },{ 1.0, 0.0 } },
	{ { 1.0, 1.0 },{ 1.0, 1.0 } }
	};


	_blitVBO = _ctx->makeVBO<VertexUV>({
		vk::Format::eR32G32Sfloat,
		vk::Format::eR32G32Sfloat
		}, 4, vuvs);


	_blitIBO = _ctx->makeIBO({
		0, 1, 2, 1, 3, 2
		});

	vector<ULB> bindings;

	for (auto & img : _sceneTargets) {
		bindings.push_back(ULB(1, vk::DescriptorType::eCombinedImageSampler, &img->getSampler()));
	}

	_blitUniformLayout = _ctx->makeUniformSetLayout(bindings);

	_blitUniformSet = _ctx->makeUniformSet(_blitUniformLayout);

	_blitUniformSet->bindImages(_sceneTargets);

	_blitShader = _ctx->makeShader(vertPath, fragPath, {

		_blitVBO->getLayout()
		
		}, {
		
			_blitUniformLayout

		});

	_blitPipeline = _ctx->makePipeline(_blitShader, _compositeRenderer);

	_compositeTasks = _ctx->makeTaskGroup(_swapchain->numImages(), COMPOSITE_POOL);
}



VultureGPUService::~VultureGPUService()
{
	_ctx->getDevice().destroySemaphore(_sceneSemaphore);
	_ctx->getDevice().destroySemaphore(_computeSemaphore);
}

void VultureGPUService::resize() {
	
}



void VultureGPUService::recordCompositeTasks()
{
	//Record to command buffer
	_compositeTasks->record([=](vk::CommandBuffer * cmd, uint32 taskNumber) {

		cmd->setViewport(0, 1, &_compositeRenderer->getFullViewport());
		cmd->setScissor(0, 1, &_compositeRenderer->getFullRect());

		_compositeRenderer->record(cmd, [=]() {

			_blitPipeline->bind(cmd);

			_blitVBO->bind(cmd);

			_blitIBO->bind(cmd);

			_blitPipeline->bindUniformSets(cmd, {
				_blitUniformSet
			});

			cmd->drawIndexed(_blitIBO->getCount(), 1, 0, 0, 0);

		}, taskNumber);
	});
}

void VultureGPUService::recordSceneTasks()
{
	_sceneLock.lock();
	_sceneTasks->at(0)->record([=](vk::CommandBuffer * cmd) {

			cmd->setViewport(0, 1, &_sceneRenderer->getFullViewport());

			cmd->setScissor(0, 1, &_sceneRenderer->getFullRect());

			_sceneRenderer->record(cmd, [=]() {

				for (auto & render : _meshRenders) {
					render->recordDraw(cmd, _sceneGlobalSet);
				}

			});
	});
	_sceneDirty = false;
	_sceneLock.unlock();

}

void VultureGPUService::recordComputeTasks()
{
	_computeLock.lock();
	_computeTasks->at(0)->record([=](vk::CommandBuffer * cmd) {

		for (auto & cmp : _meshComputes) {
			cmp->recordCompute(cmd);
		}
		
	});

	_computeDirty = false;
	_computeLock.unlock();
}

void VultureGPUService::setSceneGlobals(SceneGlobals globals)
{
	_setGlobalsLock.lock();
	_uScene->at(0) = globals;
	_uScene->sync();
	_setGlobalsLock.unlock();
}

bool VultureGPUService::update()
{

	if (!_stayAlive) {
		return false;
	}

	
	//Wait for next available frame
	if (!_swapchain->nextFrame()) {
		resize();
		return true;
	}


	//auto tStart = Time::nowMS();

	auto tStart = _timer.getSec();

	_setGlobalsLock.lock();
		
	_computeTasks->at(0)->execute(false, {}, { _computeSemaphore });

	_sceneTasks->at(0)->execute(false, { _computeSemaphore }, { _sceneSemaphore });

	_compositeTasks->at(_swapchain->getRenderingIndex())->execute(
		true,
		{ 
			_swapchain->getSemaphore(), 
			_sceneSemaphore
		}
	);

	auto tStop = _timer.getSec();


	std::cout << 1.0 / (tStop - tStart) << std::endl;
	

	_setGlobalsLock.unlock();

		
	//Present current frame to screen
	if (!_swapchain->present()) {
		resize();
		return true;
	}

	return true;
}

void VultureGPUService::addMeshRender(MeshRenderRef render)
{
	_sceneLock.lock();
	_meshRenders.push_back(render);
	_sceneDirty = true;
	_sceneLock.unlock();
}

void VultureGPUService::addCompute(ComputableRef meshCompute)
{
	_computeLock.lock();
	_meshComputes.push_back(meshCompute);
	_computeDirty = true;
	_computeLock.unlock();

}
