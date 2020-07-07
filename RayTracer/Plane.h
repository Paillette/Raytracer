#pragma once
#include "Primitive.h"
#include "vec3.h"
#include "vec2.h"

class Plane : public Primitive
{
	vec3 normal;

public:
	//constructeur
	Plane();
	Plane(const vec3& p, vec3 normal, Material* mat);


	//return la distance d'intersection (t) si le discriminant est positif
	//sinon on retourne une valeur negative (quelconque)
	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs(vec3& p) const final;
	virtual ~Plane() {}
};
