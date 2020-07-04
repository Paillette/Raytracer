#pragma once
#include "Primitive.h"
#include "vec3.h"

class Sphere : public Primitive
{
	float radius;

public:
	//constructeur
	Sphere();
	Sphere(const vec3& pos, float r, Material* mat, vec3 col);

	//return la distance d'intersection (t) si le discriminant est positif
	//sinon on retourne une valeur negative (quelconque)
	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(const vec3& p) const final;
	virtual vec2 calculateUVs(const vec3& p) const final;
	virtual ~Sphere() {}
};