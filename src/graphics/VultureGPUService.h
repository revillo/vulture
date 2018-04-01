#pragma once

#include "VultureService.h"
#include "graphics/VultureMeshRender.h"
#include "Vulcro.h"

class VultureGPUService : public VultureService
{
	public:

		static shared_ptr<VultureGPUService> make(VulkanWindow * window) {
			return make_shared<VultureGPUService>(window);
		}

		VultureGPUService(VulkanWindow * window);
		
		virtual void setupScene(vector<VulkanImageRef> targets);
		virtual void setupComposite(const char * vertPath, const char * fragPath);
		virtual void setupCompute();

		~VultureGPUService();

		virtual bool update();
		virtual void addMeshRender(VultureMeshRenderRef render);
		virtual void addMeshCompute(VultureMeshComputeRef meshCompute);

		VulkanRendererRef getSceneRenderer() {
			return _sceneRenderer;
		}

		virtual void recordCompositeTasks();
		virtual void recordSceneTasks();
		virtual void recordComputeTasks();

protected:

	virtual void resize();

	
	struct VertexUV {
		vec2 position;
		vec2 uv;
	};
	
	shared_ptr<vbo<VertexUV>> _blitVBO;
	shared_ptr<ibo> _blitIBO;

	VulkanUniformSetRef _blitUniformSet;
	VulkanUniformSetLayoutRef _blitUniformLayout;

	VulkanContextRef _ctx;

	VulkanTaskGroupRef _computeTasks;
	VulkanTaskGroupRef _sceneTasks;
	VulkanTaskGroupRef _compositeTasks;

	vk::Semaphore _sceneSemaphore, _computeSemaphore;

	VulkanSwapchainRef _swapchain;
	VulkanRendererRef _sceneRenderer, _compositeRenderer;
	VulkanPipelineRef _blitPipeline;
	VulkanShaderRef _blitShader;

	vector<VulkanImageRef> _sceneTargets;
	vector<VultureMeshRenderRef> _meshRenders;
	vector<VultureMeshComputeRef> _meshComputes;
};