#pragma once
#include "Vulcro.h"

class VultureSkeleton {

public:
	struct Spline {
		vec4 p1;
		vec4 p2;
		vec4 ray1;
		vec4 ray2;
	};

	VultureSkeleton();

	~VultureSkeleton();

protected:

	vector<Spline> _splines;


};