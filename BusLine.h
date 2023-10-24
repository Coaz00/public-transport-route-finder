#ifndef BUSLINE_H
#define BUSLINE_H

#include "BusStop.h"
#include<iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>

using namespace std;

class BusLine {
public:
	BusLine();
	BusLine(const string& name);
	~BusLine();

	string getName() const;
	list<BusStop*> getDirA() const;
	list<BusStop*> getDirB() const;
	list<BusLine*>& getCrossing();
	bool getVisited() const;
	BusLine* getPrevious() const;

	void setVisited(bool b);
	void setPrevious(BusLine* bl);

	void makeLine(const string& info, const list<BusStop*>& stops); // loads data from string and list of BusStops and gives that to BusLine

	void findNeighbours(BusStop* bs); // find directly connected stops to given stop (alongside this line) and adds it to stops' neighbours

	static int sharedStops(BusLine* bl1, BusLine* bl2); // returns number of shared stops between given lines

	list<int> numberOfCrossings(const list<BusLine*>& lines); // returns list of number of shared stops between current line and all the others 

	list<BusStop*> busStopsThroughLine() const; // returns list with all BusStops that line goes through

	void addCrossingToQueue(queue<BusLine*>& bus_line_queue); // adds crossing lines to queue during BFS algorithm

	bool containsStop(int code) const; // returns wheter this lane contains given stop

	void writeStops(int start, int end, string& out); // write stops alongside line between given stops into given string
private:
	string name_;
	list<BusStop*> direction_A_;
	list<BusStop*> direction_B_;
	list<BusLine*> crossing_; // lines that have common stop with this line
	bool visited_; // wheter line is visited during BFS algorithm
	BusLine* previous_; // previous line is line that added this line to queue during BFS algorithm

	void deleteList(list<BusStop*>& l);
	void deleteCrossing();
	bool alreadyAdded(int code, list<BusStop*>& v) const; // checks if given stop is added to given list

	void busStopsThroughDirection(list<BusStop*>& v, const list<BusStop*>& l) const; // all bus stops that line goes through in given direction

	string readName(int& pos, const string& s) const;
	int readNumber(int& pos, const string& s) const;

	list<BusStop*> makeWholeRoute(); // makes one route out of both directions
};


#endif