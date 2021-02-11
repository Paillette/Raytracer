#include "InfiniteCylinder.h"
#define M_PI 3.14159265358979323846

InfiniteCylinder::InfiniteCylinder()
    :radius(1.0f), Primitive(vec3{ 0, 0, 0 }, new Material("mat", Material::Type::MATTE, vec3{ 1, 1, 1 }, 0.f, 0.f))
{
}

InfiniteCylinder::InfiniteCylinder(const vec3& pos, float r, Material* mat)
    : radius(r), Primitive::Primitive(pos, mat)
{
}

float InfiniteCylinder::intersect(const ray& r) const
{
    ray rXZ = r;
    rXZ.origin.y = 0.0f;
	rXZ.direction.y = 0.0f;
	
	vec3 diff = rXZ.origin - position;
	float a = diff.dot(diff);
	vec3 v = rXZ.direction;
	float b = diff.dot(v.normalize());
	float c = a - radius * radius;
	float t = b * b - c;

	if (t >= 0.f)
	{
		t = -b - sqrtf(t);
	}
	return t;
}

vec3 InfiniteCylinder::calculateNormal(vec3& p) const
{
    vec3 i = globalToLocal(p);
    vec3 n = vec3{ i.x, 0, i.z };

    return n;
}

vec3 InfiniteCylinder::calculateUVs(vec3& p) const
{
	vec3 pHit = globalToLocal(p);

	float theta = atan2(pHit.x, pHit.z);
	float rawU = theta / (2 * M_PI);
	float u = 1.0f - (rawU + 0.5f);
	
	float v = fmod(pHit.y, 1);

	float S = 30.f;
	if (radius >= 1)
		S = radius * 10.f;
	else
		S = radius * 30.f;

	if(v < 0)
		v = 1 + v;

	return vec3{ u, v, 10.f };
}
