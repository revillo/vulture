#include "VultureGPUService.h"
#include "util/VultureTime.h"

#define SCENE_TASK_POOL 0
#define COMPOSITE_POOL 1
#define COMPUTE_TASK_POOL 2

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

	_sceneRenderer = _ctx->makeRenderer();
	_sceneRenderer->targetImages(_sceneTargets, true);

}

void VultureGPUService::setupCompute()
{
	_computeTasks = _ctx->makeTaskGroup(1, COMPUTE_TASK_POOL);
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

	_sceneTasks->at(0)->record([=](vk::CommandBuffer * cmd) {

			cmd->setViewport(0, 1, &_sceneRenderer->getFullViewport());

			cmd->setScissor(0, 1, &_sceneRenderer->getFullRect());

			_sceneRenderer->record(cmd, [=]() {

				for (auto & render : _meshRenders) {
					render->recordDraw(cmd);
				}

			});
	});

}

void VultureGPUService::recordComputeTasks()
{
	_computeTasks->at(0)->record([=](vk::CommandBuffer * cmd) {

		for (auto & cmp : _meshComputes) {
			cmp->recordCompute(cmd);
		}
		
	});
}

bool VultureGPUService::update()
{
	

		//Wait for next available frame
		if (!_swapchain->nextFrame()) {
			resize();
			return true;
		}


		auto tStart = Time::nowSec();
		
		_computeTasks->at(0)->execute(false, {}, { _computeSemaphore });

		_sceneTasks->at(0)->execute(false, { _computeSemaphore }, { _sceneSemaphore });

		

		//Submit command buffer
		_compositeTasks->at(_swapchain->getRenderingIndex())->execute(
			true,
			{ 
				_swapchain->getSemaphore(), 
				_sceneSemaphore
			}
		);

		
		//Present current frame to screen
		if (!_swapchain->present()) {
			resize();
			return true;
		}

		return true;
}

void VultureGPUService::addMeshRender(VultureMeshRenderRef render)
{
	_meshRenders.push_back(render);
}

void VultureGPUService::addMeshCompute(VultureMeshComputeRef meshCompute)
{
	_meshComputes.push_back(meshCompute);
}
