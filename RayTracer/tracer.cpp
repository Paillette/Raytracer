#pragma once
#include "tracer.h"
#include <algorithm>
#include <random>
#include <functional>
#include "Plane.h"
#include "BRDFs.h"
#include "RandomNumbers.h"
#include "vec2.h"
#include "Properties.h"

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
			if (primitive->intersect(ray) > 0.0f)
				return true;
		}

		return false;
}

float tracer::AmbientOcclusion(const vec3& pos, const vec3& n)
{
	randomNumbers random;

	float aoIntensity = 0.25f;
	int nbSamples = 10;
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

void fresnel(const vec3& I, const vec3& N, const float& ior, float& kr)
{
	BRDFs brdf;
	float cosi = brdf.clamp(I.dot(N), -1.f, 1.0f);
	float etai = 1, etat = ior;
	if (cosi > 0) { std::swap(etai, etat); }
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 4;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}

vec3 tracer::refract(const ray& hitPos, const vec3& normal, const float& ior)
{
	float cosi = hitPos.direction.dot(normal);
	float etai = 1, etat = ior;
	vec3 n = normal;
	if (cosi > 0)
	{
		etai = ior;
		etat = 1.0f;
		n = normal * -1;
	}
	else
	{
		etai = 1.0f;
		etat = ior;
		cosi = -cosi;
	}
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0 ? vec3{ 1, 0, 0 } : (hitPos.direction * n) + n * (eta * cosi - sqrtf(k));
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
	constant_texture texture;

	//calcul de la couleur du background pour ce pixel
	//on itere sur l'ensemble des primitives de la scene
	GetIntersection(intersection, rayon, col);

	if (intersection.primitive != nullptr)
	{
		//1 calcul du point d'intersection 
		vec3 position = rayon.evaluate(intersection.distance);
		//2 calcul d'une normale (d'une sphere ici)
		vec3 normal = intersection.primitive->calculateNormal(position);
		//calcul d'uv
		vec3 uvS = intersection.primitive->calculateUVs(position);
		col = intersection.primitive->getMaterial()->getColor(uvS);

		//3 shadow feeler
		float shadow = 1.f;
		bool isInShadow = true;

		for (int i = 0; i < lights.size(); i++)
		{
			ray feeler;
			feeler.origin = position + normal * EPSILON;
			feeler.direction = lights[i]->getDirection(position) * -1.f;

			if (!inShadow(feeler) &&
				position.distance(lights[i]->getPosition()) < lights[i]->getRange())
			{
				isInShadow = false;
				break;
			}
		}

		if (Properties::get()->getShadow() == false)
			isInShadow = false;

		if(isInShadow)
			shadow = 0.f;


		//4 initialisation du nouveau rayon 
		ray newRay;
		vec3 reflect = brdf.reflect(rayon.direction, normal);
		const Material* mat = intersection.primitive->getMaterial();
		vec3 direct = vec3();
		vec3 global = vec3{ 0, 0, 0 };
		float kr;
		ray reflectedRay;
		ray refractedRay;

		switch (intersection.primitive->getMaterial()->getType())
		{
			case Material::Type::MATTE:
				direct = calculateLighting(normal, rayon, lights, mat->getGlossiness(), col, position) * shadow;
				if(Properties::get()->getGI() == true)
				global = GlobalIllumination(position, normal);
				col = (direct + global) * col / M_PI;
				break;

			case Material::Type::PLASTIC:
				newRay.direction = reflect;
				newRay.origin = position + normal * EPSILON;
				fresnel(newRay.direction, normal, mat->getIOR(), kr);
				col = col * trace(newRay, depth + 1) * kr + calculateLighting(normal, newRay, lights, mat->getGlossiness(), col, position) * shadow;
				break;

			case Material::Type::DIELECTRIC:
				color refractedColor = vec3{ 0, 0, 0 };
				//fresnel
				fresnel(rayon.direction, normal, mat->getIOR(), kr);
				reflectedRay.direction = reflect;
				reflectedRay.origin = position + normal * EPSILON;
				color reflectionColor = trace(reflectedRay, depth + 1);

				if (kr < 1)
				{
					vec3 refractionDirection = refract(rayon, normal, mat->getIOR()).normalize();
					refractedRay.direction = refractionDirection;
					refractedRay.origin = position + normal * EPSILON;
					refractedColor = trace(refractedRay, depth + 1);
				}

				col = refractedColor; //reflectionColor * kr + 
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

		if (Properties::get()->getAO() == false)
			col = col;
		else
			col = col * AmbientOcclusion(position, normal);
	}

	return col;
}