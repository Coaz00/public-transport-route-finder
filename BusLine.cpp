#include "BusLine.h"


BusLine::BusLine() : visited_(false), previous_(nullptr)
{
}

BusLine::BusLine(const string& name) : name_(name), visited_(false), previous_(nullptr)
{
}

BusLine::~BusLine()
{
	deleteList(direction_A_);
	deleteList(direction_B_);
	deleteCrossing();
}

string BusLine::getName() const
{
	return name_;
}

list<BusStop*> BusLine::getDirA() const
{
	return direction_A_;
}

list<BusStop*> BusLine::getDirB() const
{
	return direction_B_;
}

list<BusLine*>& BusLine::getCrossing()
{
	return crossing_;
}

bool BusLine::getVisited() const
{
	return visited_;
}

BusLine* BusLine::getPrevious() const
{
	return previous_;
}

void BusLine::setVisited(bool b)
{
	visited_ = b;
}

void BusLine::setPrevious(BusLine* bl)
{
	previous_ = bl;
}

void BusLine::makeLine(const string& info, const list<BusStop*>& stops)
{
	int pos = 0;
	name_ = readName(pos, info);

	while (1) {
		int code = readNumber(pos, info);

		for (const auto& i : stops)
			if (i->getCode() == code) {
				direction_A_.push_back(i);
				break;
			}

		if (info[pos++] == '#') break;
	}

	while (pos < info.size()) {
		int code = readNumber(pos, info);

		for (const auto& i : stops)
			if (i->getCode() == code) {
				direction_B_.push_back(i);
				break;
			}
		pos++;
	}
}

void BusLine::findNeighbours(BusStop* bs)
{
	list<BusStop*>::iterator it, help;

	for(it = direction_A_.begin(); it != direction_A_.end(); it++)
		if (bs->getCode() == (*it)->getCode()) {
			help = it;
			help++;
			if (help != direction_A_.end())
				if(!alreadyAdded((*help)->getCode(), bs->getNeighbours()))
					bs->getNeighbours().push_back(*help);
		}

	for (it = direction_B_.begin(); it != direction_B_.end(); it++)
		if (bs->getCode() == (*it)->getCode()) {
			help = it;
			help++;
			if (help != direction_B_.end())
				if (!alreadyAdded((*help)->getCode(), bs->getNeighbours()))
					bs->getNeighbours().push_back(*help);
		}
}

int BusLine::sharedStops(BusLine* bl1, BusLine* bl2)
{
	int count = 0;

	if (bl1->getName() == bl2->getName()) return -1;

	for(const auto& i : bl1->busStopsThroughLine())
		for(const auto& j : bl2->busStopsThroughLine())
			if (i->getCode() == j->getCode()) {
				count++;
				break;
			}

	return count;
}

list<int> BusLine::numberOfCrossings(const list<BusLine*>& lines)
{
	list<int> num_of_crossings;

	for (const auto& i : lines) {
		if (name_ == i->getName()) num_of_crossings.push_back(-1);
		else num_of_crossings.push_back(sharedStops(this, i));
	}

	return num_of_crossings;
}

list<BusStop*> BusLine::busStopsThroughLine() const
{
	list<BusStop*> v;
	
	busStopsThroughDirection(v, direction_A_);
	busStopsThroughDirection(v, direction_B_);

	return v;
}

void BusLine::addCrossingToQueue(queue<BusLine*>& bus_line_queue)
{
	for(auto i : crossing_)
		if (!i->getVisited()) {
			bus_line_queue.push(i);
			i->setVisited(true);
			i->setPrevious(this);
		}
}

bool BusLine::containsStop(int code) const
{
	for (auto i : direction_A_)
		if (i->getCode() == code) return true;

	for (auto i : direction_B_)
		if (i->getCode() == code) return true;

	return false;
}

void BusLine::writeStops(int start, int end, string& out)
{
	bool write = false;
	list<BusStop*> whole_route = makeWholeRoute();
	
	for (auto i : whole_route) {
		if (i->getCode() == start) write = true;
		if (write) {
			out += to_string(i->getCode());
			out += ' ';
		}
		if (i->getCode() == end)
			if (write) break;
	}
	out.pop_back();
}

void BusLine::deleteList(list<BusStop*>& l)
{
	l.clear();
}

void BusLine::deleteCrossing()
{
	crossing_.clear();
}

bool BusLine::alreadyAdded(int code, list<BusStop*>& v) const
{
	for (const auto& i : v)
		if (i->getCode() == code) return true;

	return false;
}

void BusLine::busStopsThroughDirection(list<BusStop*>& v, const list<BusStop*>& l) const
{
	for (const auto& i : l)
		if (!alreadyAdded(i->getCode(), v))
			v.push_back(i);
}

string BusLine::readName(int& pos, const string& s) const
{
	string name;

	while (s[pos] != ' ')
		name += s[pos++];
	pos++;

	return name;
}

int BusLine::readNumber(int& pos, const string& s) const
{
	int number = 0;

	while (s[pos] != ' ' && s[pos] != '#' && pos < s.size())
		number = 10 * number + (s[pos++] - '0');

	return number;
}

list<BusStop*> BusLine::makeWholeRoute()
{
	list<BusStop*> whole_route;

	for (auto i : direction_A_)
		whole_route.push_back(i);


	bool first = true;
	for (auto i : direction_B_) {
		if(!first) whole_route.push_back(i);
		first = false;
	}

	return whole_route;
}
