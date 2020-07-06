#pragma once
#include "vec3.h"
#include "vec2.h"

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
	vec3 getColor(vec2 uv = vec2()) const { 
		if (uv != vec2())
		{
			if (int(uv.x) % 2 == 0)
			{
				return vec3{ 1.f, 0.f, 0.f };
			}
			else
			{
				return vec3{ 0.f, 0.f, 1.f };
			}
		}
		else
			return color; 
	}
	float getIOR() const { return IOR; }
};