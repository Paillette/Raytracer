#include "Plane.h"

Plane::Plane()
:Primitive(vec3{ 0, 0, 0 },
	new Material(Material::Type::MATTE, 0.f, 0.f),
	vec3{ 1.0f, 1.0f, 1.0f })
{
}

Plane::Plane(const vec3& p, Material* mat, vec3 color)
	: Primitive::Primitive(p, mat, color)
{
}

float Plane::intersect(const ray& ray) const
{
	vec3 normal = vec3{ 0.f, 1.f, 0.f };
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
	return vec3{ 0.f, 1.f, 0.f };
}

vec2 Plane::calculateUVs(const vec3& p) const
{
	float u = (p.x - floor(p.x));
	float v = (p.y - floor(p.y));

	return vec2(u, v);
}


