#pragma once
#include <iostream>

using namespace std;

class Properties
{
	static Properties* singleton;
	Properties(): width(500), height(500), name("Rendu"), shadow(true), AO(true), GI(true), numberOfSampleAA(4), readyToRender(false){}

	//basic properties
	int width;
	int height;
	string name;
	bool shadow;

	//more properties
	bool AO;
	bool GI;
	int numberOfSampleAA;
	
	bool readyToRender;

public:
	Properties(const Properties&) = delete;
	Properties& operator=(const Properties&) = delete;

	static Properties* get() {
		if (!singleton)
			singleton = new Properties;
		return singleton;
	}

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	string getName() const { return name; }
	bool getShadow() const { return shadow; }
	bool getAO() const { return AO; }
	bool getGI() const { return GI; }
	int getSampleAA() const{ return numberOfSampleAA; }
	bool getReady() const { return readyToRender; }

 	int setWidth(int w) { return width = w; }
	int setHeight(int h) { return height = h; }
	string setName(string n) { return name = n; }
	bool setShadow(bool s) { return shadow = s; }
	bool setAO(bool ao) { return AO = ao; }
	bool setGI(bool gi) { return GI = gi; }
	int setSampleAA(int n) { return numberOfSampleAA = n; }
	bool setReady(bool r) { return readyToRender = r; }

	~Properties() {
		delete singleton;
	}
};