#pragma once

#include "Primitive.h"
#include "vec3.h"

class InfiniteCylinder : public Primitive
{
	float radius;

public:
	//constructeur
	InfiniteCylinder();
	InfiniteCylinder(const vec3& pos, float r, Material* mat);

	//return la distance d'intersection (t) si le discriminant est positif
	//sinon on retourne une valeur negative (quelconque)
	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs(vec3& p) const final;
	virtual ~InfiniteCylinder() {}
};