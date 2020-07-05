#include "PointLight.h"

PointLight::PointLight(vec3 pos, float r, vec3 col, float i)
    :Light(pos, col, i)
{
}

vec3 PointLight::CalculateLighting(const vec3& normal, const ray& ray, float _Glossiness, vec3 Color, const vec3& pos) const
{
    vec3 v = pos - position;
    float dist = sqrt(std::pow(v.x, 2.0f) + std::pow(v.y, 2.0f) + std::pow(v.z, 2.0f));
    
    BRDFs brdf;

    //Ambiant 
    vec3 ambient = vec3{ 0.0f, 0.0f, 0.0f };

    //Diffuse 
    float diffuseFactor = brdf.clamp(std::max(0.f, normal.dot(getDirection(pos) * -1)), 0.f, 1.f);
    vec3 diffuseColor = Color * getColor() * getIntensity() * diffuseFactor;

    //Specular
    vec3 R = brdf.reflect(ray.direction, normal);
    float specularFactor = brdf.clamp(std::pow(std::max(0.f, R.dot(ray.direction * -1)), _Glossiness), 0.f, 1.0f);
    vec3 specularColor = vec3{ 1.f, 1.f, 1.f } *specularFactor * getColor() * getIntensity();

    float attenuation = 1.0 / (0.f + 0.f * dist +
        1.f * (dist * dist));

    Color = ambient * attenuation + diffuseColor * attenuation;

    if (_Glossiness > 0)
        Color = ambient * attenuation + Color * diffuseColor * attenuation + Color * specularColor * attenuation;

    return Color;
}
