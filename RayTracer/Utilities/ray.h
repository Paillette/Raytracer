#pragma once
#include "vec3.h"

struct ray
{
	vec3 origin;
	float tmin;
	vec3 direction;
	float tmax;

	ray() :origin(vec3()), direction(vec3()), tmin(0), tmax(999) {}
	ray(vec3 o, vec3 dir, float tmin = 0, float tmax = 999):origin(o), direction(dir), tmin(tmin), tmax(tmax) {}

	//compute distance from intersection point
	//t : distance between origin and intersection point
	inline vec3 evaluate(float t) const 
	{ 
		return origin + direction * t;
	}
};