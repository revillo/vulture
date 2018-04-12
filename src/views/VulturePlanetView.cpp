#include "VulturePlanetView.h"

PlanetView::PlanetView(VultureGPUService * service)
	:
	_gpuService(service),
	_ico(0)
{

	VulkanContextRef ctx = _gpuService->getContext();
	
	_mesh = make_shared<ClassicMesh>();



	vboRef vbuf = ctx->makeVBO<PlanetVertex>({
		vk::Format::eR32G32B32A32Sfloat
		}, _ico.vertices.size(), (PlanetVertex*)&_ico.vertices[0]);

	_mesh->setBuffers(

		vbuf,
		ctx->makeIBO(move(_ico.indices))

	);

	_meshRender = MeshRender::make(

		ctx->makePipeline(

			ctx->makeTessShader(
				"shaders/tess/tess_vert.spv",
				"shaders/tess/tess_tesc.spv",
				"shaders/tess/tess_tese.spv",
				"shaders/tess/tess_geom.spv",
				"shaders/tess/tess_frag.spv",
		 {
			vbuf->getLayout()
		 }, {
			 _gpuService->getSceneGlobalsLayout()
		 }),

			_gpuService->getSceneRenderer(),
			{
				vk::PrimitiveTopology::ePatchList,
				3
			}
		),

		_mesh

	);
}


PlanetView::~PlanetView()
{
}
