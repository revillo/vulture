#include "VulturePrimitives.h"

Primitives::Icosahedron::Icosahedron(uint32 subdiv)
{
	indices = {
		1, 2, 6,
		1, 7, 2,
		3, 4, 5,
		4, 3, 8,
		6, 5, 11,
		5, 6, 10,
		9, 10, 2,
		10, 9, 3,
		7, 8, 9,
		8, 7, 0,
		11, 0, 1,
		0, 11, 4,
		6, 2, 10,
		1, 6, 11,
		3, 5, 10,
		5, 4, 11,
		2, 7, 9,
		7, 1, 0,
		3, 9, 8,
		4, 8, 0
	};

	vertices = {
		vec4(0, -0.525731, 0.850651, 1.0),
		vec4(0.850651, 0, 0.525731, 1.0),
		vec4(0.850651, 0, -0.525731,  1.0),
		vec4(-0.850651, 0, -0.525731, 1.0),
		vec4(-0.850651, 0, 0.525731, 1.0),
		vec4(-0.525731, 0.850651, 0, 1.0),
		vec4(0.525731, 0.850651, 0, 1.0),
		vec4(0.525731, -0.850651, 0,  1.0),
		vec4(-0.525731, -0.850651, 0,  1.0),
		vec4(0, -0.525731, -0.850651, 1.0),
		vec4(0, 0.525731, -0.850651, 1.0),
		vec4(0, 0.525731, 0.850651, 1.0)
	};

}

Primitives::Grid::Grid(uint32 xdiv, uint32 ydiv)
{
	vertices.reserve((xdiv + 1) * (ydiv + 1));

	auto addFace = [&](uint16 a, uint16 b, uint16 c, uint16 d) {
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
		indices.push_back(d);
	};

	for (uint32 x = 0; x < xdiv + 1; x++) {
		for (uint32 y = 0; y < ydiv + 1; y++) {

			vertices.emplace_back((float)x / xdiv, (float)y / ydiv, 0.0, 1.0);
			
			if (x < xdiv && y < ydiv) {
				uint i = static_cast<uint16>(vertices.size()) - 1;
				addFace(i, i + 1, i + xdiv + 2, i + xdiv);
			}
		}
	}

}
