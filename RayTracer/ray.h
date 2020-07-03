#pragma once
#include "vec3.h"

struct ray
{
	vec3 origin;
	float tmin;
	vec3 direction;
	float tmax;

	//calcul la distance d'un point d'intersection 
	//t : distance entre origin et point d'intersection
	inline vec3 evaluate(float t) const 
	{ 
		return origin + direction * t;
	}
};