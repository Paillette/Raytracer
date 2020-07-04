#pragma once
#include "tracer.h"
#include <algorithm>
#include <random>
#include <functional>
#include "Plane.h"
#include "BRDFs.h"
#include "RandomNumbers.h"

float tracer::random_float()
{
	// equivalent de (float)rand()/(float)RAND_MAX
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	static std::function<float()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
}

bool tracer::inShadow(const ray& ray)
{
		for (const Primitive* primitive : scene)
		{
			if (primitive->intersect(ray) > 0.f)
				return true;
		}

		return false;
}

vec3 tracer::refract(const vec3& hitPos, const vec3& normal, const float& ior)
{
	BRDFs brdf;
	float cosi = brdf.clamp(-1, 1, hitPos.dot(normal));
	float etai = 1, etat = ior;
	vec3 n = normal;
	if (cosi < 0) { cosi = -cosi; }
	else { std::swap(etai, etat); n = n - normal; }
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	if (k < 0)
		return { 0, 0, 0 };
	else
		return (hitPos * eta) + n * (eta * cosi - sqrtf(k));		
}

vec3 tracer::calculateLighting(const vec3& normal, const ray& ray, std::vector<Light*> light, float _Glossiness, vec3 col, const vec3& pos)
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		col += lights[i]->CalculateLighting(normal, ray, _Glossiness, col, pos);
	}

	return col;
}


vec3 tracer::trace(const ray& rayon, int depth)
{
	color col = background.Get(rayon.direction);
	if (depth >= MAX_DEPTH)
	{
		return col;
	}
	struct Intersection
	{
		float distance;
		const Primitive* primitive;
	};

	Intersection intersection{ std::numeric_limits<float>::max(), nullptr };
	BRDFs brdf;
	randomNumbers random;

	//calcul de la couleur du background pour ce pixel
	//on itere sur l'ensemble des primitives de la scene
	/*
	for (std::vector<Sphere>::iterator iter = scene.begin();
		iter != scene.end(); ++iter)
	*/
	for (const Primitive* primit : scene)
	{
		float distance = primit->intersect(rayon);


		if (distance > EPSILON&& distance < intersection.distance)
		{
			intersection.distance = distance;
			intersection.primitive = primit;
			col = static_cast<const Primitive*>(primit)->getMaterial()->getColor();
		}
	}

	if (intersection.primitive != nullptr)
	{
		//1 calcul du point d'intersection 
		vec3 position = rayon.evaluate(intersection.distance);
		//2 calcul d'une normale (d'une sphere ici)
		vec3 normal = intersection.primitive->calculateNormal(position);
		//3 shadow feeler
		
		float shadow = 1.f;
		
		for (int i = 0; i < lights.size(); i++)
		{
			ray feeler;
			feeler.origin = position + normal * EPSILON;
			feeler.direction = lights[i]->getDirection() * -1.f;

			if (inShadow(feeler))
			{
				shadow = 0.f;
			}
		}

		//4 initialisation du nouveau rayon 
		ray newRay;
		vec3 reflect = brdf.reflect(rayon.direction, normal);
		const Material* mat = intersection.primitive->getMaterial();

		switch (intersection.primitive->getMaterial()->getType())
		{
			case Material::Type::MATTE:
				col = calculateLighting(normal, rayon, lights, mat->getGlossiness(), col, position) * shadow;
				break;

			case Material::Type::PLASTIC:
				newRay.direction = reflect;
				newRay.origin = position + normal * EPSILON;
				col = col * trace(newRay, depth + 1) + calculateLighting(normal, newRay, lights, mat->getGlossiness(), col, position) * shadow;
				break;

			case Material::Type::DIELECTRIC:
				vec3 refractionDirection = refract(reflect, normal, 1.3f).normalize();
				newRay.direction = refractionDirection;
				newRay.origin = position + refractionDirection * EPSILON;
				col = trace(newRay, depth + 1);
				break;

			case Material::Type::METALLIC:
				newRay.direction = reflect;
				newRay.origin = position + normal * EPSILON;
				col = col * trace(newRay, depth + 1) + calculateLighting(normal, newRay, lights, mat->getGlossiness(), col, position) * shadow;
				break;

			default:
				//appel recursif de trace()
				col = col * trace(newRay, depth + 1);
		}
	}
	return col;
}