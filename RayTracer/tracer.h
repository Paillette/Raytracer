#pragma once
#include <vector>
#include <functional>
#include "Background.h"
#include "Sphere.h"
#include "ray.h"
#include "Plane.h"

struct tracer {

	//limite au nombre de récursion maximum
	static const int MAX_DEPTH = 100;
	//constexpr : c'est bien une variable const 
	static constexpr float EPSILON = 0.0001f;

	Background background;
	std::vector<Primitive*> scene;

	Material* mat = new Material(Material::Type::MATTE, vec3{ 0.0f, 0.0f, 1.0f }, 0.f, 0.f);
	Material* metallic = new Material(Material::Type::METALLIC, vec3{ 1.0f, 0.0f, 0.0f });
	Material* plastic = new Material(Material::Type::PLASTIC, vec3{ 1.0f, 0.0f, 0.0f }, 1.f, 500.f);
	Material* glass = new Material(Material::Type::DIELECTRIC, vec3{ 1.0f, 1.0f, 1.0f }, 1.3f, 1.f);

	DirectionLight* directionalLight = new DirectionLight { vec3{ 1.f, -1.f, 1.f }.normalize(), { 1.f, 1.f, 1.f}, 1.f };
	//Light* pointLight = new Light{ { -1.0f, 0.0f, 0.f}, { 1.f, 1.f, 1.f }, 1.f };

	tracer() {
		scene.push_back(new Plane{ {0.f, -0.9f, 0.f}, plastic, {1.f, 0.f, 0.f} });
		scene.push_back(new Sphere{ {0.f, 0.f, 3.f}, 1.f , mat, {0.0f, 0.5f, 1.0f } });
		scene.push_back(new Sphere{ {1.f, 2.f, 5.f}, 0.8f, mat, { 0.0f, 0.0f, 1.0f } });
		scene.push_back(new Sphere{ {-0.5f, -0.5f, 2.f}, 0.4f, mat, { 0.0f, 1.0f, 0.0f} });
	}

	~tracer()
	{
		for (Primitive* primitive : scene)
			delete primitive;
	}

	/*
	vec3 traceAppel(const ray& ray)
	{
		//calcul de la couleur du background pour ce pixel
		color col = { background.Get(ray.direction) };

		if (sphere.intersect(ray) > 0.f)
		{
			col = { 0.5f, 0.f, 0.f };
		}
		else if (sphere2.intersect(ray) > 0.f)
		{
			col = { 0.f, 1.f, 0.f };
		}
		return col;
	}*/

	virtual vec3 refract(const vec3& hitPos, const vec3& normal, const float& ior);
	virtual vec3 calculateLighting(const vec3& normal, const ray& rayon, DirectionLight* light, float _Glossiness, vec3 Color);
	virtual bool inShadow(const ray& ray);
	inline float random_float();

	///methode de Whitted
	vec3 trace(const ray& rayon, int depth = 0);

};