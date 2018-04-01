#include "TestApp.h"

vec3 Slerp(vec3 r1, vec3 r2, float t)
{
	vec3 n1 = normalize(r1);
	vec3 n2 = normalize(r2);

	float dotp = dot(n1, n2);

	if (dotp > 0.9999)
	{
		return mix(r1, r2, t);
	}

	float theta = acos(dotp);
	vec3 P = ((n1 * sinf((1.0 - t)*theta) + n2 * sinf(t*theta)) / sinf(theta));
	
	return normalize(P) * length(n1);
}
vec3 rotationAxisAngle(vec3 v, vec3 axis, float angle)
{
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;

	return mat3(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c 
		) * v;
}



void printv3(vec3 v3) {
	std::cout << std::endl;
	std::cout << v3.x << ", " << v3.y << ", " << v3.z;
}

int main() {



	//printv3(Slerp(vec3(-0.5, 0, 1.0), vec3(0.5, 0.0, 1.0), 0.3));

	auto app = TestSplineApp();
	
	app.run();

	int i;
	cin >> i;


	return 0;
}