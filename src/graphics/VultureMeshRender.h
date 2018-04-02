#pragma once
#include "Vulcro.h"
#include "graphics/VultureMeshCompute.h"

class VultureMeshRender{

public:

	static shared_ptr<VultureMeshRender> make(VulkanPipelineRef pipeline, VultureMeshComputeRef meshCompute) {
		return make_shared<VultureMeshRender>(pipeline, meshCompute);
	}

	VultureMeshRender(VulkanPipelineRef pipeline, VultureMeshComputeRef meshCompute);
	~VultureMeshRender() {}

	virtual void recordDraw(vk::CommandBuffer * cmd, VulkanUniformSetRef sceneGlobalSet);

protected:

	VultureMeshComputeRef _meshCompute;
	VulkanPipelineRef _pipeline;
};

typedef shared_ptr<VultureMeshRender> VultureMeshRenderRef;