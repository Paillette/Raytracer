#include "Material.h"

Material::Material(string n, Type t, vec3 col, bool StripeOrColor, float indice, float rough, const string s)
	: name(n), type(t), color(col), stripe(StripeOrColor), IOR(indice), glossiness(rough)
{
	if (s == "N")
		tex = nullptr;
	else
		tex = new Texture(("Textures/" + s).c_str());
}

vec3 Material::getColor(vec3 uvS) const
{
	if (tex != nullptr)
	{
		return getColorInTexture(uvS);
	}
	else if (stripe)
	{
		float patternU = fmod(uvS.x * uvS.z, 1) < 0.5;
		return color * patternU;
	}
	else
		return color;
}

vec3 Material::getColorInTexture(vec3 uv) const
{
	int u = int(uv.x * (tex->getWidth() - 1));
	int v = int(uv.y * (tex->getHeight() - 1));
	
	vec3 col = tex->getPixelColor(u, v);
	col.r /= 255.0f;
	col.g /= 255.0f;
	col.b /= 255.0f;

	return  col;
}
