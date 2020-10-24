#include<iostream>
#include<fstream>
#include "Cavern.h"

using namespace std;

Cavern* readCAV(char* path) {
	ifstream file(path);
	Cavern* cavern = new Cavern();
	if (file.is_open()) {

	}
	return cavern;
}

int main(int argc, char **argv) {
	if(argc == 1)
		readCAV(argv[0]);
	else
		cout << "(!) invalid number of parameters";
	return 0;
}