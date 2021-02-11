#pragma once
#include "Primitive.h"
#include "..\Utilities\vec3.h"
#include "..\Material\Material.h"
#include "..\Utilities\ray.h"

class Square : public Primitive
{
	float size;
	vec3 normal;

public:
	Square();
	Square(const vec3& pos, vec3 n, float s, Material* mat);

	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs(vec3& p) const final;
	virtual ~Square() {}
};