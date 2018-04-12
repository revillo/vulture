#pragma once

#include "Vulcro.h"

namespace Primitives {

	class IPrimitive {

	public:
		vector<uint16> indices;
		vector<vec4> vertices;
	};
	
	class Icosahedron : public IPrimitive {
	public:

		Icosahedron(uint32 subdiv);

	};

	class Grid : public IPrimitive {

	public:
		Grid(uint32 xdiv, uint32 ydiv);
	};

};