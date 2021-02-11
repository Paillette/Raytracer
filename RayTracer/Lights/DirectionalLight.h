#pragma once
#include "Light.h"

class DirectionLight : public Light
{
	vec3 direction;

public:
	DirectionLight(vec3 dir, vec3 col, float i);

	virtual vec3 CalculateLighting(const vec3& normal, const ray& ray, float _Glossiness, vec3 Color, const vec3& pos) const;

	virtual vec3 getDirection(const vec3& pos = vec3()) const { return direction; };
};