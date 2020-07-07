#include "Plane.h"

Plane::Plane()
:normal(vec3{ 0, 0, -1.f }), Primitive(vec3{ 0, 0, 0 }, new Material("mat", Material::Type::MATTE, vec3{ 1, 1, 1 }, 0.f, 0.f))
{
}

Plane::Plane(const vec3& p, vec3 n, Material* mat)
	:normal(n), Primitive::Primitive(p, mat)
{
}

float Plane::intersect(const ray& ray) const
{
	float denom = normal.dot(ray.direction);
	float t = 0.f;

	if (abs(denom) >= 0.0001f)
	{
		t = (position - ray.origin).dot(normal) / denom;
		if (t >= 0.0001f) return t;
	}
	return t;
}

vec3 Plane::calculateNormal(const vec3& p) const
{
	return normal;
}

vec3 Plane::calculateUVs(const vec3& p) const
{
	float u = (p.x - floor(p.x));
	float v = (p.y - floor(p.y));
	float S = 1.f;

	return vec3{ u, v, S };
}


