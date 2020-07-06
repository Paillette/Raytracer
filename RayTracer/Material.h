#pragma once
#include "vec3.h"
#include "vec2.h"
#include <cmath>
#include <cfenv>

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
	bool stripe;

public:
	Material(Type t, vec3 col, bool StripeOrColor, float indice = 1.f, float rough = 10.f) : type(t), color(col), stripe(StripeOrColor), IOR(indice), glossiness(rough) {}

	Type getType() const { return type; }
	float getGlossiness() const { return glossiness; }
	vec3 getColor(vec3 uvS = vec3()) const { 
		if (stripe)
		{
			float patternU = fmod(uvS.x * uvS.z, 1) < 0.5;
			return color * patternU;
		}
		else
			return color; 
	}
	float getIOR() const { return IOR; }
};