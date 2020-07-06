#pragma once
#include "Primitive.h"
#include "vec3.h"

class Tri : public Primitive
{
private:
	float radius;

public:
	//constructeur
	Tri(float r, const vec3& pos, Material* mat);

	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(const vec3& p) const;
	virtual vec3 calculateUVs(const vec3& p) const;
	virtual ~Tri() {}
};