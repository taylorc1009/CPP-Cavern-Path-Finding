#include "data.h"

void readCAV(const char* __restrict name, std::vector<std::shared_ptr<Cavern>>& __restrict caverns) {
	const std::string path(std::string(name) + std::string(".cav")); //builds the path to the .cav file - C++ cannot concatenate pure strings (name + ".cav") as, to the compiler, they're initially pointers

	std::ifstream file(path); //attempts to open the file...
	if (!file.is_open()) //if the file failed to open, checks if it may be in the 'cavs' directory
		file.open(std::string("cavs/") + path);

	if (file.is_open()) { //continues if the file is open
		std::string tok;
		int i = 0, row = 0, col = 0, size = 0, temp = 0;
		bool isY = false; //used to determine if the current coordinate we're attempting to store is y

		while (std::getline(file, tok, ',')) { //tokenizes the .cav file by ','
			if (i > (size * 2) && i <= (size * size) + (size * 2)) { //if the range of values we're currently iterating is between the last coordinate and the end of the file - (size*2)+(size*size)
				if (stoi(tok) && row != col) //if the token is 1 then this means there's a connection (as long as 'row != col' as this would signify a cavern is connected to itself, which would cause the search to fail)... 
					caverns[col]->addConnection(caverns[row]); //so add the connection to the respective cavern's connections vector

				if (col == size - 1) { //if the current row is finished...
					row++; //move to the next...
					col = 0; //and start at row index (column) 0 again
				}
				else
					col++; //iterates along the row
			}
			else if (i > 0 && i <= size * 2) { //if the range of values we're currently iterating is between the first value (size) and the last coordinate - size*2
				if (isY) { //if the current value is y...
					caverns.push_back(std::make_shared<Cavern>(Cavern(row, temp, stoi(tok)))); //create a new shared pointer to the cavern and store the current value as x and the temporary value as y...
					row++;
				}
				else
					temp = stoi(tok); //otherwise store x temporarily

				if (i == size * 2)
					row = 0;
				else
					isY = !isY; //switches between the "current value is y" being true or false
			}
			else if (i == 0) //if the current iteration is the first, this will be the amount of caverns present
				size = stoi(tok);
			i++;
		}
		file.close();
	}
	else
		std::cout << "(!) failed to open file: " << path << "\n";
}

void writeCSN(const char* __restrict name, const std::vector<int>& __restrict solution) { //getting solution by reference to save memory
	if (!solution.empty()) { //if the solution is empty, the algorithm found no path to the goal so don't continue
		//builds the file path to output the solution to
		std::string path(std::string(name) + std::string(".csn"));
		std::ofstream file(path);

		if (file.is_open()) {
			for (int cavern : solution) //for every cavern in the solution...
				file << std::to_string(cavern + 1) << ' '; //output the ID + 1; a list of size 30 would have cavern IDs 0-29
			file.close();
		}
		else
			std::cout << "(!) failed to create/open output file\n";
	}
	else
		std::cout << "(!) search completed: no path found\n";
}