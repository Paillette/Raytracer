#pragma once
#include "vec3.h"
#include <cmath>
#include <algorithm>

class BRDFs
{
public:

	float sqr(float x) {
		return x * x;
	}

	float clamp(float n, float lower, float upper) 
	{
		return std::max(lower, std::min(n, upper));
	}

	float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	vec3 lerp(vec3 a, vec3 b, float f)
	{
		return (b - a) * (a + f) ;
	}


	vec3 reflect(const vec3& hitPos, const vec3& normal)
	{
		vec3 rayonReflect = hitPos - normal * (2 * (hitPos.dot(normal)));
		return rayonReflect;
	}

	float MixFunction(float i, float j, float x) {
		return  j * x + i * (1.0 - x);
	}

	//roughness
	float calculateRoughness(float _Glossiness)
	{
		float roughness = 1 - (_Glossiness * _Glossiness);
		roughness = roughness * roughness;
		return roughness;
	}

	//diffuse
	vec3 calculateDiffuse(vec3 Color, float _Metallic)
	{
		vec3 diffuseColor = Color * (1 - _Metallic);
		return diffuseColor;
	}

	//GGX Normal distribution
	float GGXNormal(float roughness, float NdotH)
	{
		float roughnessSqr = roughness * roughness;
		float NdotHSqr = NdotH * NdotH;
		float TanNdotHSqr = (1 - NdotHSqr) / NdotHSqr;
		return (1.0 / 3.1415926535) * sqr(roughness / (NdotHSqr * (roughnessSqr + TanNdotHSqr)));
	}
	//Geometric Shadowing
	float GShadow(float NdotL, float NdotV)
	{
		float Gs = (NdotL * NdotV);
		return Gs;
	}

	//normal incidence reflection calculation
	float F0(float NdotL, float NdotV, float LdotH, float roughness) {
		// Diffuse fresnel
		float FresnelLight = SchlickFresnel(NdotL);
		float FresnelView = SchlickFresnel(NdotV);
		float FresnelDiffuse90 = 0.5 + 2.0 * LdotH * LdotH * roughness;
		return  MixFunction(1, FresnelDiffuse90, FresnelLight) * MixFunction(1, FresnelDiffuse90, FresnelView);
	}
	//Fresnel equation
	float SchlickFresnel(float i) {
		float x = clamp(1.0 - i, 0.0, 1.0);
		float x2 = x * x;
		return x2 * x2 * x;
	}

	vec3 SchlickFresnelFunction(vec3 SpecularColor, float LdotH) {
		return SpecularColor + (vec3{ 1.f, 1.f, 1.f } -SpecularColor) * SchlickFresnel(LdotH);
	}

	vec3 FresnelLerp(vec3 x, vec3 y, float d)
	{
		float t = SchlickFresnel(d);
		return lerp(x, y, t);
	}

	float SchlickIORFresnelFunction(float ior, float LdotH) 
	{
		float f0 = pow(ior - 1, 2) / pow(ior + 1, 2);
		return f0 + (1 - f0) * SchlickFresnel(LdotH);
	}

	//Shading
	vec3 calculateLight(vec3 normal, const ray& rayon, DirectionLight* light, float _Glossiness, vec3 Color, vec3 SpecColor, float ior, float Metallic, vec3 position)
	{
		//normal direction calculations
		vec3 normalDirection = normal.normalize();
		vec3 viewDirection = (rayon.origin - position).normalize(); 
		float shiftAmount = normal.dot(viewDirection);
		normalDirection = shiftAmount < 0.0f ? normalDirection + viewDirection * (-shiftAmount + 1e-5f) : normalDirection;

		//light calculations
		//vec3 lightDirection = lerp(light.direction.x, light.direction.y - rayon.direction.y, light.direction.z).normalize();
		vec3 lightReflectDirection = reflect(light->direction, normalDirection);
		vec3 viewReflectDirection = reflect(viewDirection, normalDirection).normalize();
		float NdotL = std::max(0.f, normalDirection.dot(light->direction * -1));
		vec3 halfDirection = (viewDirection + light->direction * -1).normalize();
		float NdotH = std::max(0.f, normalDirection.dot(halfDirection));
		float NdotV = std::max(0.f, normalDirection.dot(viewDirection));
		float LdotH = std::max(0.f, (light->direction * -1).dot(halfDirection));
		float attenuation = 5.f;
		vec3 attenColor = light->color * attenuation;

		//diffuse color calculations
		float roughness = calculateRoughness(_Glossiness);
		vec3 diffuseColor = calculateDiffuse(vec3(Color), Metallic);
		float f0 = F0(NdotL, NdotV, LdotH, roughness);
		diffuseColor = diffuseColor * f0;

		//Specular calculations
		float indirectSpecular = 0.1f;
		//TODO : faire fonctionner le lerp
		vec3 specColor = SpecColor;// lerp(SpecColor, Color, Metallic * 0.5);
		float GeometricShadow = 1;
		vec3 FresnelFunction = specColor;
		vec3 SpecularDistribution = specColor * GGXNormal(roughness, NdotH);
		SpecularDistribution = SpecularDistribution * SpecularDistribution;

		//geometric shadow
		GeometricShadow *= GShadow(NdotL, NdotV);

		//fresnel
		FresnelFunction = FresnelFunction * SchlickFresnelFunction(SpecColor, LdotH);

		vec3 specularity = (SpecularDistribution * FresnelFunction * GeometricShadow) / (4 * (NdotL * NdotV));
		vec3 IndirectSpecularity = FresnelLerp(SpecColor, vec3{ roughness, roughness, roughness }, NdotV) * indirectSpecular * std::max(0.15f, Metallic) * (1 - roughness * roughness * roughness);

		vec3 lightingModel = diffuseColor + specularity +IndirectSpecularity;
		lightingModel = lightingModel * NdotL;
		vec3 finalDiffuse = lightingModel* attenColor;
		return finalDiffuse;
	}
};