#pragma once
#include <FreeImage.h>
#include <vector>
#include "..\Utilities\vec3.h"

using namespace std;

class Texture {

	bool successfullLoad = false;
	FREE_IMAGE_FORMAT baseFormat; //Base image format
	vector<vector<color>> m_pixels; //Array of RGB pixels
	int m_width; //Width
	int m_height; //Height

public:
	Texture(const char* filename);
	
	int loadFromFile(const char* filename);

	int getHeight() const { return m_height; }
	int getWidth() const { return m_width; }
	color getPixelColor(int x, int y) const { return m_pixels[x][y]; }
};