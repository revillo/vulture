#include "VultureMeshRender.h"

VultureMeshRender::VultureMeshRender(VulkanPipelineRef pipeline, VultureMeshComputeRef meshCompute)
	: _pipeline(pipeline)
	, _meshCompute(meshCompute)
{

}

void VultureMeshRender::recordDraw(vk::CommandBuffer * cmd)
{
	_pipeline->bind(cmd);
	
	_pipeline->bindUniformSets(cmd, {
		_meshCompute->getUniformSet()
	});

	//cmd->drawIndexed(_meshCompute->getNumVerts(), 1, 0, 0, 0);
	cmd->draw(_meshCompute->getNumVerts(), 1, 0, 0);
}

