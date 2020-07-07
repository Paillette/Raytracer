#include "Sphere.h"
#define M_PI 3.14159265358979323846

Sphere::Sphere()
	:radius(1.0f), Primitive(vec3{ 0, 0, 0 }, new Material("mat", Material::Type::MATTE, vec3{ 1, 1, 1 }, 0.f, 0.f))
{
}

Sphere::Sphere(const vec3& pos, float r, Material* mat)
	:radius(r), Primitive::Primitive(pos, mat)
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

vec3 Sphere::calculateUVs(const vec3& p) const
{
	vec3 pHit = globalToLocal(p).normalize();

	float phi = atan2(pHit.x, pHit.z);
	float theta = acos(pHit.y);

	float u = 1 - (phi + M_PI) / (2 * M_PI);
	float v = (abs((theta - M_PI / 2) / M_PI)) / radius;

	float S = 30.f;
	if (radius >= 1)
		S = radius * 10.f;
	else
		S = radius * 30.f;

	return vec3{ u, v, S };
}
