#pragma once
#include "models/VulturePrimitives.h"
#include "views/VultureView.h"

class PlanetView : public View {

	struct PlanetVertex {
		vec4 position;
	};

public:

	PlanetView(VultureGPUService * service);

	void show() override {
		_gpuService->addMeshRender(_meshRender);
	}

	void hide() override {
		//TODO;
	}

	~PlanetView();

protected:

	//model todo

	VultureGPUService * _gpuService;

	Primitives::Icosahedron _ico;

	ClassicMeshRef _mesh;

	MeshRenderRef _meshRender;
};

typedef shared_ptr<PlanetView> PlanetViewRef;