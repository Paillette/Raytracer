#pragma once
#include "Light.h"

class DirectionLight : public Light
{
	vec3 direction;

public:
	DirectionLight(vec3 dir, vec3 col, float i) :direction(dir), Light(vec3{ 0, 0, 0 }, col, i) {}

	vec3 getDirection() { return direction; }
};