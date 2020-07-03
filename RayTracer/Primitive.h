#pragma once
#include "ray.h"

struct Material {

	enum Type {
		MATTE, //reflexion diffuse seule (sans inter-reflexions)
		PLASTIC, //reflexion diffuse (lambert phong)
		METALLIC, //inter-reflexions seules (théoriquement, pas de diffuse)
		DIELECTRIC, //reflexions et refractions en meme temps (pas correct d'un pov physique)
	};
	
	Type type;
	vec3 color;
	float IOR;
	float roughness;

	Material(Type t, const vec3& c, float indice = 1.f, float rough =  10.f): type(t), color(c), IOR(indice), roughness(rough) {}
};

struct Primitive
{
	const Material* material;
	vec3 color;
	
	Primitive() {}
	Primitive(const Material* mat, vec3 col) : material(mat), color(col){}

	virtual float intersect(const ray& ray) const = 0;
	virtual vec3 calculateNormal(const vec3& p) const = 0;
	virtual ~Primitive() {}
};

//exo 1 : appliquer le lambert (N dot L) : L = -direction
//exo 2 : envoyer un rayon type "feeler" vers les lights si intersection => ombre col = vec3(0, 0, 0)
struct Light
{
	vec3 position;
	vec3 color;
	vec3 intensity;
};

struct DirectionLight
{
	vec3 direction;
	vec3 color;
	vec3 intensity;
};