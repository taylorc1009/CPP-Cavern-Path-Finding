#pragma once
#include<vector>

class Cavern {
private:
	int id, x, y;
	std::shared_ptr<Cavern> parent; //stores the cavern that has the shortest distance, going TO this one
	double gScore = DBL_MAX, fScore = DBL_MAX; //maximises the values so that, at first, any path found to this cavern will be recognised as the best; this is so we can definitely give this cavern a pathway to it if we find one and it doesn't already have one
	bool searched = false, pending = false;
	std::vector<std::shared_ptr<Cavern>> connections; //stores the (child) caverns of this one so we can go to them FROM this
public:
	Cavern(int _id, int _x, int _y) : id(_id), x(_x), y(_y) { parent = nullptr; } //initialise parent as 'nullptr' so we can tell, later, when we've retraced back to the starting cavern (there will be no more parents)
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

	bool operator==(const Cavern& __restrict rhs) {
		return this->getID() == rhs.getID();
	}
};