#pragma once
#include <random>
#include <functional>

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
};
