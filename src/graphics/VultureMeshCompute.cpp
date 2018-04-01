#include "VultureMeshCompute.h"

VultureMeshCompute::VultureMeshCompute(VulkanContextRef ctx, const char * computePath, uint32 bufferSize, uint32 numBuffers, uint32 groupSize)
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

void VultureMeshCompute::recordCompute(vk::CommandBuffer * cmd)
{
	_pipeline->bind(cmd);

	_pipeline->bindUniformSets(cmd, {
		_uniformSet
	});

	cmd->dispatch(getNumVerts() / _groupSize, 1, 1);

	return;

}

uint32 VultureMeshCompute::getNumVerts()
{
	return 3;
}

VultureMeshCompute::~VultureMeshCompute()
{
}

VultureMeshCompute::VultureMeshCompute(VulkanContextRef ctx) :
	_ctx(ctx)
{

}
