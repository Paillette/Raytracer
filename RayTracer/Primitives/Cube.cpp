#include "Cube.h"
#include <algorithm>

Cube::Cube()
    :size(1.0f), Primitive(vec3{ 0, 0, 0 }, new Material("mat", Material::Type::MATTE, vec3{ 1, 1, 1 }, 0.f, 0.f))
{
}

Cube::Cube(const vec3& pos, float s, Material* mat)
    : size(s), Primitive::Primitive(pos, mat)
{
}

float Cube::intersect(const ray& ray) const
{
    vec3 min { position.x - size, position.y - size, position.z - size };
    vec3 max { position.x + size, position.y + size, position.z + size };

    float tmin = (min.x - ray.origin.x) / ray.direction.x;
    float tmax = (max.x - ray.origin.x) / ray.direction.x;

    if (tmin > tmax)
        std::swap(tmin, tmax);

    float tymin = (min.y - ray.origin.y) / ray.direction.y;
    float tymax = (max.y - ray.origin.y) / ray.direction.y;

    if (tymin > tymax) 
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return 0.0f;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (min.z - ray.origin.z) / ray.direction.z;
    float tzmax = (max.z - ray.origin.z) / ray.direction.z;

    if (tzmin > tzmax) 
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return 0.0f;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;
    
    vec3 impact = ray.origin + ray.direction * tmin;

    return tmin;
}

vec3 Cube::calculateNormal(vec3& p) const
{
    vec3 hit = globalToLocal(p);
    hit = hit / size;
    vec3 obs = globalToLocal(vec3{ 0, 0, 0 });
    vec3 dir = hit - obs;

    vec3 n;

    if (hit.x > 0.9999)
        n = vec3{ 1, 0, 0 };
    else if (hit.y > 0.9999)
        n = vec3{ 0, 1, 0 };
    else if (hit.z > 0.9999)
        n = vec3{ 0, 0, 1 };
    else if (hit.x < -0.9999)
        n = vec3{ -1, 0, 0 };
    else if (hit.y < -0.9999)
        n = vec3{ 0, -1, 0 };
    else
        n = vec3{ 0, 0, -1 };

    return n;
}

vec3 Cube::calculateUVs(vec3& p) const
{
    vec3 res = globalToLocal(p);
    res = res / size;

    if (res.x > 0.9999 || res.x < -0.9999)
    {
        res.x = (res.z + 1) / 2;
        res.y = (res.y + 1) / 2;
        res.z = 10.0f;
    }
    else if (res.y > 0.9999 || res.y < -0.9999)
    {
        res.x = (res.x + 1) / 2;
        res.y = (res.z + 1) / 2;
        res.z = 10.0f;
    }
    else if (res.z > 0.9999 || res.z < -0.9999)
    {
        res.x = (res.x + 1) / 2;
        res.y = (res.y + 1) / 2;
        res.z = 10.0f;
    }

    return res;
}
