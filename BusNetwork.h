#ifndef BUSNETWORK_H
#define BUSNETWORK_H

#include "BusLine.h"
#include "BusStop.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>

using namespace std;

class BusNetwork {
public:
	BusNetwork();
	~BusNetwork();

	void makeNetwork(const string& file_stops, const string& file_lines); // loads data from file with info about stops and file with info about lines and gives it to network

	void busStopInfo(int code);
	void busLineInfo(const string& name);
	void busLineStat(const string& name);

	void randomRoute(int start, int end);
	void leastStopsRoute(int start, int end);
	void leastLinesRoute(int start, int end);

	//-------------------------
	void printStops() const; // DEBUGGING
	void printLines() const; // DEBUGGING
	//-------------------------
private:
	list<BusStop*> stops_; // essentially adjacency list for graph where stops are vertices
	list<BusLine*> lines_; // essentially adjacency list for graph where lines are vertices and they are connected if they have a common stop

	void fillStops(const string& file_stops);
	void fillLines(const string& file_lines);
	void fillNeighbours();
	void fillCrossing();

	void deleteStops();
	void deleteLines();

	bool compare(const string& s1, const string& s2) const;
	void sortLines();

	//formating output
	string busStopOutput(BusStop* bs);
	string busLineOutput(BusLine* bl);
	string statOutput(BusLine* bl);
	string routeOutput(const list<BusStop*>& route, const list<pair<BusStop*, BusLine*>>& lines_used);

	void addStopsToLineOutput(string& out, const list<BusStop*>& l);
	void addCrossingToOutput(string& out, BusLine* bl);
	void addMaxCrossingToOutput(string& out, BusLine* bl);

	int findMax(const list<int>& l);
	
	BusStop* findBusStop(int code);
	void addNeighboursToQueue(BusStop* current, queue<BusStop*>& bus_stop_queue);
	void resetVisitedStops(); // prepares for next BFS
	void resetPreviousStops(); // prepares for next BFS
	BusLine* commonLine(BusStop* bs1, BusStop* bs2); //returns common line between 2 given stops (any common line)

	void findLeastStopsRoute(int start, int end); // BFS - part where it just updates previous
	list<BusStop*> reconstructRoute(int start, int end); // BFS - from previous reconstruct route
	list<pair<BusStop*, BusLine*>> usedLines(const list<BusStop*>& route);
	void printRoute(const list<BusStop*>& route, const list<pair<BusStop*, BusLine*>>& lines_used);

	void addLinesThroughStopToQueue(int code, queue<BusLine*>& bus_line_queue);
	void resetVisitedLines();
	void resetPreviousLines();
	void writeChange(BusLine* first, BusLine* second, string& out);
	int commonStop(BusLine* bl1, BusLine* bl2);
	void makeFileLinesRoute(const string& out, int start, int end);

	void findLeastLinesRoute(int start, int end); // BFS - part where it just updates previous
	list<BusLine*> reconstructLines(int start, int end);// BFS - from previous reconstruct route
	string writeRoute(list<BusLine*> lines_used, int start, int end);

	bool busStopExist(int code);
	bool busLineExist(const string& name);
};


#endif