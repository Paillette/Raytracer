#pragma once
#include "ray.h"
#include "Material.h"

struct Primitive
{
	vec3 position;
	const Material* material;
	vec3 color;
	
	Primitive() {}
	Primitive(vec3 pos, const Material* mat, vec3 col) : position(pos), material(mat), color(col){}

	virtual float intersect(const ray& ray) const = 0;
	virtual vec3 calculateNormal(const vec3& p) const = 0;
	virtual ~Primitive() {}
};