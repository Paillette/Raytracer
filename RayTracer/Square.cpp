#include "Square.h"

Square::Square()
    :size(1.0f), normal(vec3{ 0, 0, -1 }), Primitive(vec3{ 0, 0, 0 }, new Material(Material::Type::MATTE, vec3{ 1, 1, 1 }, 0.f, 0.f))
{
}

Square::Square(const vec3& pos, vec3 n, float s, Material* mat)
	:normal(n), size(s), Primitive::Primitive(pos, mat)
{
}


float Square::intersect(const ray& ray) const
{
	float denom = normal.dot(ray.direction);
	float t = 0.f;

	if (abs(denom) >= 0.0001f)
	{
		vec3 v = position - ray.origin;
		t = v.dot(normal) / denom;
		if (t >= 0.0001f)
		{
			vec3 impact = ray.origin + ray.direction * t;
			vec3 dist = impact - position;
			if (dist.x > -size && dist.x < size && dist.y > -size && dist.y < size && dist.z > -size && dist.z < size)
				return t;
		}
	}
	return 0.f;
}

vec3 Square::calculateNormal(const vec3& p) const
{
    return normal;
}

vec3 Square::calculateUVs(const vec3& p) const
{
	float u = (p.x - floor(p.x));
	float v = (p.y - floor(p.y));
	float w = size * 10.f;

	return vec3{ u, v, w };
}
