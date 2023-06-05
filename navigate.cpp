#include "math.h"
#include "data.h"

bool shortestDistance(const std::vector<std::shared_ptr<Cavern>>& __restrict caverns, std::shared_ptr<Cavern>& __restrict current) { //returns the lowest estimated distance to the goal from each cavern that is pending search
	double shortest = DBL_MAX;
	current = nullptr;
	
	for (std::shared_ptr<Cavern> cavern : caverns) { //for each cavern in 'caverns'...
		if(cavern->isPending()) { //determine if the current iteration is pending search...
			if (cavern->fScoreGet() < shortest) { //if it is and its distance is lower than the current lowest, update the lowest to the current cavern
				current = cavern;
				shortest = cavern->fScoreGet();
			}
		}
	}

	return current != nullptr; //if 'cavern' is still a 'nullptr', we didn't find any caverns pending search and thus, the search found no path
}

double EuclideanDistance(std::shared_ptr<Cavern> current, const std::shared_ptr<Cavern> goal) {
	//pow(x, 2) is the same as x^2, but this operator isn't used in C++ for a number of reasons (https://stackoverflow.com/a/14627505/11136104)
	int x = pow(goal->getX() - current->getX(), 2), y = pow(goal->getY() - current->getY(), 2);
	return sqrt(x + y);
}

std::vector<int> reconstructPath(std::shared_ptr<Cavern>& __restrict current) { //rebuilds the final path to be in the order of traversal from start to the goal
	std::vector<int> totalPath;
	totalPath.push_back(current->getID()); //add the current cavern, which will be the goal, to the path

	while (current->getParent() != nullptr) { //while we have a parent (previous) cavern to get to...
		current = current->getParent(); //change the current cavern to the parent...
		totalPath.insert(totalPath.begin(), current->getID()); //then insert the current cavern to the start of the vector
	}

	return totalPath;
}

std::vector<int> AStar(std::vector<std::shared_ptr<Cavern>>& __restrict caverns, const std::shared_ptr<Cavern> goal) {
	std::shared_ptr<Cavern> current; //determines the current cavern in our search; the one about to be searched for connecting paths

	//initialises the starting cavern's values
	caverns[0]->setPending();
	caverns[0]->gScoreSet(0);
	caverns[0]->fScoreSet(EuclideanDistance(caverns[0], goal));

	while (shortestDistance(caverns, current)) { //as long as we have a cavern to search (shortest path doesn't set 'current' to nothing), we will search along the path with the currently known shortest distance to the goal
		if (current == goal) //exits the search by returning the path result, if it's found
			return reconstructPath(current);
		
		current->setSearched(); //declare that the current cavern has been searched, then search it's connections

		for (std::shared_ptr<Cavern> connection : current->getConnections()) { //for each connection of the current cavern, in caverns[x].connections
			if (connection->hasBeenSearched()) //skip this check if the connected path has been previously searched
				continue;
			
			double gScoreTentative = current->gScoreGet() + EuclideanDistance(current, connection); //creates a temporary distance which stores the distance to this connection of the current cavern, from the start
			
			if (gScoreTentative >= connection->gScoreGet()) //check if the distance to this connection through the current path supersedes the pervious shortest path found to this connection...
				continue; //if not, skip recording of this path

			connection->setPending(); //declare this cavern as pending a search

			//records the details of this new shortest path found
			connection->setParent(current);
			connection->gScoreSet(gScoreTentative);
			connection->fScoreSet(connection->gScoreGet() + EuclideanDistance(connection, goal));
		}
	}

	return {}; //returns an empty vector to signify an inconclusive search - no path available
}

int main(int argc, char **argv) {
	if (argc == 2) { //determines if we were given a file name
		std::vector<std::shared_ptr<Cavern>> caverns; //declares a vector of pointers to Cavern objects;
		//'shared_ptr' allows us to store multiple copies of the same pointer to the same object - we will need to access the caverns from more than one place, such as the list above and each cavern's list connections
		//we use a shared_ptr because it is unwise to use a vector<Cavern*>, as it quickly becomes difficult to understand ownership of the pointer as we use it through more and more references

		readCAV(argv[1], caverns); //retrieves the values in the file given and stores them in 'caverns' using the reference to it

		if (!caverns.empty()) { //continues as long as the file was opened successfully
			std::vector<int> solution = AStar(caverns, caverns.back()); //utilizes the A* algorithm to try and find a solution to get to the goal cavern

			writeCSN(argv[1], solution);
		}
	}
	else
		std::cout << "(!) invalid number of parameters\n";
	return 0;
}