#pragma once
#include <vector>
#include <functional>
#include "Background.h"
#include "ray.h"
#include "Light.h"
#include "Primitive.h"

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