#pragma once
#include "Primitive.h"
#include "..\Utilities\vec3.h"

class Cone : public Primitive
{
	float radius;
	float height;

public:
	Cone();
	Cone(const vec3& pos, float r, float h, Material* mat);

	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs(vec3& p) const final;
	virtual ~Cone() {}
};