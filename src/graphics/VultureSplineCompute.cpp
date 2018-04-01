#include "VultureSplineCompute.h"

VultureSplineCompute::VultureSplineCompute(VulkanContextRef ctx)
	//:VultureMeshCompute(ctx, "shaders/spline_comp.spv", sizeof(vec4) * 8, 1, 8)
	:VultureMeshCompute(ctx)
{
	Spline spl;

	spl.p1 = vec4(-0.7, 0.0, 0.0,  1.0);
	spl.p2 = vec4(0.7, 0.0, 0.0,  1.0);
	spl.ray1 = vec4(0.0, 0.6, 0.0, 1.0);
	spl.ray2 = vec4(0.0, 1.0, 0.0, 1.0);

	_skeletonBuffer = std::make_shared<VulkanBuffer>(_ctx, vk::BufferUsageFlagBits::eStorageBuffer, sizeof(Spline) * 1, VulkanBuffer::CPU_ALOT, &spl);

	_ssbo = std::make_shared<VulkanBuffer>(_ctx, vk::BufferUsageFlagBits::eStorageBuffer, sizeof(vec4) * 8, VulkanBuffer::CPU_NEVER);

	_uniformLayout = ctx->makeUniformSetLayout({
		ULB(1, vk::DescriptorType::eStorageBuffer)
	});

	_skeletonLayout = ctx->makeUniformSetLayout({
		ULB(1, vk::DescriptorType::eStorageBuffer)
	});

	_shader = ctx->makeComputeShader("shaders/spline_comp.spv", {
		_skeletonLayout,
		_uniformLayout
	});

	_pipeline = ctx->makeComputePipeline(_shader);

	_uniformSet = ctx->makeUniformSet(_uniformLayout);

	_uniformSet->bindBuffer(0, _ssbo->getDBI(), vk::DescriptorType::eStorageBuffer);

	_skeletonSet = ctx->makeUniformSet(_skeletonLayout);

	_skeletonSet->bindBuffer(0, _skeletonBuffer->getDBI(), vk::DescriptorType::eStorageBuffer);

}

VultureSplineCompute::~VultureSplineCompute()
{
}

void VultureSplineCompute::recordCompute(vk::CommandBuffer * cmd)
{

		_pipeline->bind(cmd);

		_pipeline->bindUniformSets(cmd, {
			_skeletonSet,
			_uniformSet
		});

		cmd->dispatch(1, 1, 1);

		return;

	
}
