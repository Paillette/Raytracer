#pragma once
#include "tracer.h"
#include <algorithm>
#include <random>
#include <functional>
#include "Plane.h"
#include "BRDFs.h"
#include "RandomNumbers.h"
#include <iostream>

float tracer::random_float()
{
	// equivalent de (float)rand()/(float)RAND_MAX
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	static std::function<float()> rand_generator = std::bind(distribution, generator);
	return rand_generator();
}

vec3 tracer::calculateLighting(const vec3& normal, const ray& rayon, DirectionLight* light, float _Glossiness, vec3 Color)
{
	BRDFs brdf;

	//Ambiant 
	color ambient = vec3{ 0.02f, 0.02f, 0.02f };

	//Diffuse 
	float diffuseFactor = brdf.clamp(std::max(0.f, normal.dot(light->getDirection() * -1)), 0.f, 1.f);
	color diffuseColor = Color * light->getColor() * light->getIntensity() * diffuseFactor;

	//Specular
	vec3 R = brdf.reflect(rayon.direction, normal);
	float specularFactor = brdf.clamp(std::pow(std::max(0.f, R.dot(rayon.direction * -1)), _Glossiness), 0.f, 1.0f);
	color specularColor = vec3{ 1.f, 1.f, 1.f } *specularFactor * light->getColor() * light->getIntensity();

	Color = ambient + diffuseColor;

	if (_Glossiness > 0)
		Color = ambient + Color * diffuseColor + Color * specularColor;
	return Color;
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

vec3 tracer::refract(const ray& ray, const vec3& normal, const float& ior)
{
	float cosi = ray.direction.dot(normal);
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
	return k < 0 ? vec3{ 1, 0, 0 } : (ray.direction * n) + n * ( eta * cosi - sqrtf(k));
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
		ray feeler;
		feeler.origin = position + normal * EPSILON;
		feeler.direction = directionalLight->getDirection() * -1;
		float shadow = 1.f;

		if (inShadow(feeler))
		{
			shadow = 0.0f;
		}

		//4 initialisation du nouveau rayon 
		ray newRay;
		vec3 reflect = brdf.reflect(rayon.direction, normal);
		const Material* mat = intersection.primitive->getMaterial();

		switch (intersection.primitive->getMaterial()->getType())
		{
			case Material::Type::MATTE:
				col = calculateLighting(normal, rayon, directionalLight, mat->getGlossiness(), col) * shadow;
				break;

			case Material::Type::PLASTIC:
				newRay.direction = reflect;
				newRay.origin = position + normal * EPSILON;
				float kr;
				fresnel(rayon.direction, normal, mat->getIOR(), kr);
				col = col * trace(newRay, depth + 1) * kr + calculateLighting(normal, newRay, directionalLight, mat->getGlossiness(), col) * shadow;
				break;

			case Material::Type::DIELECTRIC:
				color refractedColor = vec3{ 0, 0, 0 };
				//fresnel
				fresnel(rayon.direction, normal, mat->getIOR(), kr);

				ray reflectedRay;
				reflectedRay.direction = reflect;
				reflectedRay.origin = position + normal * EPSILON;
				color reflectionColor = trace(reflectedRay, depth + 1);

				ray refractedRay;

				if (kr < 1)
				{
					vec3 refractionDirection = refract(rayon, normal, mat->getIOR()).normalize();
					refractedRay.direction = refractionDirection;
					refractedRay.origin = position + normal * EPSILON;
					refractedColor = trace(refractedRay, depth + 1);
				}
				std::cout << depth << std::endl;
				col = refractedColor;
				break;

			case Material::Type::METALLIC:
				newRay.direction = reflect;
				newRay.origin = position + normal * EPSILON;
				col = col * trace(newRay, depth + 1) + calculateLighting(normal, newRay, directionalLight, mat->getGlossiness(), col) * shadow;
				break;

			default:
				//appel recursif de trace()
				col = col * trace(newRay, depth + 1);
		}
	}
	return col;
}