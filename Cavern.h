#pragma once
#include<vector>

class Cavern {
private:
	//int id;
	int x;
	int y;
	std::vector<int> connections;
public:
	//Cavern(int _id, int _x, int _y) : id(_id), x(_x), y(_y) {}
	Cavern(int _x, int _y) : x(_x), y(_y) {}
	//int getID() const { return id; }
	int getX() const { return x; }
	int getY() const { return y; }
	std::vector<int> const &getConnections() const { return connections; }
	void addConnection(int c) { connections.push_back(c); }
	/*void deleteConnections() {
		connections.clear();
		connections.swap(std::vector<int>(connections));
	}*/
};