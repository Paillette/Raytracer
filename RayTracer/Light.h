#pragma once
#include "vec3.h"

class Light
{
	vec3 position;
	vec3 color;
	float intensity;

public:
	Light(vec3 pos, vec3 col, float i) : position(pos), color(col), intensity(i) {}

	vec3 getPos() const { return position; }
	vec3 getColor() const { return color; }
};