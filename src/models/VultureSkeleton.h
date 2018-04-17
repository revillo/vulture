#pragma once
#include "Vulcro.h"

class SkeletalMesh{
public:
	struct Vertex {
		vec4 position;
		vec4 normal;
	};
};

class Skeleton {

public:

	struct Joint {
		vec4 pos;
		vec4 quat;
	};

	struct Link {
		uint32 pi0;
		uint32 pi1;
		uint32 c0;
		uint32 c1;
	};

	template <uint32 numJoints, uint32 numLinks>
	struct Rig {
		uvec4 info;
		Joint joints[numJoints];
		Link links[numLinks];
	};

};