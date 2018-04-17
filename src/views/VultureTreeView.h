#pragma once
#include "VultureView.h"
#include "VultureTerrainView.h"


class TreeView : public View {

public:

	class TreeGen : public IMesh, public IComputable {

	public:
		TreeGen(VultureGPUService * service, shared_ptr<TerrainView::TerrainGen> terrain);
		
		// Inherited via IMesh
		virtual void draw(vk::CommandBuffer * cmd, uint32 numInstances) override;
		virtual VulkanUniformSetRef getMeshSet() override {
			return _meshSet;
		}

		virtual VulkanUniformSetLayoutRef getMeshSetLayout() override {
			return _meshSetLayout;
		}

		// Inherited via IComputable
		virtual void recordCompute(vk::CommandBuffer * cmd) override;

	protected:
		uint32 _maxTrees;
		VulkanComputePipelineRef _meshPipeline;
		VulkanUniformSetLayoutRef _meshSetLayout;
		VulkanUniformSetRef _meshSet;

		VulkanUniformSetLayoutRef _rigSetLayout;
		VulkanUniformSetRef _rigSet;

		VulkanBufferRef _meshBuffer;
	};

	TreeView(VultureGPUService * service, shared_ptr<TerrainView::TerrainGen> terrain);


	// Inherited via View
	virtual void show() override;
	virtual void hide() override;

protected:

	shared_ptr<TreeGen> _treeGen;
	VultureGPUService * _gpuService;
	MeshRenderRef _meshRender;
};

typedef shared_ptr<TreeView> TreeViewRef;