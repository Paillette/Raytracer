#pragma once
#include "vec3.h"
#include "Matrix4.h"

class Entity {
protected:
	vec3 position;
	Matrix4 transform;
	Matrix4 transformInv;

public:
	Entity();
	Entity(vec3 p);

	vec3 localToGlobal(vec3 pos) const;
	vec3 globalToLocal(vec3 pos) const;

	~Entity() {}

	vec3 getPosition() const { return position; }
};