#include "SceneReader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Plane.h"
#include "Sphere.h"
#include "Tri.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Cone.h"
#include "Cube.h"
#include "InfiniteCylinder.h"
#include "Square.h"

using namespace std;

int returnMatIndex(std::vector<Material*> matList, std::vector<string> temp, int j)
{
	int index = 0;
	//getMaterial
	for (int i = 0; i < matList.size(); i++)
	{
		if (temp[j] == matList[i]->getName())
		{
			index = i;
			break;
		}
	}
	return index;
}

void SceneReader::readFile(string filename)
{
	ifstream file;
	file.open(filename);
	if (!file) {
		cout << " !!!! " << endl;
		cout << "Unable to open file " + filename << endl;
		exit(1);   // call system to stop
	}

	string line;
	string word;
	std::vector<std::string> temp;
	int pos = 0;
	int k = 0;

	while (getline(file, line))
	{
		while ((pos = line.find(' ')) != std::string::npos) 
		{
			word = line.substr(0, pos);
			temp.push_back(word);
			line.erase(0, pos + 1);
		}
		//materials
		if (temp[0] == "m")
		{
			Material* mat;

			if (temp[2] == "MATTE")
			{
				mat = new Material(temp[1], Material::Type::MATTE, color{ std::stof(temp[3]),std::stof(temp[4]), std::stof(temp[5]) }, std::stoi(temp[6]), std::stof(temp[7]), std::stof(temp[8]));
			}
			else if(temp[2] == "METALLIC")
				mat = new Material(temp[1], Material::Type::METALLIC, color{ std::stof(temp[3]),std::stof(temp[4]), std::stof(temp[5]) }, std::stoi(temp[6]), std::stof(temp[7]), std::stof(temp[8]));

			matList.push_back(mat);
			temp.clear();
			k++;
		}
		
		//primitives
		else if (temp[0] == "p")
		{
			Primitive* prim;
			int index = 0;

			if (temp[1] == "Plane")
			{
				index = returnMatIndex(matList, temp, 8);
				prim = new Plane(vec3{ std::stof(temp[2]),std::stof(temp[3]), std::stof(temp[4]) }, vec3{ std::stof(temp[5]),std::stof(temp[6]), std::stof(temp[7]) }, matList[index]);
			}
			else if (temp[1] == "Sphere")
			{
				index = returnMatIndex(matList, temp, 6);
				prim = new Sphere(vec3{ std::stof(temp[2]),std::stof(temp[3]), std::stof(temp[4]) }, std::stof(temp[5]), matList[index]);
			}
			else if (temp[1] == "Tri")
			{
				index = returnMatIndex(matList, temp, 6);
				prim = new Tri(std::stof(temp[2]), vec3{ std::stof(temp[3]),std::stof(temp[4]), std::stof(temp[5])}, matList[index]);
			}
			else if (temp[1] == "Cone")
			{
				index = returnMatIndex(matList, temp, 7);
				prim = new Cone( vec3{ std::stof(temp[2]), std::stof(temp[3]), std::stof(temp[4]) }, std::stof(temp[5]), std::stof(temp[6]), matList[index]);
			}
			else if (temp[1] == "Cube")
			{
				index = returnMatIndex(matList, temp, 6);
				prim = new Cube(vec3{ std::stof(temp[2]), std::stof(temp[3]), std::stof(temp[4]) }, std::stof(temp[5]), matList[index]);
			}
			else if (temp[1] == "Cylinder")
			{
				index = returnMatIndex(matList, temp, 6);
				prim = new InfiniteCylinder(vec3{ std::stof(temp[2]), std::stof(temp[3]), std::stof(temp[4]) }, std::stof(temp[5]), matList[index]);
			}
			else if (temp[1] == "Square")
			{
				index = returnMatIndex(matList, temp, 9);
				prim = new Square(vec3{ std::stof(temp[2]), std::stof(temp[3]), std::stof(temp[4]) }, vec3{ std::stof(temp[5]), std::stof(temp[6]), std::stof(temp[7]) }, std::stof(temp[8]), matList[index]);
			}

			primivitesList.push_back(prim);
			temp.clear();
		}

		//lights
		else if (temp[0] == "l")
		{
			Light* light;

			if (temp[1] == "DirectionLight")
			{
				light = new DirectionLight(vec3{ std::stof(temp[2]),std::stof(temp[3]), std::stof(temp[4]) }.normalize(), color{ std::stof(temp[5]), std::stof(temp[6]), std::stof(temp[7]) }, std::stof(temp[8]));
			}
			if (temp[1] == "PointLight")
			{
				light = new PointLight(vec3{ std::stof(temp[2]),std::stof(temp[3]), std::stof(temp[4]) }.normalize(), color{ std::stof(temp[5]), std::stof(temp[6]), std::stof(temp[7]) }, std::stof(temp[8]));
			}
				
			lightsList.push_back(light);
			temp.clear();
		}
	}

	file.close();
}
