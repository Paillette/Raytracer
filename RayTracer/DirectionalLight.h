#pragma once
#include "Light.h"
#include "BRDFs.h"

class DirectionLight : public Light
{
	vec3 direction;

public:
	DirectionLight(vec3 dir, vec3 col, float i) :direction(dir), Light(vec3{ 0, 0, 0 }, col, i) {}

	virtual vec3 CalculateLighting(const vec3& normal, const ray& ray, float _Glossiness, vec3 Color, const vec3& pos = vec3()) const;

	virtual vec3 getDirection(const vec3& pos = vec3()) const { return direction; };
};