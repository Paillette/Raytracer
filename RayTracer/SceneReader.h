#pragma once
#include <fstream>
#include "Primitives/Primitive.h"
#include "Lights/Light.h"
#include "Material/Material.h"
#include <vector>
using namespace std;

class SceneReader
{
private:
	std::vector<Material*> matList;
	std::vector<Primitive*> primivitesList;
	std::vector<Light*> lightsList;

public:
	SceneReader() {}
	void readFile(string filename);
	std::vector<Primitive*> getPrimitivesList() { return primivitesList; }
	std::vector<Light*> getLightList() { return lightsList; }
	std::vector<Material*> getMaterial() { return matList; }
};

