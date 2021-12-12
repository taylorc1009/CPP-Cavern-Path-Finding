#include "data.h"

void readCAV(char* name, std::vector<std::shared_ptr<Cavern>> *caverns) {
	//builds the path to the .cav file - C++ cannot concatenate pure strings (name + ".cav") as, to the compiler, they're initially pointers
	std::string path(std::string(name) + std::string(".cav"));

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
					(*caverns)[col].get()->addConnection((*caverns)[row]); //so add the connection to the respective cavern's connections vector

				col++; //iterates along the row
				if (col == size) { //if the current row is finished...
					row++; //move to the next...
					col = 0; //and start at row index (column) 0 again
				}
			}
			else if (i > 0 && i <= size * 2) { //if the range of values we're currently iterating is between the first value (size) and the last coordinate - size*2

				if (isY) { //if the current value is y...
					(*caverns).push_back(std::make_shared<Cavern>(Cavern(row, temp, stoi(tok)))); //create a new shared pointer to the cavern and store the current value as x and the temporary value as y...
					row++;
				}
				else
					temp = stoi(tok); //otherwise store x temporarily

				if (i == size * 2)
					row = 0;
				else
					isY = !isY; //switches between the "current value is y" being true or false
			}
			else if (i == 0) { //if the current iteration is the first, this will be the amount of caverns present
				size = stoi(tok);
				(*caverns).reserve(size); //creates ('reserve's) empty spaces in the list of 'shared_ptr<Cavern>' objects to store x ('size') amount of pointers
			}
			i++;
		}
		file.close();
	}
	else
		std::cout << "(!) failed to open file: " << path << "\n";
}