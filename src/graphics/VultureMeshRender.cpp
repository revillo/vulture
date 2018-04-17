#include "VultureMeshRender.h"


MeshRender::MeshRender(VulkanPipelineRef pipeline, MeshRef mesh)
	: _pipeline(pipeline)
	, _mesh(mesh)
{

}

void MeshRender::recordDraw(vk::CommandBuffer * cmd, VulkanUniformSetRef sceneGlobalSet)
{
	_pipeline->bind(cmd);
	
	_pipeline->bindUniformSets(cmd, {
		sceneGlobalSet,
		_mesh->getMeshSet()
	});

	_mesh->draw(cmd, 1);
}

