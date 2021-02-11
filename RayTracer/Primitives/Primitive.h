#pragma once
#include "..\Entity.h"
#include "..\Utilities\vec3.h"
#include "..\Material\Material.h"
#include "..\Utilities\ray.h"

class Primitive : public Entity
{
protected:
	const Material* material;

public:
	Primitive(){}
	Primitive(vec3 pos, const Material* mat) : Entity(pos), material(mat){}


	//return intersection distance if delta is positive
	//else return negative number
	virtual float intersect(const ray& ray) const = 0;
	virtual vec3 calculateNormal(vec3& p) const = 0;
	virtual vec3 calculateUVs(vec3& p) const = 0;
	virtual ~Primitive() {}

	const Material* getMaterial() const { return material; }
};