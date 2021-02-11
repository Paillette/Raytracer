#pragma once

#include "Primitive.h"
#include "..\Utilities\vec3.h"

class InfiniteCylinder : public Primitive
{
	float radius;

public:
	InfiniteCylinder();
	InfiniteCylinder(const vec3& pos, float r, Material* mat);

	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs(vec3& p) const final;
	virtual ~InfiniteCylinder() {}
};