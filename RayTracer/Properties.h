#pragma once
#include <iostream>

using namespace std;

class Properties
{
	static Properties* singleton;
	Properties(): width(500), height(500), name("Rendu"), shadow(true), AO(true), numberOfSampleAO(10), GI(true), numberOfSampleAA(4), numberOfSampleGI(10), readyToRender(false){}

	//basic properties
	int width;
	int height;
	string name;
	bool shadow;

	//more properties
	bool AO;
	int numberOfSampleAO;
	bool GI;
	int numberOfSampleGI;
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
	int getSampleAO() const { return numberOfSampleAO; }
	bool getGI() const { return GI; }
	int getSampleGI() const { return numberOfSampleGI; }
	int getSampleAA() const{ return numberOfSampleAA; }
	bool getReady() const { return readyToRender; }

 	int setWidth(int w) { return width = w; }
	int setHeight(int h) { return height = h; }
	string setName(string n) { return name = n; }
	bool setShadow(bool s) { return shadow = s; }
	bool setAO(bool ao) { return AO = ao; }
	int setSampleAO(int n) { return numberOfSampleAO = n; }
	bool setGI(bool gi) { return GI = gi; }
	int setSampleGI(int n) { return numberOfSampleGI = n; }
	int setSampleAA(int n) { return numberOfSampleAA = n; }
	bool setReady(bool r) { return readyToRender = r; }

	~Properties() {
		delete singleton;
	}
};