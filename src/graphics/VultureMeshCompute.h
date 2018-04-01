#pragma once
#include "Vulcro.h"

class VultureMeshCompute {

public:

	static shared_ptr<VultureMeshCompute> make(VulkanContextRef ctx, const char * computePath, uint32 bufferSize, uint32 numBuffers, uint32 groupSize) {
		return make_shared<VultureMeshCompute>(ctx, computePath, bufferSize, numBuffers, groupSize);
	}

	VultureMeshCompute(VulkanContextRef ctx, const char * computePath, uint32 bufferSize, uint32 numBuffers, uint32 groupSize);

	virtual void recordCompute(vk::CommandBuffer * cmd);

	VulkanBufferRef getSSBO() {
		return _ssbo;
	}

	VulkanUniformSetRef getUniformSet() {
		return _uniformSet;
	}

	VulkanUniformSetLayoutRef getUniformSetLayout() {
		return _uniformLayout;
	}

	virtual uint32 getNumVerts();

	~VultureMeshCompute();

protected:

	VultureMeshCompute(VulkanContextRef ctx);

	uint32 _bufferSize;

	VulkanContextRef _ctx;

	VulkanComputePipelineRef _pipeline;

	VulkanUniformSetLayoutRef _uniformLayout;

	VulkanUniformSetRef _uniformSet;

	VulkanBufferRef _ssbo;

	VulkanShaderRef _shader;

	uint32 _groupSize;
};

typedef shared_ptr<VultureMeshCompute> VultureMeshComputeRef;