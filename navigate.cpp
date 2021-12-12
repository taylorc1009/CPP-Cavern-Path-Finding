#include "math.h"
#include "data.h"

bool shortestDistance(std::vector<std::shared_ptr<Cavern>> **caverns, std::shared_ptr<Cavern>& current) { //returns the lowest estimated distance to the goal from each cavern that is pending search
	double shortest = DBL_MAX;
	current = nullptr;
	
	for (std::vector<std::shared_ptr<Cavern>>::iterator cavern = (*caverns)->begin(); cavern != (*caverns)->end(); cavern++) { //for each cavern in 'caverns'...
		if((*cavern).get()->isPending()) { //determine if the current iteration is pending search...
			if ((*cavern).get()->fScoreGet() < shortest) { //if it is and its distance is lower than the current lowest, update the lowest to the current cavern
				current = *cavern;
				shortest = (*cavern).get()->fScoreGet();
			}
		}
	}

	return current != nullptr; //if 'cavern' is still a 'nullptr', we didn't find any caverns pending search and thus, the search found no path
}

double EuclidianDistance(Cavern* current, Cavern* goal) {
	//pow(x, 2) is the same as x^2, but this operator isn't used in C++ for a number of reasons (https://stackoverflow.com/a/14627505/11136104)
	int x = pow((goal->getX() - current->getX()), 2), y = pow((goal->getY() - current->getY()), 2);
	return sqrt(x + y);
}

std::vector<int> reconstructPath(std::shared_ptr<Cavern> &current) { //rebuilds the final path to be in the order of traversal from start to the goal
	std::vector<int> totalPath;
	totalPath.push_back(current.get()->getID()); //add the current cavern, which will be the goal, to the path

	while (current.get()->getParent() != nullptr) { //while we have a parent (previous) cavern to get to...
		current = current.get()->getParent(); //change the current cavern to the parent...
		totalPath.insert(totalPath.begin(), current.get()->getID()); //then insert the current cavern to the start of the vector
	}

	return totalPath;
}

std::vector<int> AStar(std::vector<std::shared_ptr<Cavern>> *caverns, int goal) {
	std::shared_ptr<Cavern> current; //determines the current subject cavern
	
	//initialises the starting cavern's values
	(*caverns)[0].get()->setPending();
	(*caverns)[0].get()->gScoreSet(0);
	(*caverns)[0].get()->fScoreSet(EuclidianDistance((*caverns)[0].get(), (*caverns)[goal].get()));

	while (shortestDistance(&caverns, current)) { //as long as we have a cavern to search (shortest path doesn't set 'current' to nothing), we will search along the path with the currently known shortest distance to the goal
		if (current.get()->getID() == goal) //exits the search by returning the path result, if it's found
			return reconstructPath(current);
		
		current.get()->setSearched(); //declare that the current cavern has been searched, then search it's connections

		for (std::vector<std::shared_ptr<Cavern>>::iterator connection = current.get()->getConnections().begin(); connection != current.get()->getConnections().end(); connection++) { //for each connection of the current cavern, in caverns[x].connections
			if ((*connection).get()->hasBeenSearched()) //skip this check if the connected path has been previously searched
				continue;
			
			double gScoreTentative = current->gScoreGet() + EuclidianDistance(current.get(), (*connection).get()); //creates a temporary distance which stores the distance to this connection of the current cavern, from the start
			
			if (gScoreTentative >= (*connection)->gScoreGet()) //check if the distance to this connection through the current path supersedes the pervious shortest path found to this connection...
				continue; //if not, skip recording of this path

			(*connection).get()->setPending(); //declare this cavern as pending a search

			//records the details of this new shortest path found
			(*connection).get()->setParent(current);
			(*connection).get()->gScoreSet(gScoreTentative);
			(*connection).get()->fScoreSet((*connection).get()->gScoreGet() + EuclidianDistance((*connection).get(), (*caverns)[goal].get()));
		}
	}

	return std::vector<int>(); //returns an empty vector to signify an inconclusive search - no path available
}

int main(int argc, char **argv) {
	if (argc == 2) { //determines if we were given a file name
		std::vector<std::shared_ptr<Cavern>> caverns; //declares a vector of pointers to Cavern objects;
		//'shared_ptr' allows us to store multiple copies of the same pointer to the same object - we will need to access the caverns from more than one place, such as the list above and each cavern's list connections
		//we use a shared_ptr because it is unwise to use a vector<Cavern*>, as it quickly becomes difficult to understand ownership of the pointer as we use it through more and more references

		readCAV(argv[1], &caverns); //retrieves the values in the file given and stores them in 'caverns' using the reference to it

		if (!caverns.empty()) { //continues as long as the file was opened successfully
			std::vector<int> solution = AStar(&caverns, caverns.end()->get()->getID()); //utilizes the A* algorithm to try and find a solution to get to the goal cavern

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
	}
	else
		std::cout << "(!) invalid number of parameters\n";
	return 0;
}