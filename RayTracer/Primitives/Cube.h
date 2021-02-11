#pragma once
#include "Primitive.h"
#include "..\Utilities\vec3.h"

class Cube : public Primitive
{
	float size;

public:
	//constructeur
	Cube();
	Cube(const vec3& pos, float s, Material* mat);

	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs (vec3& p) const final;
	virtual ~Cube() {}
};