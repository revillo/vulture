#pragma once

#include "models\VulturePrimitives.h"

class VulturePlanet {

	struct PlanetVertex {
		vec4 position;
	};

	VulturePlanet();

	~VulturePlanet();


protected:

	Primitives::Icosahedron _ico;


};