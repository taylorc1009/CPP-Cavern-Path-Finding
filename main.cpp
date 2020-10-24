#include<iostream>
#include<fstream>
#include<string>
#include "Cavern.h"

using namespace std;

Cavern* readCAV(char* path) {
	ifstream file (path);
	Cavern* cavern = new Cavern();
	if (file.is_open()) {
		//string line;
		//while (!file.eof()) {
		//	//file >> line;
		//	line.push_back(file.get());
		//}
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