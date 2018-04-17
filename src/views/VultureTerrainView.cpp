#include "VultureTerrainView.h"

using TerrainGen = TerrainView::TerrainGen;

TerrainGen::TerrainGen(VultureGPUService * service, ivec2 textureSize) {
	auto ctx = service->getContext();
	_textureSize = textureSize;

	_heightMap = ctx->makeImage(VulkanImage::SAMPLED_STORAGE, textureSize, vk::Format::eR32Sfloat);
	_heightMap->allocateDeviceMemory();
	_heightMap->createImageView(vk::ImageAspectFlagBits::eColor);
	_heightMap->createSampler();


	//featureMap = ctx->makeImage(VulkanImage::SAMPLED_STORAGE, ivec2(32,32), vk::Format::eR8G8B8A8Uint);
	//featureMap->allocateDeviceMemory();
	//featureMap->createImageView(vk::ImageAspectFlagBits::eColor);
	//featureMap->createSampler();

	//_dataBuffer = ctx->makeBuffer(vk::BufferUsageFlagBits::eStorageBuffer, textureSize.x * textureSize.y * sizeof(TerrainDatum));

	_dataBuffer = ctx->makeSSBO<TerrainDatum>(textureSize.x * textureSize.y);


	_treeSkeletonBuffer = ctx->makeBuffer(vk::BufferUsageFlagBits::eStorageBuffer, 
		sizeof(uvec4) + sizeof(TreeRig) * maxTrees, VulkanBuffer::CPU_NEVER);


	_computeSetLayout = service->getContext()->makeUniformSetLayout({
		ULB(1, vk::DescriptorType::eStorageImage),
		ULB(1, vk::DescriptorType::eStorageBuffer),
		ULB(1, vk::DescriptorType::eStorageBuffer)
	});

	_computeSet = service->getContext()->makeUniformSet(_computeSetLayout);
	_computeSet->bindStorageImage(0, _heightMap);
	//set->bindStorageImage(1, featureMap);
	_computeSet->bindBuffer(1, _dataBuffer);
	_computeSet->bindBuffer(2, _treeSkeletonBuffer);

	_initPipeline = ctx->makeComputePipeline("shaders/terrain/terrain_init_comp.spv", { _computeSetLayout });
	_simPipeline = ctx->makeComputePipeline("shaders/terrain/terrain_simulate_comp.spv", { _computeSetLayout });
	_transferPipeline = ctx->makeComputePipeline("shaders/terrain/terrain_transfer_comp.spv", { _computeSetLayout });
	_treePipeline = ctx->makeComputePipeline("shaders/tree/populate_trees_comp.spv", { _computeSetLayout });

	//Mesh set


	_meshSetLayout = ctx->makeUniformSetLayout({
		ULB(1, vk::DescriptorType::eCombinedImageSampler, &_heightMap->getSampler())
	});

	_meshSet = ctx->makeUniformSet(_meshSetLayout);

	_meshSet->bindImage(0, _heightMap);
}

void TerrainGen::recordCompute(vk::CommandBuffer * cmd)
{
	auto * pipeline = &_initPipeline;

	switch (_stage) {
		case eInit: pipeline = &_initPipeline; break;
		case eSimulate: pipeline = &_simPipeline; break;
		case eTransfer: pipeline = &_transferPipeline; break;
		case ePopulateTrees: pipeline = &_treePipeline; break;
	};

	(*pipeline)->bind(cmd);
	(*pipeline)->bindUniformSets(cmd, { _computeSet });

	cmd->dispatch(_textureSize.x / 32, _textureSize.y / 32, 1);
}


TerrainView::TerrainView(VultureGPUService * service)
	:_gpuService(service)
{
	terrainGen = make_shared<TerrainGen>(service);
	terrainGen->setStage(TerrainGen::ComputeStage::eInit);
	service->computeOnce(terrainGen);
	terrainGen->setStage(TerrainGen::ComputeStage::eSimulate);
	service->computeNTimes(terrainGen, 16);
	//terrainGen->setStage(TerrainGen::ComputeStage::eTransfer);
	//service->computeOnce(terrainGen);
	//auto tdata = terrainGen->loadData();
	terrainGen->setStage(TerrainGen::ComputeStage::ePopulateTrees);
	service->computeOnce(terrainGen);

	
	//auto *p = (uint32*)terrainGen->_treeSkeletonBuffer->getMapped();
	//auto count = *p;

	/*
	auto * rigs = (TerrainGen::TreeRig *)(p + 4);

	for (int i = 0; i < 100; i++) {

		auto v = p[i];

		auto r = rigs[i];
		i = i;
	}
	*/


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
					terrainGen->getMeshSetLayout()
				}),

			_gpuService->getSceneRenderer(),
			{
				vk::PrimitiveTopology::ePatchList,
				4
			}
		),
		terrainGen
	);

	/*
	_waterMeshRender = MeshRender::make(

		ctx->makePipeline(

			ctx->makeTessShader(
				"shaders/sea/sea_vert.spv",
				"shaders/sea/sea_tesc.spv",
				"shaders/sea/sea_tese.spv",
				nullptr,
				"shaders/sea/sea_frag.spv",
				{
				}, {
					_gpuService->getSceneGlobalsLayout()
				}),

			_gpuService->getSceneRenderer(),
			{
				vk::PrimitiveTopology::ePatchList,
				4
			}
		),
		BufferlessMesh::make(64 * 64 * 4)
	);
	*/


}
