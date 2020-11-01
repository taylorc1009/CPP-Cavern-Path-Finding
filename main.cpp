#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<map>
#include<algorithm>
#include "math.h"
#include "Cavern.h"

template <typename T>
struct identity { typedef T type; };

template <typename A, typename B>
inline bool containsKey(const std::map<A, B> &m, const typename identity<A>::type &val) {
	return m.find(val) != m.end();
}

template <typename A>
inline bool contains(const std::vector<A> &v, const typename identity<A>::type &val) {
	return std::find(v.begin(), v.end(), val) != v.end();
}

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

std::vector<int> reconstructPath(std::map<int, int> &cameFrom, int &currentCavern) {
	/*for (std::map<int, int>::iterator it = cameFrom.begin(); it != cameFrom.end(); it++)
		std::cout << it->first << "," << it->second << "\n";*/

	std::vector<int> totalPath;
	totalPath.push_back(currentCavern);

	/*for (std::vector<int>::iterator it = totalPath.begin(); it != totalPath.end(); it++)
		std::cout << *it << "\n";*/

	bool c = true;
	while (containsKey(cameFrom, currentCavern)) {
		/*std::cout << "i - ";
		std::cout << cameFrom[0] << "\n";*/

		/*std::map<int, int>::iterator it = cameFrom.find(currentCavern);

		

		if (it != cameFrom.end()) {*/
			currentCavern = cameFrom[currentCavern];
			totalPath.insert(totalPath.begin(), currentCavern);
			//std::cout << currentCavern << "\n";
		/*}
		else
			c = false;*/
	}
	//std::reverse(totalPath.begin(), totalPath.end());
	/*for (std::vector<int>::iterator i = totalPath.begin(); i != totalPath.end(); i++)
		std::cout << *i << '\n';*/

	return totalPath;
}

std::vector<int> AStar(std::vector<Cavern>& caverns, int goal, double estDist) {
	std::vector<int> usedVec, openVec;
	std::map<int, int> cameFrom;
	std::map<int, double> gScore, fScore;
	int currentCavern;
	bool found = false;

	gScore[caverns[0].id] = 0;
	fScore[caverns[0].id] = estDist;
	openVec.push_back(caverns[0].id);

	//std::cout << "done";

	while (openVec.size() > 0) {
		double lowest = DBL_MAX;

		//looks for the cavern in the list of unsearched caverns with the lowest distance (in fScore)
		for (std::vector<int>::iterator unvisited = openVec.begin(); unvisited != openVec.end(); unvisited++) {
			/*for (std::map<int, double>::iterator cavern = fScore.begin(); cavern != fScore.end(); cavern++) {
				if (*unvisited == cavern->first) {*/
			std::map<int, double>::iterator it = fScore.find(*unvisited);
			if (it != fScore.end()) {
				if (it->second < lowest) {
					lowest = it->second;
					currentCavern = it->first; // could be just i->first
					//std::cout << lowest;
				}
			}
				/*}
			}*/
		}

		if (currentCavern == goal) {
			found = true;
			break;
		}

		openVec.erase(std::remove(openVec.begin(), openVec.end(), currentCavern), openVec.end());
		usedVec.push_back(currentCavern);
		
		for (std::vector<int>::iterator neighbour = caverns[currentCavern].connections.begin(); neighbour != caverns[currentCavern].connections.end(); neighbour++) {
			if (contains(usedVec, *neighbour))
				continue;

			//std::cout << *neighbour << '\n';

			/*for (std::vector<int>::iterator i = usedVec.begin(); i != usedVec.end(); i++)
				std::cout << *i;*/
			//std::cout << '\n';

			double gScoreTentative = gScore.find(currentCavern)->second + EuclidianDistance(caverns[currentCavern], caverns[*neighbour]);
			//std::cout << "--" << gScoreTentative << " < " << gScore.find(*neighbour)->second << "? --";
			if (!contains(openVec, *neighbour))
				openVec.push_back(*neighbour);
			else if (gScoreTentative >= gScore.find(*neighbour)->second)
				continue;
			cameFrom[*neighbour] = currentCavern;
			gScore[*neighbour] = gScoreTentative;
			fScore[*neighbour] = gScore.find(*neighbour)->second + EuclidianDistance(caverns[*neighbour], caverns[goal]);
			/*if (gScoreTentative < gScore.find(*neighbour)->second) {
				cameFrom[*neighbour] = currentCavern;
				gScore[*neighbour] = gScoreTentative;
				fScore[*neighbour] = gScore.find(*neighbour)->second + estDist;
				if (!contains(openVec, *neighbour))
					openVec.push_back(*neighbour);
			}*/
		}

	}
	//std::cout << "\nfound = " << found;
	if (found)
		return reconstructPath(cameFrom, currentCavern);
	else
		return std::vector<int>();
}

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
		std::vector<int> path = AStar(reference, reference.size() - 1, EuclidianDistance(reference[0], reference[reference.size() - 1]));
		for (std::vector<int>::iterator i = path.begin(); i != path.end(); i++)
			std::cout << *i << ' ';
		freeCavern(reference);
		//std::cout << "done";
	}
	else
		std::cout << "(!) invalid number of parameters";
	return 0;
}