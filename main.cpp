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
	/*for (int i = 0; i < caverns.size(); i++)
		caverns[i].deleteConnections();*/
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
				if (isY)
					/*caverns.push_back(Cavern(row, temp, stoi(tok)));
					row++;*/
					caverns.push_back(Cavern(temp, stoi(tok)));
				else
					temp = stoi(tok);
				/*if (i == (size * 2))
					row = 0; //resets col count for the connections acquisition
				else*/
					isY = !isY;
			}
			else if (i == 0) {
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
					caverns[j].addConnection(i);

		for (i = 0; i < size; i++)
			delete cons[i];
		delete cons;
	}
	else
		std::cout << "(!) failed to open file: " << str;
}

std::vector<int> reconstructPath(std::map<int, int> &cameFrom, int &currentCavern) {
	std::vector<int> totalPath;
	totalPath.push_back(currentCavern);
	while (containsKey(cameFrom, currentCavern)) {
		currentCavern = cameFrom[currentCavern];
		totalPath.insert(totalPath.begin(), currentCavern);
	}
	return totalPath;
}

std::vector<int> AStar(std::vector<Cavern>& caverns, int goal, double estDist) {
	std::vector<int> usedVec, openVec;
	std::map<int, int> cameFrom;
	std::map<int, double> gScore, fScore;
	int currentCavern;
	bool found = false;

	gScore[0] = 0;
	fScore[0] = estDist;
	openVec.push_back(0);

	while (openVec.size() > 0) {
		double lowest = DBL_MAX;

		//looks for the cavern in the list of unsearched caverns with the lowest distance (in fScore)
		for (std::vector<int>::iterator unvisited = openVec.begin(); unvisited != openVec.end(); unvisited++) {
			std::map<int, double>::iterator it = fScore.find(*unvisited);
			if (it != fScore.end()) {
				if (it->second < lowest) {
					currentCavern = it->first;
					lowest = it->second;
				}
			}
		}

		//exits the search if a result is found
		if (currentCavern == goal) {
			found = true;
			break;
		}

		//moves current subject cavern from the to the searched 
		openVec.erase(std::remove(openVec.begin(), openVec.end(), currentCavern), openVec.end());
		usedVec.push_back(currentCavern);
		
		//for each connection of the current Cavern object in caverns[x].connections
		for (std::vector<int>::const_iterator connection = caverns[currentCavern].getConnections().begin(); connection != caverns[currentCavern].getConnections().end(); connection++) {
			//skip this check if the connection has been previously searched
			if (contains(usedVec, *connection))
				continue;

			double gScoreTentative = gScore[currentCavern] + EuclidianDistance(caverns[currentCavern], caverns[*connection]);
			//std::cout << "--" << gScoreTentative << " < " << gScore.find(*connection)->second << "? --";
			if (!contains(openVec, *connection))
				openVec.push_back(*connection);
			else if (gScoreTentative >= gScore[*connection])
				continue;
			cameFrom[*connection] = currentCavern;
			gScore[*connection] = gScoreTentative;
			fScore[*connection] = gScore[*connection] + EuclidianDistance(caverns[*connection], caverns[goal]);
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

		/*std::cout << reference.size() << '\n';
		for (int i = 0; i < reference.size(); i++)
			std::cout << "(" << reference[i].getX() << "," << reference[i].getY() << ")\n";
		for (int i = 0; i < reference.size(); i++) {
			for (int j = 0; j < reference[i].getConnections().size(); j++)
				std::cout << reference[i].getConnection(j) << ",";
			std::cout << "\n";
		}

		std::cout << "\n\n --- A* --- \n\n";*/

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