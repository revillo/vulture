#pragma once
#include "views/VultureView.h"
#include "graphics/VultureMeshCompute.h"
#include "models/VultureSkeleton.h"

class AgentView : public View {
public:

	class AgentGen : public IMesh, public IComputable {

	public:
		AgentGen(VultureGPUService * service, shared_ptr<TerrainView::TerrainGen> terrain);

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
		uint32 _maxAgents;
		VulkanComputePipelineRef _meshPipeline;
		VulkanUniformSetLayoutRef _meshSetLayout;
		VulkanUniformSetRef _meshSet;

		VulkanUniformSetLayoutRef _rigSetLayout;
		VulkanUniformSetRef _rigSet;

		VulkanBufferRef _meshBuffer;
	};

	AgentView(VultureGPUService * service, shared_ptr<TerrainView::TerrainGen> terrain);


	// Inherited via View
	virtual void show() override;
	virtual void hide() override;

protected:

	shared_ptr<AgentGen> _agentGen;
	VultureGPUService * _gpuService;
	MeshRenderRef _meshRender;
};

typedef shared_ptr<AgentView> AgentViewRef;