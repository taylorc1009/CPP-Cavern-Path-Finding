#pragma once

class Cavern {
public:
	int id;
	int x;
	int y;
	int* connections;
	Cavern(int _x, int _y, int size) : x(_x), y(_y) {
		this->connections = new int[size];
	}
	Cavern() = default;
	int getX() const { return x; }
	int getY() const { return y; }
};