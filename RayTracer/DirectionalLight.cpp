#include "DirectionalLight.h"

vec3 DirectionLight::CalculateLighting(const vec3& normal, const ray& ray, float _Glossiness, vec3 Color, const vec3& pos) const
{
	BRDFs brdf;

	//Ambiant 
	vec3 ambient = vec3{ 0.0f, 0.0f, 0.0f };

	//Diffuse 
	float diffuseFactor = brdf.clamp(std::max(0.f, normal.dot(getDirection() * -1)), 0.f, 1.f);
	vec3 diffuseColor = Color * this->color * this->intensity * diffuseFactor;

	//Specular
	vec3 R = brdf.reflect(ray.direction, normal);
	float specularFactor = brdf.clamp(std::pow(std::max(0.f, R.dot(ray.direction * -1)), _Glossiness), 0.f, 1.0f);
	vec3 specularColor = vec3{ 1.f, 1.f, 1.f } * specularFactor * this->color * this->intensity;

	Color = ambient + diffuseColor;

	if (_Glossiness > 0)
		Color = ambient + Color * diffuseColor + Color * specularColor;

	return Color;
}