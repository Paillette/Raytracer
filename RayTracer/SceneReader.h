#pragma once
#include <fstream>
#include "Primitive.h"
#include "Light.h"
#include <vector>
using namespace std;

class SceneReader
{
private:
	std::vector<Material> matList;
	std::vector<Primitive*> primivitesList;
	std::vector<Light*> lightsList;

public:
	SceneReader() {}
	void readFile(string filename);
	std::vector<Primitive*> getPrimitivesList() { return primivitesList; }
	std::vector<Light*> getLightList() { return lightsList; }
};

