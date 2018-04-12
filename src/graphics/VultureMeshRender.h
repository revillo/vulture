#pragma once
#include "Vulcro.h"
#include "graphics/VultureMesh.h"

class IRenderable : public OOP::Interface {
	
public:

	virtual void recordDraw(vk::CommandBuffer * cmd, VulkanUniformSetRef sceneGlobalSet) = 0;

};

typedef shared_ptr<IRenderable> RenderableRef;


class MeshRender : public IRenderable {
public:

	static shared_ptr<MeshRender> make(VulkanPipelineRef pipeline, MeshRef mesh) {
		return make_shared<MeshRender>(pipeline, mesh);
	}

	MeshRender(VulkanPipelineRef pipeline, MeshRef mesh);

	virtual void recordDraw(vk::CommandBuffer * cmd, VulkanUniformSetRef sceneGlobalSet) override;

protected:
	MeshRef _mesh;
	VulkanPipelineRef _pipeline;
};

typedef shared_ptr<MeshRender> MeshRenderRef;
