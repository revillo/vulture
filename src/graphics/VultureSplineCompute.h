#pragma once

#include "VultureMeshCompute.h"

template <class Skeleton>
class VultureSplineCompute : public ComputeMesh {


public:

	static shared_ptr<VultureSplineCompute<Skeleton>> make(VulkanContextRef ctx, uint32 numSkeletons, uint32 maxSplines, Skeleton * data = nullptr)
	{
		return make_shared<VultureSplineCompute<Skeleton>>(ctx, numSkeletons, maxSplines, data);
	}

	VultureSplineCompute(VulkanContextRef ctx, uint32 numSkeletons, uint32 maxSplines, Skeleton * data = nullptr)
		:ComputeMesh(ctx),
		_numSkeletons(numSkeletons),
		_maxSplines(maxSplines)
	{
		struct Vertex {
			vec4 position;
			vec4 normal;
		};

		_skeletonBuffer = std::make_shared<VulkanBuffer>(_ctx, vk::BufferUsageFlagBits::eStorageBuffer, sizeof(Skeleton) * numSkeletons, VulkanBuffer::CPU_ALOT, data);

		_ssbo = std::make_shared<VulkanBuffer>(_ctx, vk::BufferUsageFlagBits::eStorageBuffer, sizeof(Vertex) * 8 * maxSplines * numSkeletons, VulkanBuffer::CPU_NEVER);

		_uniformLayout = ctx->makeUniformSetLayout({
			ULB(1, vk::DescriptorType::eStorageBuffer)
			});

		_skeletonLayout = ctx->makeUniformSetLayout({
			ULB(1, vk::DescriptorType::eStorageBuffer)
			});

		_shader = ctx->makeComputeShader("shaders/spline/spline_comp.spv", {
			_skeletonLayout,
			_uniformLayout
			});

		_pipeline = ctx->makeComputePipeline(_shader);

		_uniformSet = ctx->makeUniformSet(_uniformLayout);

		_uniformSet->bindBuffer(0, _ssbo->getDBI(), vk::DescriptorType::eStorageBuffer);

		_skeletonSet = ctx->makeUniformSet(_skeletonLayout);

		_skeletonSet->bindBuffer(0, _skeletonBuffer->getDBI(), vk::DescriptorType::eStorageBuffer);
	};


	uint32 getNumVerts() {
		return 16 *_maxSplines * _numSkeletons;
	}


	void recordCompute(vk::CommandBuffer * cmd) override {

		_pipeline->bind(cmd);

		_pipeline->bindUniformSets(cmd, {
			_skeletonSet,
			_uniformSet
		});

		cmd->dispatch(_numSkeletons, _maxSplines, 1);

		return;


	}
	
protected:

	uint32 _numSkeletons, _maxSplines;

	VulkanBufferRef _skeletonBuffer;
	VulkanUniformSetLayoutRef _skeletonLayout;
	VulkanUniformSetRef _skeletonSet;
};