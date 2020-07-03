
#include <cmath>
#include <iostream>

#include "Bitmap.h"
#include "ray.h"
#include "tracer.h"
#include "BRDFs.h"

int main(int argc, const char* argv[])
{
	const int width = 800;
	const int height = 600;

	Bitmap bmp;
	bmp.SetHeader(width, height);
	float aspectRatio = float(width) / float(height);
	tracer tracer;
	BRDFs clp;
	int nbreOfRaysByPixel;

	for (int j = 0; j < height; j++)
	{			
		//convertion des coordonnées des pixels height en World 
		float y = -2.f * (float(j - height / 2) + 0.5f) / height;

		for (int i = 0; i < width; i++)
		{
			//convertion des coordonnées des pixels width en World 
			float x = aspectRatio * 2.f * (float(i - width / 2) + 0.5f) / width;

			//Generation du rayon primaire
			ray ray;
			ray.origin = { 0.f, 0.f, 0.f };
			ray.direction = { x, y, 1.f };
			ray.direction.normalize();

			color col = tracer.trace(ray);
			col.r = clp.clamp(col.r, 0.f, 1.f);
			col.g = clp.clamp(col.g, 0.f, 1.f);
			col.b = clp.clamp(col.b, 0.f, 1.f);

			//compression gamma des couleurs linéaires
			//pour le stockage en 8bits RGB
			int r = int(powf(col.r, 1.f/2.2f) * 255.99f);
			int g = int(powf(col.g, 1.f/2.2f) *255.99f);
			int b = int(powf(col.b, 1.f/2.2f) * 255.99f);
			
			//equivalent a typedef vec3 color
			//usign color = vec3
			bmp.OutputColor(r, g, b);
		}
	}

	return 0;
}