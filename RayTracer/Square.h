#pragma once
#include "Primitive.h"

class Square : public Primitive
{
	float size;
	vec3 normal;

public:
	//constructeur
	Square();
	Square(const vec3& pos, vec3 n, float s, Material* mat);

	//return la distance d'intersection (t) si le discriminant est positif
	//sinon on retourne une valeur negative (quelconque)
	float intersect(const ray& ray) const;
	virtual vec3 calculateNormal(const vec3& p) const final;
	virtual vec3 calculateUVs(const vec3& p) const final;
	virtual ~Square() {}
};