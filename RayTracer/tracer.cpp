#pragma once
#include "tracer.h"
#include <algorithm>
#include <random>
#include <functional>
#include "Plane.h"
#include "BRDFs.h"

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
	//Diffuse 
	float diffuseFactor = brdf.clamp(std::max(0.f, normal.dot(light->direction * -1)), 0.f, 1.f);
	vec3 R = brdf.reflect(rayon.direction, normal);
	//Specular
	float specularFactor = brdf.clamp(std::pow(std::max(0.f, R.dot(rayon.direction * -1)), _Glossiness), 0.f, 1.0f);

	Color = Color * diffuseFactor;
	if(_Glossiness > 0)
		Color = Color + vec3({ 0.5, 0.5, 0.5 }) * specularFactor;
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
		}
	}

	if (intersection.primitive != nullptr)
	{
		vec3 BC = intersection.primitive->color;
		//1 calcul du point d'intersection 
		vec3 position = rayon.evaluate(intersection.distance);
		//2 calcul d'une normale (d'une sphere ici)
		vec3 normal = intersection.primitive->calculateNormal(position);
		//3 shadow feeler
		ray feeler;
		feeler.origin = position + normal * EPSILON;
		feeler.direction = directionalLight->direction * -1;
		float shadow = 1.f;

		if (inShadow(feeler))
		{
			shadow = 0.f;
		}

		//4 initialisation du nouveau rayon 
		ray newRay;

		switch (intersection.primitive->material->type)
		{
			
			case Material::Type::MATTE:
				newRay.direction = normal;
				newRay.origin = position + normal * EPSILON;
				col = calculateLighting(normal, rayon, directionalLight, 0.f, BC) * shadow + (BC * 0.1);
				break;

			case Material::Type::PLASTIC:
				vec3 reflected = brdf.reflect(rayon.direction, normal);
				newRay.direction = reflected;
				newRay.origin = position + normal * EPSILON;
				BC = trace(newRay, depth + 1);
				col = calculateLighting(normal, newRay, directionalLight, intersection.primitive->material->roughness, BC) * shadow + (BC * 0.2);

				break;

			case Material::Type::DIELECTRIC:
				vec3 refractionDirection = refract(brdf.reflect(rayon.direction, normal), normal, 1.3f).normalize();
				newRay.direction = refractionDirection;
				newRay.origin = position + refractionDirection * EPSILON;
				col = trace(newRay, depth + 1);
				break;
			case Material::Type::METALLIC:
				vec3 reflectionColor = trace(newRay, depth + 1);
				col = reflectionColor;
				break;
			default:
				//appel recursif de trace()
				col = col * trace(newRay, depth + 1);
		}
	}
	return col;
}