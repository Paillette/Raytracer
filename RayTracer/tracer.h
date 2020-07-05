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

	Material* mat = new Material(Material::Type::MATTE, color{ 0.5f, 0.5f, 0.5f } , 0.f, 0.f);
	Material* metallic = new Material(Material::Type::METALLIC, color{ 0.5f, 0.5f, 0.5f },  1.0f, 100.0f);
	Material* plastic = new Material(Material::Type::PLASTIC, color{ 0.5f, 0.5f, 0.5f }, 1.f, 100.f);
	Material* glass = new Material(Material::Type::DIELECTRIC, color{ 0.f, 0.5f, 0.5f }, 1.3f, 1.f);

	tracer() {
		//Objects
		scene.push_back(new Plane( vec3{0.f, -0.9f, 0.f}, metallic ));
		scene.push_back(new Sphere( vec3{ 0.0f, 0.f, 3.0f }, 1.f , mat));
		scene.push_back(new Sphere( vec3{ 1.f, 1.5f, 4.f }, 1.f, metallic ));
		scene.push_back(new Sphere( vec3{ -0.5f, -0.65f, 1.8f }, 0.3f, mat ));

		//Lights
		lights.push_back(new DirectionLight( vec3{ 1.f, -1.f, 1.f }.normalize(), color{ 1.f, 1.f, 1.f}, 2.f ));
		lights.push_back(new PointLight(vec3{ 0.f, 0.f, 1.5f }, color{ 1.f, 0.f, 0.f }, 1.0f));
	}

	~tracer()
	{
	}

	bool GetIntersection(Intersection& intersection, const ray& ray, vec3& col);
	virtual vec3 refract(const vec3& hitPos, const vec3& normal, const float& ior);
	virtual vec3 calculateLighting(const vec3& normal, const ray& rayon, std::vector<Light*> light, float _Glossiness, vec3 Color, const vec3& pos = vec3());
	bool inShadow(const ray& ray);
	float AmbientOcclusion(const vec3& position, const vec3& normal);
	vec3 GlobalIllumination(const vec3& position, const vec3& normal);
	inline float random_float();

	//methode de Whitted
	vec3 trace(const ray& rayon, int depth = 0);

};