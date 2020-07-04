#pragma once
#include "ray.h"
#include "Entity.h"
#include "Material.h"
#include "vec2.h"

class Primitive : public Entity
{
protected:
	const Material* material;

public:
	Primitive(){}
	Primitive(vec3 pos, const Material* mat) : Entity(pos), material(mat){}

	virtual float intersect(const ray& ray) const = 0;
	virtual vec3 calculateNormal(const vec3& p) const = 0;
	virtual vec2 calculateUVs(const vec3& p) const = 0;
	virtual ~Primitive() {}

	const Material* getMaterial() const { return material; }
};