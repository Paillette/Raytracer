#pragma once
#include "ray.h"
#include "Entity.h"
#include "Material.h"
#include "vec2.h"

class Primitive : public Entity
{
protected:
	const Material* material;
	vec3 color;

public:
	Primitive(){}
	Primitive(vec3 pos, const Material* mat, vec3 col) : Entity(pos), material(mat), color(col){}

	virtual float intersect(const ray& ray) const = 0;
	virtual vec3 calculateNormal(const vec3& p) const = 0;
	virtual vec2 calculateUVs(const vec3& p) const = 0;
	virtual ~Primitive() {}

	vec3 getColor() const { return color; }
	const Material* getMaterial() const { return material; }
};