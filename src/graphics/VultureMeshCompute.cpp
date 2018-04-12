#include "VultureMeshCompute.h"

ComputeMesh::ComputeMesh(VulkanContextRef ctx, const char * computePath, uint32 bufferSize, uint32 numBuffers, uint32 groupSize)
	:_ctx(ctx)
	, _bufferSize(bufferSize)
	, _groupSize(groupSize)
{

	_ssbo = std::make_shared<VulkanBuffer>(_ctx, vk::BufferUsageFlagBits::eStorageBuffer, bufferSize * numBuffers, VulkanBuffer::CPU_NEVER);

	_uniformLayout = ctx->makeUniformSetLayout({
		ULB(numBuffers, vk::DescriptorType::eStorageBuffer, nullptr)
	});
	
	_shader = ctx->makeComputeShader(computePath, {
		_uniformLayout	
	});

	_pipeline = ctx->makeComputePipeline(_shader);

	_uniformSet = ctx->makeUniformSet(_uniformLayout);

	_uniformSet->bindBuffer(0, _ssbo->getDBI(), vk::DescriptorType::eStorageBuffer);
	
}

void ComputeMesh::recordCompute(vk::CommandBuffer * cmd)
{
	_pipeline->bind(cmd);

	_pipeline->bindUniformSets(cmd, {
		_uniformSet
	});

	cmd->dispatch(getNumVerts() / _groupSize, 1, 1);

	return;

}

void ComputeMesh::draw(vk::CommandBuffer * cmd, uint32 numInstances)
{
	cmd->draw(getNumVerts(), numInstances, 0, 0);
}



ComputeMesh::ComputeMesh(VulkanContextRef ctx) :
	_ctx(ctx)
{

}
