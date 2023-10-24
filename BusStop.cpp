#include "BusStop.h"

BusStop::BusStop(): visited_(false), previous_(nullptr)
{
}

BusStop::BusStop(int code, const string& name): code_(code), name_(name), visited_(false), previous_(nullptr)
{
}

BusStop::~BusStop()
{
	deleteNeighbours();
}

int BusStop::getCode() const
{
	return code_;
}

string BusStop::getName() const
{ 
	return name_;
}

bool BusStop::getVisited() const
{
	return visited_;
}

BusStop* BusStop::getPrevious() const
{
	return previous_;
}

list<BusStop*>& BusStop::getNeighbours()
{
	return neighbours_;
}

void BusStop::setVisited(bool b)
{
	visited_ = b;
}

void BusStop::setPrevious(BusStop* bs)
{
	previous_ = bs;
}

void BusStop::makeStop(const string& info)
{
	int pos = 0;
	code_ = readNumber(pos, info);
	name_ = readName(pos, info);
}

int BusStop::readNumber(int& pos, const string& s) const
{
	int number = 0;

	while (s[pos] != ' ')
		number = 10 * number + (s[pos++] - '0'); // making number from string of digits(chars)
	pos++;

	return number;
}

string BusStop::readName(int& pos, const string& s) const
{
	string name;

	while (pos < s.size())
		name += s[pos++];

	return name;
}

void BusStop::deleteNeighbours()
{
	neighbours_.clear();
}
