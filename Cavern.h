#pragma once
#include<vector>

class Cavern {
private:
	int id, x, y;
	std::shared_ptr<Cavern> parent;
	double gScore = DBL_MAX, fScore = DBL_MAX;
	bool searched = false, pending = false;
	std::vector<std::shared_ptr<Cavern>> connections;
public:
	Cavern(int _id, int _x, int _y) : id(_id), x(_x), y(_y) { parent = nullptr; }
	Cavern() = default;

	int getID() const { return id; }
	int getX() const { return x; }
	int getY() const { return y; }
	std::shared_ptr<Cavern> getParent() { return parent; }
	double gScoreGet() { return gScore; }
	double fScoreGet() { return fScore; }
	bool hasBeenSearched() { return searched; }
	bool isPending() { return pending; }
	std::vector<std::shared_ptr<Cavern>>& getConnections() { return connections; }

	void setParent(std::shared_ptr<Cavern> p) { parent = p; }
	void gScoreSet(double s) { gScore = s; }
	void fScoreSet(double s) { fScore = s; }
	void setSearched() { pending = false; searched = true; }
	void setPending() { pending = true; }
	void addConnection(std::shared_ptr<Cavern>& c) { connections.push_back(c); }
};