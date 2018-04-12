#pragma once
#include "views/VultureView.h"
#include "graphics/VultureMeshCompute.h"

class TerrainView : public View {

public:

	class TerrainGen : public IComputable, public IMesh {
	public:

		TerrainGen(VultureGPUService * service)
		{
			auto ctx = service->getContext();

			heightMap = ctx->makeImage(vk::ImageUsageFlagBits::eStorage | vk::ImageUsageFlagBits::eSampled, ivec2(64, 64), vk::Format::eR32Sfloat);
			heightMap->allocateDeviceMemory();
			heightMap->createImageView(vk::ImageAspectFlagBits::eColor);
			heightMap->createSampler();


			setLayout = service->getContext()->makeUniformSetLayout({
				ULB(1, vk::DescriptorType::eStorageImage)
				});

			set = service->getContext()->makeUniformSet(setLayout);
			set->bindStorageImage(0, heightMap);

			_pipeline = service->getContext()->makeComputePipeline(
				service->getContext()->makeComputeShader(
					"shaders/terrain/terrain_comp.spv",
					{
						setLayout
					}
				)
			);

			sampledSetLayout = ctx->makeUniformSetLayout({
				ULB(1, vk::DescriptorType::eCombinedImageSampler, &heightMap->getSampler())
			});

			sampledSet = ctx->makeUniformSet(sampledSetLayout);

			sampledSet->bindImage(0, heightMap);
		};

		VulkanUniformSetRef getUniformSet() override {
			return sampledSet;
		}

		void draw(vk::CommandBuffer *cmd, uint32 numInstances) {
			
			cmd->draw(64 * 64 * 4, numInstances, 0, 0);
		
		};


		void recordCompute(vk::CommandBuffer * cmd) override {
			_pipeline->bind(cmd);
			_pipeline->bindUniformSets(cmd, { set });
			cmd->dispatch(64, 64, 1);
		}

		VulkanComputePipelineRef _pipeline;
		VulkanImageRef heightMap;
		VulkanUniformSetLayoutRef setLayout;
		VulkanUniformSetRef set;


		VulkanUniformSetLayoutRef sampledSetLayout;
		VulkanUniformSetRef sampledSet;

	};

	TerrainView(VultureGPUService * service);

	void show() override {
		_gpuService->addMeshRender(_meshRender);
	};

	void hide() override {
		//TODO;
	};

protected:

	BufferlessMeshRef _mesh;

	MeshRenderRef _meshRender;
	VultureGPUService * _gpuService;

	shared_ptr<TerrainGen> terrainGen;



};

typedef shared_ptr<TerrainView> TerrainViewRef;