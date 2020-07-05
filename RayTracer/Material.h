#pragma once
#include "vec3.h"

class Material {

public:
	enum class Type {
		MATTE, //reflexion diffuse seule (sans inter-reflexions)
		PLASTIC, //reflexion diffuse (lambert phong)
		METALLIC, //inter-reflexions seules (théoriquement, pas de diffuse)
		DIELECTRIC, //reflexions et refractions en meme temps (pas correct d'un pov physique)
	};

private:
	Type type;
	float IOR;
	float glossiness;
	vec3 color;

public:
	Material(Type t, vec3 col, float indice = 1.f, float rough = 10.f) : type(t), color(col), IOR(indice), glossiness(rough) {}

	Type getType() const { return type; }
	float getGlossiness() const { return glossiness; }
	vec3 getColor() const { return color; }
	float getIOR() const { return IOR; }
};