#pragma once
#include <random>
#include <functional>
#include "vec3.h"
#define M_PI 3.1416

class randomNumbers
{
public:
	float random_float() {
		// equivalent de (float)rand()/(float)RAND_MAX
		static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
		static std::mt19937 generator;
		static std::function<float()> rand_generator = std::bind(distribution, generator);
		return rand_generator();
	}

	float RandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
	/*
	vec3 random_unit_vector() {
		auto a = ((2.0 * M_PI) - 0.0) * ((double)rand() / (double)RAND_MAX) + 0.0;
		auto z = ((1.0) - (-1.0)) * ((double)rand() / (double)RAND_MAX) -1.0;
		auto r = sqrt(1 - z * z);
		return vec3{ r * cos(a), r * sin(a), z };
	}
	*/
	vec3 random_in_unit_sphere() {
		while (true) {
			float random = RandomFloat(-1, 1);
			vec3 p = { random, random, random };
			if (sqrt(pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2)) >= 1) continue;
			return p;
		}
	}

	vec3 randomDirectionInHemisphere(const vec3& normal)
	{
		vec3 randDir = vec3{
			(2.0f * RandomFloat(0, 1)) - 1.0f,
			(2.0f * RandomFloat(0, 1)) - 1.0f,
			(2.0f * RandomFloat(0, 1)) - 1.0f };
		randDir = randDir.normalize();
		return (randDir.dot(normal) >= 0.0) ? randDir : (randDir * -1.f);
	}
};
