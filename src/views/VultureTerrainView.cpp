#include "VultureTerrainView.h"

TerrainView::TerrainView(VultureGPUService * service)
	:_gpuService(service)
{

	terrainGen = make_shared<TerrainGen>(service);
	service->computeOnce(terrainGen);
	terrainGen->stage = 1;
	service->computeNTimes(terrainGen, 24);

	VulkanContextRef ctx = _gpuService->getContext();

	

	_meshRender = MeshRender::make(

		ctx->makePipeline(

			ctx->makeTessShader(
				"shaders/terrain/terrain_vert.spv",
				"shaders/terrain/terrain_tesc.spv",
				"shaders/terrain/terrain_tese.spv",
				"shaders/terrain/terrain_geom.spv",
				"shaders/terrain/terrain_frag.spv",
				{
				}, {
					_gpuService->getSceneGlobalsLayout(),
					terrainGen->sampledSetLayout
				}),

			_gpuService->getSceneRenderer(),
			{
				vk::PrimitiveTopology::ePatchList,
				4
			}
		),
		terrainGen
	);



}
