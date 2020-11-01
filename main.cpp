#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<map>
#include<algorithm>
#include "math.h"
#include "Cavern.h"

void freeCavern(std::vector<Cavern> &caverns) {
	for (int i = 0; i < caverns.size(); i++) {
		caverns[i].connections.clear();
		caverns[i].connections.swap(std::vector<int>(caverns[i].connections));
	}
	caverns.clear();
	caverns.swap(std::vector<Cavern>(caverns));
	delete &caverns;
}

double EuclidianDistance(Cavern &current, Cavern &goal) {
	int x = pow((current.getX() + goal.getX()), 2);
	int y = pow((current.getY() + goal.getY()), 2);
	return sqrt(x + y);
}

void readCAV(char* path, std::vector<Cavern> &caverns) {
	std::string str(path);
	str.append(".cav");

	std::ifstream file(str);

	if (file.is_open()) {
		std::string tok;
		int i = 0, row = 0, col = 0, size = 0, temp = 0;
		int** cons;
		bool isY = false;

		while (std::getline(file, tok, ',')) {
			if (i > (size * 2) && i <= ((size * size) + (size * 2))) {
				cons[row][col] = stoi(tok);
				col++;
				if (col == size) {
					row++;
					col = 0;
				}
			}
			else if (i > 0 && i <= (size * 2)) {
				if (isY) {
					caverns.push_back(Cavern(row, temp, stoi(tok)));
					row++;
				}
				else
					temp = stoi(tok);
				if (i == (size * 2)) {
					row = 0; //resets col for the connections declaration count
				}
				else
					isY = !isY;
			}
			else if (i == 0) { //declares the Cavern object
				size = stoi(tok);
				caverns.reserve(size);
				cons = new int* [size];
				for (int j = 0; j < size; j++)
					cons[j] = new int[size];
			}
			i++;
		}
		file.close();
		
		for (i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if (cons[i][j] == 1)
					//cout << j << "row: " << i << "\n";
					caverns[j].connections.push_back(i);
		for (i = 0; i < size; i++)
			delete cons[i];
		delete cons;
	}
	else
		std::cout << "(!) failed to open file: " << str;
}

void AStar(std::vector<Cavern>& caverns, int goal, double estDist) {
	std::vector<int> usedVec, openVec;
	std::map<int, int> cameFrom;
	std::map<int, double> gScore, fScore;
	int currentCavern;

	gScore[caverns[0].id] = 0;
	fScore[caverns[0].id] = estDist;
	openVec.push_back(caverns[0].id);

	//std::cout << "done";

	while (openVec.size() > 0) {
		double lowest = DBL_MAX;

		//looks for the cavern in the list of unsearched caverns with the lowest distance (in fScore)
		for (std::vector<int>::iterator unvisited = openVec.begin(); unvisited != openVec.end(); unvisited++) {
			//for (std::map<int, double>::iterator cavern = fScore.begin(); cavern != fScore.end(); cavern++) {
				//if (*unvisited == cavern->first) {
			std::map<int, double>::iterator it = fScore.find(*unvisited);
			if (it != fScore.end()) {
				if (it->second < lowest) {
					lowest = it->second;
					currentCavern = caverns[it->first].id; // could be just i->first
					std::cout << lowest;
				}
			}
				//}
			//}
		}
		if (currentCavern == goal)
			break;
		openVec.erase(std::remove(openVec.begin(), openVec.end(), currentCavern), openVec.end());
	}
}

/*void readCAV(char* path, Cavern* cavern) { // backup matrix storage system
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
}*/

int main(int argc, char **argv) {
	if (argc == 2) {
		std::vector<Cavern>* caverns = new std::vector<Cavern>;
		std::vector<Cavern>& reference = *caverns;
		readCAV(argv[1], reference);
		/*for (int i = 0; i < reference.size(); i++) {
			for (int j = 0; j < reference[i].connections.size(); j++)
				cout << reference[i].connections[j] << ",";
			cout << "\n";
		}*/
		AStar(reference, reference.size() - 1, EuclidianDistance(reference[0], reference[reference.size() - 1]));
		freeCavern(reference);
		//cout << "done";
	}
	else
		std::cout << "(!) invalid number of parameters";
	return 0;
}