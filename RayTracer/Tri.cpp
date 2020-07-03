#include "Tri.h"

Tri::Tri(const vec3& pos, float r, Material* mat, vec3 color)
	: radius(r), Primitive::Primitive(pos, mat, color)
{
}

float Tri::intersect(const ray& ray) const
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

vec3 Tri::calculateNormal(const vec3& p) const
{
	return (p - position).normalize();
}
