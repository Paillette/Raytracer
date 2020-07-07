#include "SceneReader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Plane.h"
#include "Sphere.h"
#include "Tri.h"

using namespace std;

void SceneReader::readFile(string filename)
{
	ifstream file;
	//TODO : passer en paramètre ? 
	//file.open(filename);
	file.open(filename);
	if (!file) {
		cout << " !!!! " << endl;
		cout << "Unable to open file " + filename << endl;
		exit(1);   // call system to stop
	}

	string line;
	string word;
	//std::vector<std::string> temp;
	/*
	int i = 0;
	while (file >> word)
	{
		temp.push_back(word);
		cout << temp[i] << endl;
		i++;
	}

	for (int j = 0; j < temp.size(); j++)
	{
		if (temp[j] == "m")
		{
			string name = temp[j + 1];
			Material* name = new Material(Material::Type::MATTE, color{ temp[j + 2] }, true, 0.f, 0.f);
			matList.push_back(name);
		}
			cout << "material";
	}
	*/
	std::vector<std::string> temp;
	int pos = 0;

	while (getline(file, line))
	{
		while ((pos = line.find(' ')) != std::string::npos) 
		{
			word = line.substr(0, pos);
			std::cout << word << std::endl;
			temp.push_back(word);
			line.erase(0, pos + 1);
		}
		//materials
		if (temp[0] == "m")
		{
			Material* mat = new Material(temp[1], Material::Type::MATTE, color{ std::stof(temp[3]),std::stof(temp[4]), std::stof(temp[5]) }, std::stoi(temp[6]), std::stof(temp[7]), std::stof(temp[8]));
			matList.push_back(mat);
			temp.clear();
		}
		
		//primitives
		else if (temp[0] == "p")
		{
			Primitive* prim;
			if (temp[1] == "Plane")
			{
				//std::stof(temp[4]) 
				//prim = new Plane(vec3{ std::stof(temp[2]),std::stof(temp[3]), std::stof(temp[4]) }, vec3{ 0.f, 1.f, 0.f }, matGrey);
			}

			//primivitesList.push_back(prim);
		}
		//lights
		else if (line[0] == 'l')
			cout << line[0] << endl;

	}

	file.close();
}
