#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "math.h"
#include "Cavern.h"


template <typename T> //used to declare paramater types from a non-type parameter in the 'contains' and 'containsKey' methods
struct identity { typedef T type; }; //allows an instance of a non-type paramater to be created with a type; in our case, type 'T' will become an instance of an 'int'

template <typename A, typename B> //again, declares non-types, but this time for the map; this time, in our case, it will become 'map<int, double>' from 'map<A, B>
inline bool containsKey(const std::map<A, B> &m, const typename identity<A>::type &val) {
	return m.find(val) != m.end(); //.find will iterate through the map to find a key and return that node, but if a value isn't found then it will return the end of the map has been reached
}

template <typename A> //this time, it performs the same function as for the map but ith one type, as we will be searching a vector here
inline bool contains(const std::vector<A> &v, const typename identity<A>::type &val) {
	return std::find(v.begin(), v.end(), val) != v.end(); //here, find will perform the same function as for map with the same results, only this time we need to explicitly tell it what to iterate (as 'vector' has no .find method)
}

void freeCavern(std::vector<Cavern> &caverns) { //deallocates the memory used to store the caverns
	caverns.clear(); //clears the vector to make sure it's empty before deallocation
	caverns.swap(std::vector<Cavern>(caverns)); //creates an instance of an unallocated vector and replaces ('swap's) 'caverns' with it
	delete &caverns; //finally, deletes 'caverns' by reference
}

void readCAV(char* name, std::vector<Cavern> &caverns) {
	//builds the path to the .cav file - C++ cannot concatenate pure strings (name + ".cav") as, to the compiler, they're initially pointers
	std::string path(std::string(name) + std::string(".cav"));

	std::ifstream file(path); //attempts to open the file

	if (!file.is_open()) //if the file failed to open, checks if it may be in the 'cavs' directory
		file.open(std::string("cavs/") + path);

	if (file.is_open()) { //continues if the file is open
		std::string tok;
		int i = 0, row = 0, col = 0, size = 0, temp = 0;
		int** cons; //this will be used to store the connections as booleans so we can then later store the connections, in the objects, explicitly as the cavern IDs
		bool isY = false; //used to determine if the current coordinate we're attempting to store is y

		while (std::getline(file, tok, ',')) { //tokenizes the .cav file by ','
			if (i > (size * 2) && i <= (size * size) + (size * 2)) { //if the range of values we're currently iterating is between the last coordinate and the end of the file - (size*2)+(size*size)
				cons[row][col] = stoi(tok);
				col++; //iterates along the row
				if (col == size) { //if the current row is finished...
					row++; //move to the next...
					col = 0; //and start at row index (column) 0 again
				}
			}
			else if (i > 0 && i <= size * 2) { //if the range of values we're currently iterating is between the first value (size) and the last coordinate - size*2
				if (isY) //if the current value is y...
					caverns.push_back(Cavern(temp, stoi(tok))); //store it as y and the temporary value as x...
				else
					temp = stoi(tok); //otherwise store x temporarily
				isY = !isY; //switches between the "current value is y" being true or false
			}
			else if (i == 0) { //if the current iteration is the first, this will be the amount of caverns present
				size = stoi(tok);
				caverns.reserve(size); //creates ('reserve's) empty spaces in the list of 'Cavern' objects to store x ('size') amount of values
				cons = new int* [size]; //initialises 'cons' as an array based on the amount of caverns
				for (int j = 0; j < size; j++)
					cons[j] = new int[size]; //stores more arrays in each index of 'cons', essentially creating a matrix
			}
			i++;
		}
		file.close();
		
		//adds each caverns connections to their connections vector
		for (i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if (cons[i][j] == 1 && i != j) //i != j prevents a cavern from being connected to itself, as this could cause the search to fail
					caverns[j].addConnection(i); //adds the connection by cavern ID 'i' if a connection to the current cavern 'j' is 1 (true)

		//deallocates the 'cons' matrix
		for (i = 0; i < size; i++)
			delete cons[i];
		delete cons;
	}
	else
		std::cout << "(!) failed to open file: " << path << "\n";
}

int shortestDistance(std::vector<int> pendingCavs, std::map<int, double> fScore) { //returns the lowest estimated distance to the goal from each cavern in 'pendingCavs'
	int cav;
	double lowest = DBL_MAX;

	for (std::vector<int>::iterator unvisited = pendingCavs.begin(); unvisited != pendingCavs.end(); unvisited++) { //for each unvisited cavern in 'pendingCavs'...
		if(containsKey(fScore, *unvisited)) { //determine if we have an 'fScore' for it yet...
			if (fScore[*unvisited] < lowest) { //if we do and its distance is lower than the current lowest, update lowest to the current cavern
				cav = *unvisited;
				lowest = fScore[*unvisited];
			}
		}
	}

	return cav;
}

double EuclidianDistance(Cavern& current, Cavern& goal) {
	//pow(x, 2) is the same as x^2, but this isn't used in C++ for a number of reasons (https://stackoverflow.com/a/14627505/11136104)
	int x = pow((goal.getX() - current.getX()), 2), y = pow((goal.getY() - current.getY()), 2);
	return sqrt(x + y);
}

std::vector<int> reconstructPath(std::map<int, int> &cameFrom, int &currentCavern) { //rebuilds the final path to be in the order of traversal
	std::vector<int> totalPath;
	totalPath.push_back(currentCavern); //add the current cavern, which will be the goal, to the path

	while (containsKey(cameFrom, currentCavern)) { //while 'cameFrom' contains any cavern preceeding the goal...
		currentCavern = cameFrom[currentCavern]; //step back through the path to the previous connection to the current cavern...
		totalPath.insert(totalPath.begin(), currentCavern); //then insert the previous connection to the start of the list
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

	while (pendingCavs.size() > 0) { //keeps searching as long as we have a possible solution

		currentCavern = shortestDistance(pendingCavs, fScore); //we will search along the path with the currently known shortest distance to the goal

		if (currentCavern == goal) //exits the search returning the result, if it's found
			return reconstructPath(cameFrom, currentCavern);

		pendingCavs.erase(std::remove(pendingCavs.begin(), pendingCavs.end(), currentCavern), pendingCavs.end()); //removes current subject cavern from the pending cavs 
		searchedCavs.push_back(currentCavern); //then adds the to the list of searched cavs
		
		for (std::vector<int>::const_iterator connection = caverns[currentCavern].getConnections().begin(); connection != caverns[currentCavern].getConnections().end(); connection++) { //for each connection of the current Cavern object, in caverns[x].connections

			if (contains(searchedCavs, *connection)) //skip this check if the connected path has been previously searched
				continue;

			double gScoreTentative = gScore[currentCavern] + EuclidianDistance(caverns[currentCavern], caverns[*connection]); //creates a temporary distance which stores the distance to a connection of the current cavern from the start

			if (!contains(pendingCavs, *connection)) //if the unsearched connection of the current cavern is not pending search, appends it to the list to be searched...
				pendingCavs.push_back(*connection);
			else if (gScoreTentative >= gScore[*connection]) //otherwise, check if the distance to the connection through the current path supersedes the pervious shortest path to the connection...
				continue; //if not, skip recording of this path

			//if the path is better, or or it hasn't been previously found, its details will be recorded
			cameFrom[*connection] = currentCavern;
			gScore[*connection] = gScoreTentative;
			fScore[*connection] = gScore[*connection] + EuclidianDistance(caverns[*connection], caverns[goal]);
		}
	}
	return std::vector<int>(); //returns an empty vector to signify an inconclusive search - no path available
}

int main(int argc, char **argv) {
	if (argc == 2) { //determines if we were given a file name

		std::vector<Cavern>* caverns = new std::vector<Cavern>; //declares a vector of type 'Cavern' on the heap
		std::vector<Cavern>& reference = *caverns; //declares a reference to said vector do we can pass a parameter of it by reference

		readCAV(argv[1], reference); //retrieves the values in the file given and stores them in 'caverns' using the reference to it

		if (!reference.empty()) { //continues as long as the file was opened successfully
			std::vector<int> solution = AStar(reference, reference.size() - 1); //utilizes the A* algorithm to try and find a solution to get to the goal cavern

			if (!solution.empty()) { //if the solution is empty, the algorithm found no path to the goal so don't continue

				//builds the file path to output the solution to
				std::string path(std::string(argv[1]) + std::string(".csn"));

				std::ofstream file(path);

				if (file.is_open()) {
					for (std::vector<int>::iterator cavern = solution.begin(); cavern != solution.end(); cavern++) //for every cavern in the solution...
						file << *cavern + 1 << ' '; //output the ID + 1; a list of size 30 would have cavern IDs 0-29
					file.close();
				}
				else
					std::cout << "(!) failed to create/open output file\n";
			}
			else
				std::cout << "(!) search completed: no path found\n";
		}

		freeCavern(reference); //after everything is done, deallocate 'caverns'
	}
	else
		std::cout << "(!) invalid number of parameters\n";
	return 0;
}