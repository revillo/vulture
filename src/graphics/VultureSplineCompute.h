#pragma once

#include "VultureMeshCompute.h"

class VultureSplineCompute : public VultureMeshCompute {


public:
	struct Spline {
		vec4 p1;
		vec4 p2;
		vec4 ray1;
		vec4 ray2;
	};

	VultureSplineCompute(VulkanContextRef ctx);

	~VultureSplineCompute();

	uint32 getNumVerts() {
		return 24;
	}

	void recordCompute(vk::CommandBuffer * cmd);
	
protected:

	VulkanBufferRef _skeletonBuffer;
	VulkanUniformSetLayoutRef _skeletonLayout;
	VulkanUniformSetRef _skeletonSet;
};