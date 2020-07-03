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

	Type type;
	float IOR;
	float roughness;

	Material(Type t, float indice = 1.f, float rough = 10.f) : type(t), IOR(indice), roughness(rough) {}
};