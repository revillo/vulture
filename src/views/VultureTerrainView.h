#pragma once
#include "views/VultureView.h"
#include "graphics/VultureMeshCompute.h"
#include "models/VultureSkeleton.h"

class TerrainView : public View {

public:

	class TerrainGen : public IComputable, public IMesh {
	public:
		struct TerrainDatum {
			vec4 height;
		};


		struct TerrainData {
			ivec2 size;
			TerrainDatum * data;

			TerrainDatum& getDatum(uint32 x, uint32 y) {
				return data[x + (y * size.x)];
			}
		};

		enum ComputeStage {
			eInit,
			eSimulate,
			eTransfer,
			ePopulateTrees
		};

		using TreeRig = Skeleton::Rig<20, 20>;

		TerrainGen(VultureGPUService * service, ivec2 textureSize = ivec2(1024, 1024));

		VulkanUniformSetRef getMeshSet() override {
			return _meshSet;
		}

		VulkanUniformSetLayoutRef getMeshSetLayout() override {
			return _meshSetLayout;
		}

		void draw(vk::CommandBuffer *cmd, uint32 numInstances) {
			
			cmd->draw(64 * 64 * 4, numInstances, 0, 0);
		
		};

		void setStage(ComputeStage stage) {
			_stage = stage;
		}

		void recordCompute(vk::CommandBuffer * cmd) override;

		TerrainData loadData() {
			return TerrainData {
				_textureSize,
				_dataBuffer->loadHeap()
			};
		}

		VulkanComputePipelineRef _initPipeline;
		VulkanComputePipelineRef _simPipeline;
		VulkanComputePipelineRef _transferPipeline;
		VulkanComputePipelineRef _treePipeline;

		VulkanImageRef _heightMap;
		//VulkanImageRef featureMap;
		//VulkanBufferRef _dataBuffer;

		VulkanBufferRef _treeSkeletonBuffer;

		shared_ptr<ssbo<TerrainDatum>> _dataBuffer;

		VulkanUniformSetLayoutRef _computeSetLayout;
		VulkanUniformSetRef _computeSet;

		ComputeStage _stage = eInit;

		ivec2 _textureSize;
		VulkanUniformSetLayoutRef _meshSetLayout;
		VulkanUniformSetRef _meshSet;

		uint32 maxTrees = 20000;
	};

	TerrainView(VultureGPUService * service);

	void show() override {
		_gpuService->addMeshRender(_meshRender);
	};

	void hide() override {
		//TODO;
	};

	shared_ptr<TerrainGen> terrainGen;

protected:

	MeshRenderRef _meshRender;

	MeshRenderRef _waterMeshRender;

	VultureGPUService * _gpuService;

	//shared_ptr<TerrainGen> terrainGen;

};

typedef shared_ptr<TerrainView> TerrainViewRef;