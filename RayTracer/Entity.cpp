#include "Entity.h"

Entity::Entity()
    :position(vec3{0, 0, 0}), transform(Matrix4()), transformInv(Matrix4())
{
}

Entity::Entity(vec3 p)
    : position(p)
{
    transform = Matrix4::Translate(p.x, p.y, p.z);
    transformInv = Matrix4::Translate(-p.x, -p.y, -p.z);
}

vec3 Entity::localToGlobal(vec3 pos) const
{
    return transform * pos;
}

vec3 Entity::globalToLocal(vec3 pos) const
{
    return transformInv * pos;
}
