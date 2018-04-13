#pragma once
#include "views/VultureView.h"
#include "graphics/VultureMeshCompute.h"


class TerrainView : public View {

public:

	class TerrainGen : public IComputable, public IMesh {
	public:

		TerrainGen(VultureGPUService * service, ivec2 textureSize = ivec2(1024, 1024))
		{
			auto ctx = service->getContext();
			_textureSize = textureSize;
			heightMap = ctx->makeImage(vk::ImageUsageFlagBits::eStorage | vk::ImageUsageFlagBits::eSampled, textureSize, vk::Format::eR32Sfloat);
			heightMap->allocateDeviceMemory();
			heightMap->createImageView(vk::ImageAspectFlagBits::eColor);
			heightMap->createSampler();


			setLayout = service->getContext()->makeUniformSetLayout({
				ULB(1, vk::DescriptorType::eStorageImage)
				});

			set = service->getContext()->makeUniformSet(setLayout);
			set->bindStorageImage(0, heightMap);

			_initPipeline = ctx->makeComputePipeline(
				ctx->makeComputeShader(
					"shaders/terrain/terrain_init_comp.spv",
					{
						setLayout
					}
				)
			);

			_simPipeline = ctx->makeComputePipeline(
				ctx->makeComputeShader(
					"shaders/terrain/terrain_simulate_comp.spv",
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
			
			if (stage == 0) {
				_initPipeline->bind(cmd);
				_initPipeline->bindUniformSets(cmd, { set });
			}
			else if (stage == 1) {
				_simPipeline->bind(cmd);
				_simPipeline->bindUniformSets(cmd, { set });
			}
			else {
				return;
			}

			cmd->dispatch(_textureSize.x / 32, _textureSize.y / 32, 1);
		}

		VulkanComputePipelineRef _initPipeline;
		VulkanComputePipelineRef _simPipeline;
		VulkanImageRef heightMap;
		VulkanUniformSetLayoutRef setLayout;
		VulkanUniformSetRef set;

		uint32 stage = 0;

		ivec2 _textureSize;
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