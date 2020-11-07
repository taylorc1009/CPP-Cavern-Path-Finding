#include <iostream>
#include <fstream>
#include <string>
#include <map>
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

void readCAV(char* name, std::vector<Cavern> &caverns) {
	std::string path("cavs/");
	path.append(name);
	path.append(".cav");

	std::ifstream file(path);

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
					caverns.push_back(Cavern(temp, stoi(tok)));
				else
					temp = stoi(tok);
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
		
		//adds each caverns connections to their connections vector
		for (i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if (cons[i][j] == 1 && i != j) //i != j prevents a cavern from being connected to itself, as this could cause the search to fail
					caverns[j].addConnection(i);

		for (i = 0; i < size; i++)
			delete cons[i];
		delete cons;
	}
	else
		std::cout << "(!) failed to open file: " << path;
}

int lowestDistance(std::vector<int> pendingCavs, std::map<int, double> fScore) {
	int cav;
	double lowest = DBL_MAX;
	for (std::vector<int>::iterator unvisited = pendingCavs.begin(); unvisited != pendingCavs.end(); unvisited++) {
		//std::map<int, double>::iterator it = fScore.find(*unvisited);
		//if (it != fScore.end()) {
		if(containsKey(fScore, *unvisited)) {
			if (fScore[*unvisited] < lowest) {
				cav = *unvisited;
				lowest = fScore[*unvisited];
			}
		}
	}
	return cav;
}

double EuclidianDistance(Cavern& current, Cavern& goal) {
	int x = pow((goal.getX() - current.getX()), 2);
	int y = pow((goal.getY() - current.getY()), 2);
	return sqrt(x + y);
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

std::vector<int> AStar(std::vector<Cavern>& caverns, int goal) {
	std::vector<int> searchedCavs, pendingCavs; //used to determine which caverns have been searched and which caverns to search next, based on connections
	std::map<int, int> cameFrom; //signifies where a cavern (int 1) came from (int 2)

	//gScore stores the current lowest distance from the start, to a node
	//fScore stores the estimated distance to traverse a path to the goal from the start
	std::map<int, double> gScore, fScore;

	int currentCavern; //determines the current subject node

	//initialises the lists of values to start at the first cavern
	gScore[0] = 0;
	fScore[0] = EuclidianDistance(caverns[0], caverns[goal]);
	pendingCavs.push_back(0);

	while (pendingCavs.size() > 0) {
		//looks for the cavern in the list of perviously discovered caverns, pending search, with the lowest distance (in fScore)
		currentCavern = lowestDistance(pendingCavs, fScore);

		//exits the search returning the result, if it's found
		if (currentCavern == goal)
			return reconstructPath(cameFrom, currentCavern);

		//removes current subject cavern from the pending cavs 
		pendingCavs.erase(std::remove(pendingCavs.begin(), pendingCavs.end(), currentCavern), pendingCavs.end());
		//then adds the to the list of searched cavs
		searchedCavs.push_back(currentCavern);
		
		//for each connection of the current Cavern object, in caverns[x].connections
		for (std::vector<int>::const_iterator connection = caverns[currentCavern].getConnections().begin(); connection != caverns[currentCavern].getConnections().end(); connection++) {

			//skip this check if the connected path has been previously searched
			if (contains(searchedCavs, *connection))
				continue;

			//creates a temporary distance which stores the distance to a connection of the current cavern from the start
			double gScoreTentative = gScore[currentCavern] + EuclidianDistance(caverns[currentCavern], caverns[*connection]);

			if (!contains(pendingCavs, *connection)) //if the unsearched connection of the current cavern is not pending search, appends it to the list to be searched
				pendingCavs.push_back(*connection);
			else if (gScoreTentative >= gScore[*connection]) //otherwise, check if the distance to the connection through the current path supersedes the pervious shortest path to the connection
				continue; //if not, skip recording of this path

			//if the path is better, or or it hasn't been previously found, its details will be recorded
			cameFrom[*connection] = currentCavern;
			gScore[*connection] = gScoreTentative;
			fScore[*connection] = gScore[*connection] + EuclidianDistance(caverns[*connection], caverns[goal]);
		}
	}

	/*std::vector<int> v = reconstructPath(cameFrom, currentCavern);
	int temp;
	for (std::vector<int>::iterator i = v.begin(); i != v.end(); i++) {
		if (*i != 0)
			std::cout << temp + 1 << " -> " << *i + 1 << ": " << EuclidianDistance(caverns[temp], caverns[*i]) << "\n";
		temp = *i;
	}*/

	//returns an empty vector to 
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
		}*/

		std::vector<int> solution = AStar(reference, reference.size() - 1);

		if (solution.empty()) {
			std::cout << "- search completed: no path found";
			return 0;
		}
		else {
			std::string path(argv[1]);
			path.append(".csn");

			std::ofstream file(path);
			if (file.is_open()) {
				for (std::vector<int>::iterator cavern = solution.begin(); cavern != solution.end(); cavern++)
					file << *cavern + 1 << ' ';
				file.close();
			}
			else
				std::cout << "(!) failed to create/open output file";
		}

		/*for (std::vector<int>::iterator i = solution.begin(); i != solution.end(); i++)
			std::cout << *i + 1 << ' ';*/

		freeCavern(reference);
	}
	else
		std::cout << "(!) invalid number of parameters";
	return 0;
}