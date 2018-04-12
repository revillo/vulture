#pragma once

#include "VultureService.h"
#include "graphics/VultureMeshRender.h"
#include "graphics/VultureMeshCompute.h"
#include "Vulcro.h"
#include <mutex>

class VultureGPUService : public VultureService
{
public:

	struct SceneGlobals {
		mat4 perspective;
		mat4 view;
		vec4 viewPos;
		vec4 sunDirWorld;
		vec4 time;
	};


	static shared_ptr<VultureGPUService> make(VulkanWindow * window) {
		return make_shared<VultureGPUService>(window);
	}

	VultureGPUService(VulkanWindow * window);
	~VultureGPUService();

	virtual void setupScene(vector<VulkanImageRef> targets);
	virtual void setupComposite(const char * vertPath, const char * fragPath);
	virtual void setupCompute();

	virtual void computeOnce(ComputableRef compute);
	
	virtual void recordCompositeTasks();
	virtual void recordSceneTasks();
	virtual void recordComputeTasks();

	virtual void setSceneGlobals(SceneGlobals globals);

	virtual bool update();
	virtual void addMeshRender(MeshRenderRef meshRender);
	virtual void addCompute(ComputableRef compute);
	
	VulkanUniformSetLayoutRef getSceneGlobalsLayout() {
		return _sceneGlobalLayout;
	}
	
	VulkanRendererRef getSceneRenderer() {
		return _sceneRenderer;
	}

	VulkanContextRef getContext() {
		return _ctx;
	}
	
protected:

	virtual void resize();

	
	struct VertexUV {
		vec2 position;
		vec2 uv;
	};
	
	mutex _setGlobalsLock, _computeLock, _sceneLock;
	
	atomic<bool> _computeDirty = false;
	atomic<bool> _sceneDirty = false;

	shared_ptr<vbo<VertexUV>> _blitVBO;
	shared_ptr<ibo> _blitIBO;

	shared_ptr<ubo<SceneGlobals>> _uScene;

	VulkanUniformSetRef _blitUniformSet;
	VulkanUniformSetLayoutRef _blitUniformLayout;

	VulkanUniformSetRef _sceneGlobalSet;
	VulkanUniformSetLayoutRef _sceneGlobalLayout;

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
	vector<MeshRenderRef> _meshRenders;
	vector<ComputableRef> _meshComputes;
};