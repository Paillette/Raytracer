#pragma once
#include "Primitive.h"
#include "vec3.h"

class Tri : public Primitive
{
	float radius;

public:
	//constructeur
	Tri(const vec3& pos, float r, Material* mat, vec3 color);

	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(const vec3& p) const final;

	virtual ~Tri() {}
};