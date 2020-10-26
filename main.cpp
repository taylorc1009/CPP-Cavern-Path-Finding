#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <sstream>
#include "Cavern.h"

using namespace std;

Cavern* readCAV(char* path) {
	fstream file(path);
	Cavern* cavern = new Cavern();

	if (file.is_open()) {
		std::string tok;
		int i = 0, row = 0, col = 0;
		bool isY = false;

		while (std::getline(file, tok, ',')) {
			if (i > (cavern->size * 2) && i <= (cavern->size * cavern->size)) {
				cavern->connections[row][col] = std::stoi(tok);
			}
			else if (i > 0 && i <= (cavern->size * 2)) {
				cavern->coords[isY ? 1 : 0][col] = std::stoi(tok);
				if (isY)
					col++;
				if (i == (cavern->size * 2) + 1)
					col = 0; //resets col for the connections declaration count
				isY = !isY;
			}
			else if (i == 0) { //declares the Cavern object
				cavern->size = std::stoi(tok);
				cavern->coords = new int* [cavern->size];
				for (int i = 0; i < cavern->size; i++)
					cavern->coords[i] = new int[2];
				cavern->connections = new int* [cavern->size];
				for (int i = 0; i < cavern->size; i++)
					cavern->coords[i] = new int[cavern->size];
			}
			i++;
		}
	}
	else
		cout << "(!) file failed to open: " << path;
	//cout << cavern->size;
	//std::cin.ignore();
	//string y;
	//while (true) {
	//	cout << "\nWhich y?";
	//	std::getline(std::cin, y);
	//	cout << "(" << cavern->coords[0][std::stoi(y)] << "," << cavern->coords[1][std::stoi(y)] << ")";
	//}
	///*for (int i = 0; i < cavern->size; i++)
	//	cout << "(" << cavern->coords[0][i] << "," << cavern->coords[1][i] << "), ";
	//std::cin.ignore();*/
	return cavern;
}

int main(int argc, char **argv) {
	if(argc == 2)
		readCAV(argv[1]);
	else
		cout << "(!) invalid number of parameters";
	return 0;
}