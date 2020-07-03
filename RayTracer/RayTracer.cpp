
#include <cmath>
#include <iostream>

#include "Bitmap.h"
#include "ray.h"
#include "tracer.h"
#include "BRDFs.h"
#include "RandomNumbers.h"

int main(int argc, const char* argv[])
{
	const int width = 1920;// 800;
	const int height = 1080;// 600;

	Bitmap bmp;
	bmp.SetHeader(width, height);
	float aspectRatio = float(width) / float(height);
	tracer tracer;
	BRDFs clp;
	int nbreOfRaysByPixel;

	//SuperSampling
	int numberOfSamples = 1;

	//Jittering
	randomNumbers randomFloat;

	for (int j = 0; j < height; j++)
	{			
		//convertion des coordonnées des pixels height en World 
		//float y = -2.f * (float(j - height / 2) + 0.5f) / height;

		for (int i = 0; i < width; i++)
		{
			//convertion des coordonnées des pixels width en World 
			//float x = aspectRatio * 2.f * (float(i - width / 2) + 0.5f) / width;

			// Init the pixel to 100% black (no light).
			vec3 col = vec3{ 0.f, 0.f, 0.f };
			//Generation du rayon primaire
			ray ray;
			ray.origin = { 0.f, 0.f, 0.f };

			// Accumulate light for N samples.
			for (int sample = 0; sample < numberOfSamples; ++sample)
			{
				//Supersampling with jittering
				float x = aspectRatio * 2.f * (float((i + randomFloat.random_float()) - (width / 2)) + 0.5f) / width;
				float y = -2.f * (float((j + randomFloat.random_float()) - (height / 2)) + 0.5f) / height;

				ray.direction = { x, y, 1.f };
				ray.direction.normalize();

				col += vec3{ tracer.trace(ray) };
			}
			
			// Get the average
			col = col / numberOfSamples;

			// Get the average.
			//image[width * j + i] /= 4.0;

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