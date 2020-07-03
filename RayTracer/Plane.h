#pragma once
#include "Primitive.h"
#include "vec3.h"

struct Plane : public Primitive
{
public:
	//constructeur
	Plane();
	Plane(const vec3& p, Material* mat, vec3 color) : Primitive(p, mat, color) {}


	//return la distance d'intersection (t) si le discriminant est positif
	//sinon on retourne une valeur negative (quelconque)
	float intersect(const ray& ray) const
	{
		vec3 normal = vec3{ 0.f, 1.f, 0.f };
		float denom = normal.dot(ray.direction);
		float t = 0.f;

		if (abs(denom) >= 0.0001f) 
		{
			t = (position - ray.origin).dot(normal) / denom;
			if (t >=  0.0001f) return t; 
		}
		return t;
	}
	virtual vec3 calculateNormal(const vec3& p) const final
	{
		return vec3{ 0.f, 1.f, 0.f };
	}

	virtual ~Plane() {}
};
