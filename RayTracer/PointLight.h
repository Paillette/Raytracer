#pragma once
#include "Light.h"

class PointLight : public Light
{
	float radius;

public:
	PointLight(vec3 pos, float r, vec3 col, float i) :radius(r), Light(pos, col, i) {}

	virtual vec3 getDirection(const vec3& pos = vec3()) const { return pos - position; }
};