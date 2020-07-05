#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(vec3 pos, vec3 col, float i);

	virtual vec3 CalculateLighting(const vec3& normal, const ray& ray, float _Glossiness, vec3 Color, const vec3& pos) const;
    
	virtual vec3 getDirection(const vec3& pos = vec3()) const { return (pos - position).normalize(); }
};