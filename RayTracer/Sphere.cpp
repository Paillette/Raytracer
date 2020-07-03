#include "Sphere.h"

Sphere::Sphere()
:radius(1.0f), Primitive(vec3{ 0, 0, 0 }, new Material(Material::Type::MATTE, 0.f, 0.f), vec3{ 1.0f, 1.0f, 1.0f })
{
}

Sphere::Sphere(float r, Material* mat, vec3 col, const vec3& pos)
	:radius(r), Primitive::Primitive(pos, mat, col)
{
}

float Sphere::intersect(const ray& ray) const
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

vec3 Sphere::calculateNormal(const vec3& p) const
{
	return (p - position).normalize();
}
