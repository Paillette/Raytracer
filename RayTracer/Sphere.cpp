#include "Sphere.h"
#define M_PI 3.14159265358979323846

Sphere::Sphere()
:radius(1.0f), Primitive(vec3{ 0, 0, 0 }, new Material(Material::Type::MATTE, 0.f, 0.f), vec3{ 1.0f, 1.0f, 1.0f })
{
}

Sphere::Sphere(const vec3& pos, float r, Material* mat, vec3 col)
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

vec2 Sphere::calculateUVs(const vec3& p) const
{
	float phi = atan2(p.z, p.x);
	float theta = asin(p.y);
	float u = 1 - (phi + M_PI) / (2 * M_PI);
	float v = (theta + M_PI / 2) / M_PI;

	return vec2(u, v);
}
