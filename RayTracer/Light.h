#pragma once
#include "vec3.h"
#include "Entity.h"
#include "ray.h"
#include "BRDFs.h"

class Light : public Entity
{
protected:
	static constexpr float EPSILON = 0.0001f;
	vec3 color;
	float intensity;
	float range;

public:
	Light(vec3 pos, vec3 col, float i, float range): Entity(pos), color(col), intensity(i), range(range) {}

	virtual vec3 CalculateLighting(const vec3& normal, const ray& ray, float _Glossiness, vec3 Color, const vec3& pos = vec3()) const = 0;
	
	virtual vec3 getDirection(const vec3& pos = vec3()) const = 0;
	vec3 getColor() const { return color; }
	float getIntensity() const { return intensity; }
	float getRange() const { return range; }
	
};