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

float tracer::AmbientOcclusion(const vec3& pos, const vec3& n)
{
	randomNumbers random;

	float aoIntensity = 0.25f;
	int nbSamples = 5;
	float occlusionDistance = 2.0f;

	vec3 hitPos = pos + n * EPSILON;

	double percent = 0.0;
	for (int i = 0; i < nbSamples; i++)
	{
		vec3 hemisphereDir = random.randomDirectionInHemisphere(n);
		ray hemisphereRay = ray(hitPos, hemisphereDir);

		Intersection currentTry;
		GetIntersection(currentTry, hemisphereRay, vec3());

		percent += (currentTry.distance > occlusionDistance) ?
			1.0 : (currentTry.distance / occlusionDistance);
	}

	return percent / nbSamples;
}

vec3 tracer::GlobalIllumination(const vec3& pos, const vec3& n)
{
	randomNumbers random;
	int nbSamples = 10;

	vec3 hitPos = pos + n * EPSILON;
	color indirectLigthing = vec3();

	for (int i = 0; i < nbSamples; i++)
	{
		vec3 hemisphereDir = random.randomDirectionInHemisphere(n);
		ray hemisphereRay = ray(hitPos, hemisphereDir);

		Intersection currentTry;
		color col;

		if (GetIntersection(currentTry, hemisphereRay, col))
			indirectLigthing += col;
	}

	return indirectLigthing / nbSamples;
}

bool tracer::GetIntersection(Intersection& intersection, const ray& ray, vec3& col)
{
	for (const Primitive* primit : scene)
	{
		float distance = primit->intersect(ray);

		if (distance > EPSILON && distance < intersection.distance)
		{
			intersection.distance = distance;
			intersection.primitive = primit;
			col = intersection.primitive->getMaterial()->getColor();
		}
	}

	if (intersection.distance == std::numeric_limits<float>::max())
		return false;

	return true;
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
	vec3 result = lights[0]->CalculateLighting(normal, ray, _Glossiness, col, pos);
	for (size_t i = 1; i < lights.size(); i++)
	{
		result += lights[i]->CalculateLighting(normal, ray, _Glossiness, col, pos);
	}

	return result;
}

vec3 tracer::trace(const ray& rayon, int depth)
{
	color col = background.Get(rayon.direction);
	if (depth >= MAX_DEPTH)
	{
		return col;
	}

	Intersection intersection;
	BRDFs brdf;
	randomNumbers random;

	//calcul de la couleur du background pour ce pixel
	//on itere sur l'ensemble des primitives de la scene
	GetIntersection(intersection, rayon, col);

	if (intersection.primitive != nullptr)
	{
		//1 calcul du point d'intersection 
		vec3 position = rayon.evaluate(intersection.distance);
		//2 calcul d'une normale (d'une sphere ici)
		vec3 normal = intersection.primitive->calculateNormal(position);
		//3 shadow feeler
		
		float shadow = 0.8f;
		bool isInShadow = true;
		for (int i = 0; i < lights.size(); i++)
		{
			ray feeler;
			feeler.origin = position + normal * EPSILON;
			feeler.direction = lights[i]->getDirection(position) * -1.f;

			if (!inShadow(feeler))
			{
				isInShadow = false;
				break;
			}
		}

		if(isInShadow)
			shadow = 0.f;

		//4 initialisation du nouveau rayon 
		ray newRay;
		vec3 reflect = brdf.reflect(rayon.direction, normal);
		const Material* mat = intersection.primitive->getMaterial();
		vec3 direct = vec3();
		vec3 global = vec3();

		switch (intersection.primitive->getMaterial()->getType())
		{
			case Material::Type::MATTE:
				direct = calculateLighting(normal, rayon, lights, mat->getGlossiness(), col, position) * shadow;
				global = GlobalIllumination(position, normal);
				col = (direct + global) * col / M_PI;
				break;

			case Material::Type::PLASTIC:
				newRay.direction = reflect;
				newRay.origin = position + normal * EPSILON;
				direct = col * trace(newRay, depth + 1) + calculateLighting(normal, newRay, lights, mat->getGlossiness(), col, position) * shadow;
				global = GlobalIllumination(position, normal);
				col = (direct + global) * col / M_PI;
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

		col = col * AmbientOcclusion(position, normal);
	}

	return col;
}