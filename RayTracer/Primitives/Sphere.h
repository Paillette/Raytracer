#pragma once
#include "Primitive.h"
#include "..\Utilities\vec3.h"
#include "..\Material\Material.h"
#include "..\Utilities\ray.h"

class Sphere : public Primitive
{
	float radius;

public:

	Sphere();
	Sphere(const vec3& pos, float r, Material* mat);

	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs(vec3& p) const final;
	virtual ~Sphere() {}
};