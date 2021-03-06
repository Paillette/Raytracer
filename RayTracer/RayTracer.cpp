
#include <cmath>
#include <iostream>

#include "Bitmap.h"
#include "Utilities/ray.h"
#include "tracer.h"
#include "Utilities/BRDFs.h"
#include "Utilities/RandomNumbers.h"
#include "Properties.h"
#include "SceneReader.h"

using namespace std;

int main(int argc, const char* argv[])
{
	//Instance singleton properties
	Properties* prop;
	prop = Properties::get();

	int scene = 1;
	string name = prop->getName();
	int height = prop->getHeight();
	int width = prop->getWidth();
	bool shadow = prop->getShadow();
	bool moreOption = false;
	bool AO = prop->getAO();
	int numberOfSamplesAO = Properties::get()->getSampleAO();
	int numberOfSamplesGI = Properties::get()->getSampleGI();
	bool GI = prop->getGI();
	float percent = 0.f;
	bool changeMat = true;
	SceneReader sr;
	//SuperSampling
	int numberOfSamples = Properties::get()->getSampleAA();

	//Ask the user
	while (prop->getReady() == false)
	{
		cout << u8"-----------Welcome to raytracing !-------------" << "\n" << "\n";

		cout << u8"Choose scene to render. Enter a number between 1 and 4" << "\n";
		cin >> scene;
		cout << "\n";
		sr.readFile("Scene\\scene" + std::to_string(scene) + ".txt");

		cout << u8"Choose image name" << "\n";
		cin >> name;
		cout << "\n";

		cout << u8"Height" << "\n";
		cin >> height;
		cout << "\n";

		cout << u8"Width" << "\n";
		cin >> width;
		cout << "\n";

		cout << u8"Render with shadow ?" << "\n" << u8"Yes : 1" << u8"      No : 0     " << "\n";
		cin >> shadow;
		cout << "\n";

		cout << u8"Change material ?" << "\n" << u8"Yes : 1" << u8"      No : 0     " << "\n";
		cin >> changeMat;
		cout << "\n";

		if (changeMat == true)
		{
			for (int i = 0; i < sr.getMaterial().size(); i++)
			{
				bool answer = 0;
				cout << " --->" << sr.getMaterial()[i]->getName() << "\n" << u8"Yes : 1" << u8"      No : 0     " << "\n";
				cin >> answer;
				cout << "\n";
				sr.getMaterial()[i]->setStripeOrColor(answer);
			}
		}

		cout << u8"Want more options ?" << "\n" << u8"Yes : 1" << u8"       No : 0     " << "\n";
		cin >> moreOption;
		cout << "\n";

		if (moreOption == true)
		{
			cout << u8"Render with AO ?" << "\n" << u8"Yes : 1" << u8"      No : 0     " << "\n";
			cin >> AO;
			cout << "\n";

			cout << u8"Number of AO samples " << "\n";
			cin >> numberOfSamplesAO;
			cout << "\n";

			cout << u8"Render with GI ?" << "\n" << u8"Yes : 1" << u8"      No : 0     " << "\n";
			cin >> GI;
			cout << "\n";

			cout << u8"Number of GI samples " << "\n";
			cin >> numberOfSamplesGI;
			cout << "\n";

			cout << u8"Number of samples for AA" << "\n";
			cin >> numberOfSamples;
			cout << "\n";
		}

		cout << "------------------------------------------------" << "\n" << u8"Image ready to render" << "\n";
		prop->setReady(true);
		cout << "------------------------------------------------" << "\n";
	}
	cout << "Image Processing : " << "\n";

	//Set input from users to singleton
	prop->setShadow(shadow);
	prop->setHeight(height);
	prop->setWidth(width);
	prop->setAO(AO);
	prop->setSampleAO(numberOfSamplesAO);
	prop->setSampleAA(numberOfSamples);
	prop->setSampleGI(numberOfSamplesGI);
	prop->setGI(GI);
	prop->setName(name);


	//Image
	Bitmap bmp;
	bmp.SetHeader(width, height);
	float aspectRatio = float(width) / float(height);
	tracer tracer(sr.getPrimitivesList(), sr.getLightList());
	BRDFs clp;
	int nbreOfRaysByPixel;
	int i, j = 0;
	std::vector<std::vector<vec3>> pixels;

	//Jittering
	randomNumbers randomFloat;

	//Raytracing
	for (j = 0; j < height; j++)
	{			
		pixels.resize(width);

		for (i = 0; i < width; i++)
		{

			// Init the pixel to 100% black (no light).
			vec3 col = vec3{ 0.f, 0.f, 0.f };
			//Primary ray generation
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

			col.r = clp.clamp(col.r, 0.f, 1.f);
			col.g = clp.clamp(col.g, 0.f, 1.f);
			col.b = clp.clamp(col.b, 0.f, 1.f);

			//Gamma compression
			float r = int(powf(col.r, 1.f / 2.2f) * 255.99f);
			float g = int(powf(col.g, 1.f / 2.2f) * 255.99f);
			float b = int(powf(col.b, 1.f / 2.2f) * 255.99f);

			//equivalent a typedef vec3 color
			//usign color = vec3
			bmp.OutputColor(r, g, b);
			vec3 pixel = vec3{ r, g, b };

			pixels[i].push_back(pixel);
			percent++;
		
		}

		cout << "\r" << (percent) / (width * height) * 100 << "%";
	}

	bmp.save((prop->getName() + ".jpg").c_str(), FREE_IMAGE_FORMAT::FIF_BMP, prop->getWidth(), prop->getHeight(), pixels);
	cout << "\n" << "Finish !" << "\n";
	return 0;
}