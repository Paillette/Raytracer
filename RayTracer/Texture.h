#pragma once
#include "vec3.h"

class constant_texture
{
public:
	constant_texture() {}
	constant_texture(vec3 c) : color(c) {}

	virtual vec3 value(float u, float v, const vec3& p) const {
		return color;
	}

	vec3 color;
};