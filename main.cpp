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
	fstream file(path);

	if (file.is_open()) {
		std::string tok;
		int i = 0, row = 0, col = 0;
		bool isY = false;

		while (std::getline(file, tok, ',')) {
			if (i > (cavern->size * 2) && i <= ((cavern->size * cavern->size) + (cavern->size * 2))) {
				/*if(col == 0)
					cout << "\n";
				else
					cout << col;*/
				cavern->connections[row][col] = std::stoi(tok);
				//cout << cavern->connections[row][col];
				col++;
				if (col == cavern->size) {
					row++;
					col = 0;
					//cout << "\n";
				}
			}
			else if (i > 0 && i <= (cavern->size * 2) + 1) {
				cavern->coords[isY ? 1 : 0][col] = std::stoi(tok);
				if (isY)
					col++;
				if (i == (cavern->size * 2))
					col = 0; //resets col for the connections declaration count
				else
					isY = !isY;
			}
			else if (i == 0) { //declares the Cavern object
				cavern->size = std::stoi(tok);
				cout << cavern->size << "\n";
				cavern->coords = new int* [cavern->size];
				for (int i = 0; i < cavern->size; i++)
					cavern->coords[i] = new int[2];
				cavern->connections = new int* [cavern->size];
				for (int i = 0; i < cavern->size; i++)
					cavern->connections[i] = new int[cavern->size];
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

	/*cout << "\n";
	for (int i = 0; i < cavern->size; i++) {
		for (int j = 0; j < cavern->size; j++)
			cout << cavern->connections[i][j];
		cout << "\n";
	}
	cout << "done";*/
}

int main(int argc, char **argv) {
	if (argc == 2) {
		Cavern* cavern = new Cavern();
		readCAV(argv[1], cavern);
		/*for (int i = 0; i < cavern->size; i++) {
			for (int j = 0; j < cavern->size; j++)
				cout << cavern->connections[i][j];
		}*/
		freeCavern(cavern);
	}
	else
		cout << "(!) invalid number of parameters";
	return 0;
}