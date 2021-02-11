#pragma once
#include "Primitive.h"
#include "..\Utilities\vec3.h"
#include "..\Material\Material.h"
#include "..\Utilities\ray.h"
#include "..\Utilities\vec2.h"

class Plane : public Primitive
{
	vec3 normal;

public:

	Plane();
	Plane(const vec3& p, vec3 normal, Material* mat);

	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs(vec3& p) const final;
	virtual ~Plane() {}
};
