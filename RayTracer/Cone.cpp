#include "Cone.h"
#define M_PI 3.14159265358979323846

Cone::Cone()
    :radius(1.0f), height(2.0f), Primitive(vec3{ 0, 0, 0 }, new Material(Material::Type::MATTE, vec3{ 1, 1, 1 }, 0.f, 0.f))
{
}

Cone::Cone(const vec3& pos, float r, float h, Material* mat)
    : radius(r), height(h), Primitive::Primitive(pos, mat)
{
}

float Cone::intersect(const ray& ray) const
{
    float A = ray.origin.x - position.x;
    float B = ray.origin.z - position.z;
    float D = height - ray.origin.y + position.y;

    float tan = (radius / height) * (radius / height);

    float a = (ray.direction.x * ray.direction.x) + (ray.direction.z * ray.direction.z) - (tan * (ray.direction.y * ray.direction.y));
    float b = (2 * A * ray.direction.x) + (2 * B * ray.direction.z) + (2 * tan * D * ray.direction.y);
    float c = (A * A) + (B * B) - (tan * (D * D));

    float delta = b * b - 4 * (a * c);
    if (fabs(delta) < 0.001) return 0;
    if (delta < 0.0) return 0;

    float t1 = (-b - sqrt(delta)) / (2 * a);
    float t2 = (-b + sqrt(delta)) / (2 * a);
    float t;

    if (t1 > t2) t = t2;
    else t = t1;

    float r = ray.origin.y + t * ray.direction.y;

    if ((r > position.y) && (r < position.y + height)) 
        return t;
    else 
        return 0;
}

vec3 Cone::calculateNormal(vec3& p) const
{
    float r = sqrt((p.x - position.x) * (p.x - position.x) + (p.z - position.z) * (p.z - position.z));
    vec3 n = vec3{ p.x - position.x, r * (radius / height), p.z - position.z };
    return n.normalize();
}

vec3 Cone::calculateUVs(vec3& p) const
{
    vec3 pHit = globalToLocal(p);

    float theta = atan2(pHit.x, pHit.z);
    float rawU = theta / (2 * M_PI);
    float u = 1.0f - (rawU + 0.5f);

    float v = fmod(pHit.y, 1);

    float S = 30.f;
    if (radius >= 1)
        S = radius * 10.f;
    else
        S = radius * 30.f;

    return vec3{ u, v, 10.f };
}
