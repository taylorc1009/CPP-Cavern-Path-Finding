#pragma once
#include<vector>

class Cavern {
private:
	int x;
	int y;
	std::vector<int> connections;
public:
	Cavern(int _x, int _y) : x(_x), y(_y) {}
	int getX() const { return x; }
	int getY() const { return y; }
	std::vector<int> const &getConnections() const { return connections; }
	void addConnection(int c) { connections.push_back(c); }
};