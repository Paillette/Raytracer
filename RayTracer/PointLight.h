#pragma once
#include "Light.h"

class PointLight : public Light
{
	float radius;

public:
	PointLight(float r, vec3 col, float i) :radius(r), Light(vec3{ 0, 0, 0 }, col, i) {}

	virtual vec3 getDirection() const { return vec3(); }
};