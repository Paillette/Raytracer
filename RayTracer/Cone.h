#pragma once
#include "Primitive.h"
#include "vec3.h"

class Cone : public Primitive
{
	float radius;
	float height;

public:
	//constructeur
	Cone();
	Cone(const vec3& pos, float r, float h, Material* mat);

	//return la distance d'intersection (t) si le discriminant est positif
	//sinon on retourne une valeur negative (quelconque)
	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(vec3& p) const final;
	virtual vec3 calculateUVs(vec3& p) const final;
	virtual ~Cone() {}
};