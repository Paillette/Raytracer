#pragma once
#include "vec3.h"
#include "Entity.h"

class Light : Entity
{
	vec3 color;
	float intensity;

public:
	Light(vec3 pos, vec3 col, float i): Entity(pos), color(col), intensity(i) {}

	vec3 getColor() const { return color; }
};