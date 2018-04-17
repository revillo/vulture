#include "VultureTreeView.h"

using TreeGen = TreeView::TreeGen;

TreeGen::TreeGen(VultureGPUService * service, shared_ptr<TerrainView::TerrainGen> terrain)
{
	auto ctx = service->getContext();

	_rigSetLayout = ctx->makeUniformSetLayout({
		ULB(1, vk::DescriptorType::eStorageBuffer)
		});

	_meshSetLayout = ctx->makeUniformSetLayout({
		ULB(1, vk::DescriptorType::eStorageBuffer)
	});

	_maxTrees = terrain->maxTrees;

	_meshBuffer = ctx->makeLocalStorageBuffer(terrain->maxTrees * 20 * 8 * sizeof(SkeletalMesh::Vertex));

	_meshPipeline = ctx->makeComputePipeline("shaders/tree/make_meshes_comp.spv", {
		_rigSetLayout,
		_meshSetLayout
	});

	_rigSet = ctx->makeUniformSet(_rigSetLayout);
	_meshSet = ctx->makeUniformSet(_meshSetLayout);

	_rigSet->bindBuffer(0, terrain->_treeSkeletonBuffer);
	_meshSet->bindBuffer(0, _meshBuffer);
}

void TreeView::TreeGen::draw(vk::CommandBuffer * cmd, uint32 numInstances)
{
	cmd->draw(16 * 20 * _maxTrees, numInstances, 0, 0);
}

void TreeView::TreeGen::recordCompute(vk::CommandBuffer * cmd)
{
	_meshPipeline->bind(cmd);
	_meshPipeline->bindUniformSets(cmd, {
		_rigSet,
		_meshSet
	});

	cmd->dispatch(_maxTrees, 20, 1);
}

TreeView::TreeView(VultureGPUService * service, shared_ptr<TerrainView::TerrainGen> terrain)
	:_gpuService(service)
{
	_treeGen = make_shared<TreeGen>(service, terrain);
	service->computeOnce(_treeGen);
	auto ctx = service->getContext();

	_meshRender = MeshRender::make(

		ctx->makePipeline(

			ctx->makeTessShader(
				"shaders/spline/spline_vert.spv",
				"shaders/spline/spline_tesc.spv",
				"shaders/spline/spline_tese.spv",
				"shaders/spline/spline_geom.spv",
				"shaders/spline/spline_frag.spv",
				{
				}, {
					service->getSceneGlobalsLayout(),
					_treeGen->getMeshSetLayout()
				}),

			service->getSceneRenderer(),
			{
				vk::PrimitiveTopology::ePatchList,
				4
			}
		),
		_treeGen
	);


}

void TreeView::show()
{
	_gpuService->addMeshRender(_meshRender);

}

void TreeView::hide()
{
}
