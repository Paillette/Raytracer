#pragma once
#include "vec3.h"

class Entity {
protected:
	vec3 position;

public:
	Entity() { position = vec3{ 0, 0, 0 }; }
	Entity(vec3 p) : position(p) {}

	~Entity() {}

	vec3 getPosition() const { return position; }
};