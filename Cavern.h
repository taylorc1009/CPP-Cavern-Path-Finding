#pragma once
#include<vector>

class Cavern {
public:
	int id;
	int x;
	int y;
	std::vector<int> connections;
	Cavern(int _id, int _x, int _y) : id(_id), x(_x), y(_y) {}
	int getX() const { return x; }
	int getY() const { return y; }
};