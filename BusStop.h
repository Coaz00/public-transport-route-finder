#ifndef BUSSTOP_H
#define BUSSTOP_H

#include<iostream>
#include<list>

using namespace std;

class BusStop {
public:
	BusStop();
	BusStop(int code, const string& name);
	~BusStop();

	int getCode() const;
	string getName() const;
	bool getVisited() const;
	BusStop* getPrevious() const;
	list<BusStop*>& getNeighbours();

	void setVisited(bool b);
	void setPrevious(BusStop* bs);

	void makeStop(const string& info); // loads data from string and gives that to BusStop
private:
	int code_; //number different for every BusStop
	string name_;
	bool visited_; // wheter BusStop is visisted or not during BFS algorithm
	BusStop* previous_; // remembers which BusStop added current BusStop to queue during BFS algorithm
	list<BusStop*> neighbours_; // BusStops that are directly connected to current stop

	int readNumber(int& pos, const string& s) const; 
	string readName(int& pos, const string& s) const;

	void deleteNeighbours();
};



#endif