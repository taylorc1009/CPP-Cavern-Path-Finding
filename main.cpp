#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <sstream>
#include "Cavern.h"

using namespace std;

Cavern* readCAV(char* path) {
	//ifstream file (path);
	fstream file(path);
	Cavern* cavern = new Cavern();
	/*if (file.is_open()) {
		string line;
		while (file >> line) {
			while(line.;
		}
	}*/
	std::string token, line;
	std::stringstream convertor(line);
	int tokens = 0;
	bool isY = false;
	while (std::getline(convertor, token, ',')) {
		tokens++;
		if (tokens == 1) { //declares the Cavern object
			cavern->size = std::stoi(token);
			cavern->coords = new int*[cavern->size];
			for (int i = 0; i < cavern->size; i++)
				cavern->coords[i] = new int[2];
			cavern->connections = new int*[cavern->size];
			for (int i = 0; i < cavern->size; i++)
				cavern->coords[i] = new int[cavern->size];
		}
		else if (tokens < (cavern->size * 2) + 1) {
			cavern->coords[isY ? 1 : 0][isY ? tokens - 2 : tokens - 1] = std::stoi(token);
			isY = !isY;
		}
		else
			cavern->connections[tokens]
	}
	return cavern;
}

int main(int argc, char **argv) {
	if(argc == 2)
		readCAV(argv[0]);
	else
		cout << "(!) invalid number of parameters";
	return 0;
}