#pragma once
#include <vector>
#include <functional>
#include "Background.h"
#include "Sphere.h"
#include "ray.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Plane.h"
#include "Tri.h"
#include "Cube.h"
#include "InfiniteCylinder.h"
#include "Texture.h"

struct tracer {

	struct Intersection
	{
		float distance;
		const Primitive* primitive;

		Intersection() : distance(std::numeric_limits<float>::max()), primitive(nullptr) {}
	};

	//limite au nombre de récursion maximum
	static const int MAX_DEPTH = 100;
	//constexpr : c'est bien une variable const 
	static constexpr float EPSILON = 0.0001f;

	Background background;
	std::vector<Primitive*> scene;
	std::vector<Light*> lights;

	Material* matGrey = new Material("matBlue", Material::Type::MATTE, color{ 0.8f, 0.8f, 0.8f }, true, 0.f, 0.f);
	Material* matPink = new Material("matPink", Material::Type::MATTE, color{ 1.f, 0.3f, 0.5f }, 0.f, 0.f);
	Material* matRed = new Material("matRed", Material::Type::MATTE, color{ 1.f, 0.13f, 0.2f }, 0.f, 0.f);
	Material* matBlue = new Material("matBlue", Material::Type::MATTE, color{ 0.13f, 0.18f, 0.8f }, 0.f, 0.f);
	Material* matGreen= new Material("matGreen", Material::Type::MATTE, color{ 0.8f, 1.f, 0.2f }, 0.f, 0.f);
	Material* matOrange = new Material("matOrange", Material::Type::MATTE, color{ 0.9f, 0.3f, 0.0f }, 0.f, 0.f);

	Material* metallic = new Material("metallic", Material::Type::METALLIC, color{ 0.5f, 0.5f, 0.5f },  1.0f, 100.0f);
	Material* plastic = new Material("plastic", Material::Type::PLASTIC, color{ 0.5f, 0.5f, 0.5f }, 1.8f, 100.f);
	Material* glass = new Material("glass", Material::Type::DIELECTRIC, color{ 0.f, 0.5f, 0.5f }, 1.f, 1.f);

	tracer(std::vector<Primitive*> primTab, std::vector<Light*> lightTab) {
		//Objects
		for (int i = 0; i < primTab.size(); i++)
			scene.push_back(primTab[i]);
		//Lights
		for (int j = 0; j < lightTab.size(); j++)
			lights.push_back(lightTab[j]);
	}

	~tracer()
	{
	}

	bool GetIntersection(Intersection& intersection, const ray& ray, vec3& col);
	virtual vec3 refract(const ray& hitPos, const vec3& normal, const float& ior);
	virtual vec3 calculateLighting(const vec3& normal, const ray& rayon, std::vector<Light*> light, float _Glossiness, vec3 Color, const vec3& pos = vec3());
	bool inShadow(const ray& ray);
	float AmbientOcclusion(const vec3& position, const vec3& normal);
	vec3 GlobalIllumination(const vec3& position, const vec3& normal);
	inline float random_float();

	//methode de Whitted
	vec3 trace(const ray& rayon, int depth = 0);

};