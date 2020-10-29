#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <sstream>
#include "Cavern.h"

using namespace std;

void freeCavern(Cavern* cavern) {
	for (int i = 0; i < cavern->size; i++)
		delete[] cavern->connections[i];
	delete[] cavern->connections;
	delete[] cavern->coords[0];
	delete[] cavern->coords[1];
	delete cavern->coords;
	delete cavern;
}

void readCAV(char* path, Cavern* cavern) {
	std::string str(path);
	str.append(".cav");

	ifstream file(str);

	if (file.is_open()) {
		std::string tok;
		int i = 0, row = 0, col = 0;
		bool isY = false;

		while (std::getline(file, tok, ',')) {
			if (i > (cavern->size * 2) && i <= ((cavern->size * cavern->size) + (cavern->size * 2))) {
				cavern->connections[row][col] = stoi(tok);
				row++;
				if (row == cavern->size) {
					col++;
					row = 0;
				}
			}
			else if (i > 0 && i <= (cavern->size * 2)) {
				cavern->coords[row][isY ? 1 : 0] = stoi(tok);
				if (isY)
					row++;
				if (i == (cavern->size * 2))
					row = 0; //resets col for the connections declaration count
				else
					isY = !isY;
			}
			else if (i == 0) { //declares the Cavern object
				cavern->size = stoi(tok);
				cavern->coords = new int* [cavern->size];
				for (int i = 0; i < cavern->size; i++)
					cavern->coords[i] = new int[2];
				cavern->connections = new int* [cavern->size];
				for (int i = 0; i < cavern->size; i++)
					cavern->connections[i] = new int[cavern->size];
			}
			i++;
		}
		file.close();
	}
	else
		cout << "(!) failed to open file: " << str;
}

int main(int argc, char **argv) {
	if (argc == 2) {
		Cavern* cavern = new Cavern();
		readCAV(argv[1], cavern);
		freeCavern(cavern);
	}
	else
		cout << "(!) invalid number of parameters";
	return 0;
}