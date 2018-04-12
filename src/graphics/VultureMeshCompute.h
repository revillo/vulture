#pragma once
#include "Vulcro.h"
#include "graphics\VultureMesh.h"

class IComputable {
public:

	virtual void recordCompute(vk::CommandBuffer * cmd) = 0;
};

typedef shared_ptr<IComputable> ComputableRef;

class ComputeMesh : public IComputable, public IMesh {

public:

	//IComputable
	virtual void recordCompute(vk::CommandBuffer * cmd) override;


	//IMesh
	VulkanUniformSetRef getUniformSet() override {
		return _uniformSet;
	}

	virtual void draw(vk::CommandBuffer *cmd, uint32 numInstances) override;


	VulkanBufferRef getSSBO() {
		return _ssbo;
	}

	VulkanUniformSetLayoutRef getUniformSetLayout() {
		return _uniformLayout;
	}

	virtual uint32 getNumVerts() = 0;


protected:

	ComputeMesh(VulkanContextRef ctx, const char * computePath, uint32 bufferSize, uint32 numBuffers, uint32 groupSize);

	ComputeMesh(VulkanContextRef ctx);

	uint32 _bufferSize;

	VulkanContextRef _ctx;

	VulkanComputePipelineRef _pipeline;

	VulkanUniformSetLayoutRef _uniformLayout;

	VulkanUniformSetRef _uniformSet;

	VulkanBufferRef _ssbo;

	VulkanShaderRef _shader;

	uint32 _groupSize;
};

typedef shared_ptr<ComputeMesh> ComputeMeshRef;