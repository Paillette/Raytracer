#include "Tri.h"
#include "vec3.h"

Tri::Tri(float r, const vec3& pos, Material* mat)
	: radius(r), Primitive::Primitive(pos, mat)
{
}

float Tri::intersect(const ray& ray) const
{
	float t = 1;

    vec3 v0 = vec3{ position.x, position.y + radius/4, position.z };
    vec3 v1 = vec3{ position.x - radius / 4, position.y - radius / 4,  position.z };
    vec3 v2 = vec3{ position.x + radius/4, position.y - radius/4,  position.z  };

	vec3 v0v1 = v1 - v0;
	vec3 v0v2 = v2 - v0;
	vec3 N = v0v1^(v0v2);

	float NdotRayDirection = N.dot(ray.direction);
	if (fabs(NdotRayDirection) < 0.0001)
		return 0; 

	float d = N.dot(v0);
	t = (N.dot(ray.origin) + d) / NdotRayDirection;
	if (t < 0) t = 0;

    vec3 P = ray.origin + ray.direction * t;
    vec3 C;

    vec3 edge0 = v1 - v0;
    vec3 vp0 = P - v0;
    C = edge0^(vp0);
    if (N.dot(C) < 0) t = 0; 

    // edge 1
    vec3 edge1 = v2 - v1;
    vec3 vp1 = P - v1;
    C = edge1^(vp1);
    if (N.dot(C) < 0)  t = 0;

    // edge 2
    vec3 edge2 = v0 - v2;
    vec3 vp2 = P - v2;
    C = edge2^(vp2);
    if (N.dot(C) < 0) t = 0; 

	return t;
}

vec3 Tri::calculateNormal(const vec3& p) const
{
    vec3 v0 = vec3{ position.x, position.y + radius / 4,  position.z };
    vec3 v1 = vec3{ position.x - radius / 4, position.y - radius / 4,  position.z };
    vec3 v2 = vec3{ position.x + radius / 4, position.y - radius / 4,  position.z };

    vec3 v0v1 = v1 - v0;
    vec3 v0v2 = v2 - v0;

	return v0v2^v0v1;
}

vec3 Tri::calculateUVs(const vec3& p) const
{
    //TODO: Faire les UVS
    return vec3();
}
