#pragma once
#include "Primitive.h"
#include "vec3.h"
#include "vec2.h"

class Plane : public Primitive
{
public:
	//constructeur
	Plane();
	Plane(const vec3& p, Material* mat);


	//return la distance d'intersection (t) si le discriminant est positif
	//sinon on retourne une valeur negative (quelconque)
	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(const vec3& p) const final;
	virtual vec2 calculateUVs(const vec3& p) const final;
	virtual ~Plane() {}
};
