#include "SceneReader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
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

	string word;
	
	while (file >> word)
	{
		if(word == "m")
			cout << word << endl;
		if (word == "p")
			cout << "primitive" << endl;
		if (word == "l")
			cout << "light" << endl;

	}

}
