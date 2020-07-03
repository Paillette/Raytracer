#pragma once
#include "Primitive.h"
#include "vec3.h"

struct Sphere : public Primitive
{
	vec3 position;
	float radius;

	//constructeur
	//Sphere() {}
	Sphere(const vec3& p, float r, Material* mat, vec3 color) : position(p), radius(r), Primitive(mat, color) {}

	//return la distance d'intersection (t) si le discriminant est positif
	//sinon on retourne une valeur negative (quelconque)
	float intersect(const ray& ray) const
	{
		vec3 diff = ray.origin - position;
		float a = diff.dot(diff);
		vec3 v = ray.direction;
		float b = diff.dot(v.normalize());
		float c = a - radius * radius;
		float t = b * b - c;

		if (t >= 0.f)
		{
			t = -b - sqrtf(t);
		}
		return t;
	}
	virtual vec3 calculateNormal(const vec3& p) const final
	{
		return (p - position).normalize();
	}

	virtual ~Sphere() {}
};