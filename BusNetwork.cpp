#include "BusNetwork.h"

BusNetwork::BusNetwork()
{
}

BusNetwork::~BusNetwork()
{
	deleteStops();
	deleteLines();
}

void BusNetwork::makeNetwork(const string& file_stops, const string& file_lines)
{
	fillStops(file_stops);
	fillLines(file_lines);
	sortLines();
	fillNeighbours();
	fillCrossing();
}

void BusNetwork::busStopInfo(int code)
{
	if (!busStopExist(code)) throw BusStopDoesNotExist();

	for(const auto& i : stops_)
		if (i->getCode() == code) {
			string bus_stop_info = "stajaliste_";
			bus_stop_info += to_string(i->getCode());
			bus_stop_info += ".txt";

			string out = busStopOutput(i);

			fstream output(bus_stop_info, ios::out);

			output << out;

			output.close();
		}
}

void BusNetwork::busLineInfo(const string& name)
{
	if (!busLineExist(name)) throw BusLineDoesNotExist();

	for(const auto& i : lines_)
		if (i->getName() == name) {
			string bus_line_info = "linija_";
			bus_line_info += i->getName();
			bus_line_info += ".txt";

			string out = busLineOutput(i);

			fstream output(bus_line_info, ios::out);

			output << out;

			output.close();
		}
}

void BusNetwork::busLineStat(const string& name)
{
	if (!busLineExist(name)) throw BusLineDoesNotExist();

	for(const auto& i : lines_)
		if (i->getName() == name) {
			string bus_line_stat = "statistika_";
			bus_line_stat += i->getName();
			bus_line_stat += ".txt";

			string out = statOutput(i);

			fstream output(bus_line_stat, ios::out);

			output << out;

			output.close();
		}
}

void BusNetwork::randomRoute(int start, int end)
{
	leastStopsRoute(start, end);
}

void BusNetwork::leastStopsRoute(int start, int end)
{
	if (!busStopExist(start) || !busStopExist(end)) throw BusStopDoesNotExist();

	findLeastStopsRoute(start, end);
	list<BusStop*> route = reconstructRoute(start, end);
	list<pair<BusStop*, BusLine*>> lines_used = usedLines(route);

	printRoute(route, lines_used);
}

void BusNetwork::leastLinesRoute(int start, int end)
{
	if (!busStopExist(start) || !busStopExist(end)) throw BusStopDoesNotExist();

	findLeastLinesRoute(start, end);
	list<BusLine*> route = reconstructLines(start, end);
	string out = writeRoute(route, start, end);
	makeFileLinesRoute(out, start, end);

}

void BusNetwork::printStops() const
{
	for (const auto& i : stops_) {
		cout << i->getCode() << " " << i->getName() << ' ';
		for (const auto& j : i->getNeighbours())
			cout << j->getCode() << ' ';
		cout << endl;
	}
}

void BusNetwork::printLines() const
{
	for (const auto& i : lines_) {
		cout << i->getName() << ' ';
		
		for (const auto& j : i->getDirA())
			cout << j->getCode() << ' ';

		cout << '#';

		for (const auto& j : i->getDirB())
			cout << j->getCode() << ' ';

		cout << endl;
	}
}

void BusNetwork::fillStops(const string& file_stops)
{
	deleteStops();

	fstream input(file_stops, ios::in);

	if (input.fail()) throw FileNotFound();

	string line;

	while (getline(input, line)) {
	
		BusStop* bs = new BusStop();
		bs->makeStop(line);

		stops_.push_back(bs);
	}

	input.close();
}

void BusNetwork::fillLines(const string& file_lines)
{
	deleteLines();

	fstream input(file_lines, ios::in);

	if (input.fail()) throw FileNotFound();

	string line;

	while (getline(input, line)) {

		BusLine* bl = new BusLine();
		bl->makeLine(line, stops_);

		lines_.push_back(bl);
	}

	input.close();
}

void BusNetwork::deleteStops()
{
	for (const auto& i : stops_) delete i;
	stops_.clear();
}

void BusNetwork::deleteLines()
{
	for (const auto& i : lines_) delete i;
	lines_.clear();
}

bool BusNetwork::compare(const string& s1, const string& s2) const
{
	if (s1 == s2) return false;

	int i = 0;
	while (1) {
		if (s1[i] > s2[i]) return true;
		else if (s1[i] < s2[i]) return false;
		else {
			i++;
			if (i == s1.size()) return false;
			if (i == s2.size()) return true;
		}
	}
}

void BusNetwork::sortLines()
{
	list<BusLine*>::iterator it1, it2;

	for (it1 = lines_.begin(); it1 != lines_.end(); it1++) {
		for (it2 = it1; it2 != lines_.end(); it2++) {
			if (compare((*it1)->getName(), (*it2)->getName())) {
				swap(*it1, *it2);
			}
		}
	}
}

string BusNetwork::busStopOutput(BusStop* bs)
{
	string out;

	out += to_string(bs->getCode());
	out += ' ';
	out += bs->getName();
	out += " [";

	for(const auto& i : lines_)
		for(const auto& j : i->busStopsThroughLine())
			if (bs->getCode() == j->getCode()) {
				out += i->getName();
				out += ' ';
				break;
			}

	out.pop_back();
	out += ']';

	return out;
}

string BusNetwork::busLineOutput(BusLine* bl)
{
	string out;

	out += bl->getName();
	out += ' ';
	out += bl->getDirA().front()->getName();
	out += "->";
	out += bl->getDirA().back()->getName();
	out += '\n';

	addStopsToLineOutput(out, bl->getDirA());
	addStopsToLineOutput(out, bl->getDirB());
	
	out.pop_back();

	return out;
}

string BusNetwork::statOutput(BusLine* bl)
{
	string out;

	out += bl->getName();
	out += '\n';

	addCrossingToOutput(out, bl);
	addMaxCrossingToOutput(out, bl);

	return out;
}

string BusNetwork::routeOutput(const list<BusStop*>& route, const list<pair<BusStop*, BusLine*>>& lines_used)
{
	string out;
	BusStop* bs = nullptr;
	bool print = false;
	BusLine* current_line = nullptr;

	for (auto i : lines_used) {
		if (bs == nullptr) bs = i.first;
		else {
			for (auto j : route) {
				if (bs->getCode() == j->getCode()) print = true;

				if (print) {
					out += to_string(j->getCode());
					out += ' ';
				}

				if (i.first->getCode() == j->getCode()) print = false;
			}
			out.pop_back();
			out += '\n';
			bs = i.first;
		}

		if (current_line != nullptr) {
			out += current_line->getName();
		}
		out += "->";
		out += i.second->getName();
		out += '\n';

		current_line = i.second;
	}

	print = false;
	for (auto i : route) {
		if (i->getCode() == bs->getCode()) print = true;
		if (print) {
			out += to_string(i->getCode());
			out += ' ';
		}
	}

	out.pop_back();

	return out;
}

void BusNetwork::addStopsToLineOutput(string& out, const list<BusStop*>& l)
{
	for (const auto& i : l) {
		out += to_string(i->getCode());
		out += ' ';
		out += i->getName();
		out += '\n';
	}
}

void BusNetwork::addCrossingToOutput(string& out, BusLine* bl)
{
	for (const auto& i : bl->getCrossing()) {
		out += i->getName();
		out += ' ';
	}

	out.pop_back();
	out += '\n';
}

void BusNetwork::addMaxCrossingToOutput(string& out, BusLine* bl)
{
	list<int> num_of_crossings = bl->numberOfCrossings(lines_);
	int max = findMax(num_of_crossings);

	for (const auto& i : lines_)
		if (max == BusLine::sharedStops(bl, i)) {
			out += i->getName();
			out += ',';
		}

	out.pop_back();
}

int BusNetwork::findMax(const list<int>& l)
{
	int max = -1;

	for (const auto& i : l)
		if (max < i || max == -1)
			max = i;

	return max;
}

BusStop* BusNetwork::findBusStop(int code)
{
	for (auto i : stops_)
		if (i->getCode() == code) return i;

	return nullptr;
}

void BusNetwork::addNeighboursToQueue(BusStop* current, queue<BusStop*>& bus_stop_queue)
{
	for (auto i : current->getNeighbours()) {
		if (!i->getVisited()) {
			bus_stop_queue.push(i);
			i->setVisited(true);
			i->setPrevious(current);
		}
		
	}
}

void BusNetwork::resetVisitedStops()
{
	for (auto i : stops_)
		i->setVisited(false);
}

void BusNetwork::resetPreviousStops()
{
	for (auto i : stops_)
		i->setPrevious(nullptr);
}

BusLine* BusNetwork::commonLine(BusStop* bs1, BusStop* bs2)
{
	bool first = false;
	bool second = false;

	for (auto i : lines_) {
		for (auto j : i->getDirA()) {
			if (j->getCode() == bs1->getCode()) first = true;
			if (j->getCode() == bs2->getCode()) second = true;
		}
		for (auto j : i->getDirB()) {
			if (j->getCode() == bs1->getCode()) first = true;
			if (j->getCode() == bs2->getCode()) second = true;
		}
		if (first && second) return i;
		else {
			first = false;
			second = false;
		}
	}

	return nullptr;
}

void BusNetwork::findLeastStopsRoute(int start, int end)
{
	queue<BusStop*> bus_stop_queue;
	BusStop* starting_stop = findBusStop(start);

	bus_stop_queue.push(starting_stop);
	starting_stop->setVisited(true);

	while (!bus_stop_queue.empty()) {
		BusStop* current = bus_stop_queue.front();
		bus_stop_queue.pop();

		if (current->getCode() == end) {
			resetVisitedStops();
			return;
		}

		addNeighboursToQueue(current, bus_stop_queue);
	}

	resetVisitedStops();
}

list<BusStop*> BusNetwork::reconstructRoute(int start, int end)
{
	list<BusStop*> route;

	BusStop* starting_stop = findBusStop(start);
	BusStop* ending_stop = findBusStop(end);

	BusStop* tmp = ending_stop;

	while (tmp != nullptr) {
		route.push_back(tmp);
		tmp = tmp->getPrevious();
	}

	route.reverse();

	resetPreviousStops();

	return route;
}

list<pair<BusStop*, BusLine*>> BusNetwork::usedLines(const list<BusStop*>& route)
{
	list<pair<BusStop*, BusLine*>> lines_used;

	list<BusStop*> reverse_route = route;
	reverse_route.reverse();

	pair<BusStop*, BusLine*> p;
	BusStop* current = route.front();

	while (current->getCode() != route.back()->getCode()) {
		p.first = current;
		for (auto i : reverse_route) {
			BusLine* common_line = commonLine(current, i);
			if (common_line != nullptr) {
				p.second = common_line;
				current = i;
				break;
			}
		}
		lines_used.push_back(p);
	}

	return lines_used;
}

void BusNetwork::printRoute(const list<BusStop*>& route, const list<pair<BusStop*, BusLine*>>& lines_used)
{
	string route_name = "putanja_";
	route_name += to_string(route.front()->getCode());
	route_name += "_";
	route_name += to_string(route.back()->getCode());
	route_name += ".txt";

	string out = routeOutput(route, lines_used);

	fstream output(route_name, ios::out);
	
	output << out;

	output.close();

}

void BusNetwork::addLinesThroughStopToQueue(int code, queue<BusLine*>& bus_line_queue)
{
	for (auto i : lines_) 
		for(auto j : i->busStopsThroughLine())
			if (code == j->getCode()) {
				bus_line_queue.push(i);
				i->setVisited(true);
				break;
			}
	
}

void BusNetwork::resetVisitedLines()
{
	for (auto i : lines_)
		i->setVisited(false);
}

void BusNetwork::resetPreviousLines()
{
	for (auto i : lines_)
		i->setPrevious(nullptr);
}

void BusNetwork::writeChange(BusLine* first, BusLine* second, string& out)
{
	if (first != nullptr && second != nullptr) {
		out += first->getName();
		out += "->";
		out += second->getName();
		return;
	}

	if (first == nullptr && second == nullptr) return;

	if (first == nullptr && second != nullptr) {
		out += "->";
		out += second->getName();
		return;
	}

	if (first != nullptr && second == nullptr) {
		out += "->";
		out += first->getName();
		return;
	}
}

int BusNetwork::commonStop(BusLine* bl1, BusLine* bl2)
{
	for (auto i : bl1->busStopsThroughLine())
		for (auto j : bl2->busStopsThroughLine())
			if (i->getCode() == j->getCode()) return j->getCode();

	return -1;
}

void BusNetwork::makeFileLinesRoute(const string& out, int start, int end)
{
	string output_name = "putanja_";
	output_name += to_string(start);
	output_name += "_";
	output_name += to_string(end);
	output_name += ".txt";

	fstream output(output_name, ios::out);

	output << out;

	output.close();
}


void BusNetwork::findLeastLinesRoute(int start, int end)
{
	queue<BusLine*> bus_line_queue;

	addLinesThroughStopToQueue(start, bus_line_queue);

	BusLine* current;

	while (!bus_line_queue.empty()) {
		current = bus_line_queue.front();
		bus_line_queue.pop();

		if (current->containsStop(end)) {
			resetVisitedLines();
			return;
		}

		current->addCrossingToQueue(bus_line_queue);
	}

	resetVisitedLines();
}

list<BusLine*> BusNetwork::reconstructLines(int start, int end)
{
	list<BusLine*> route;

	BusLine* last_line = nullptr;

	for (auto i : lines_)
		if (i->getPrevious() != nullptr && i->containsStop(end)) {
			last_line = i;
			break;
		}

	if (last_line == nullptr)
		for(auto i : lines_)
			if (i->containsStop(start) && i->containsStop(end)) {
				route.push_back(i);
				break;
			}

	if (last_line != nullptr)
		while (last_line != nullptr) {
			route.push_front(last_line);
			last_line = last_line->getPrevious();
		}
	
	resetPreviousLines();
	return route;
}

string BusNetwork::writeRoute(list<BusLine*> lines_used, int start, int end)
{
	string out = "";

	list<BusLine*>::iterator curr, next;

	int starting_stop, ending_stop;
	starting_stop = start;

	out += "->";
	out += lines_used.front()->getName();
	out += '\n';
	
	for (curr = lines_used.begin(); curr != lines_used.end(); curr++) {
		next = curr;
		next++;
		if (next != lines_used.end()) {
			ending_stop = commonStop(*curr, *next);

			(*curr)->writeStops(starting_stop, ending_stop, out);

			out += '\n';
			out += (*curr)->getName();
			out += "->";
			out += (*next)->getName();
			out += '\n';

			starting_stop = ending_stop;
		}
		else (*curr)->writeStops(starting_stop, end, out);

	}

	return out;
}

bool BusNetwork::busStopExist(int code)
{
	for (auto i : stops_)
		if (i->getCode() == code) return true;

	return false;
}

bool BusNetwork::busLineExist(const string& name)
{
	for (auto i : lines_)
		if (i->getName() == name) return true;

	return false;
}

void BusNetwork::fillNeighbours()
{
	for (const auto& i : stops_)
		for (const auto& j : lines_)
			j->findNeighbours(i);
}

void BusNetwork::fillCrossing()
{
	for (const auto& i : lines_)
		for (const auto& j : lines_)
			if (i->getName() != j->getName() && BusLine::sharedStops(i, j) > 0)
				i->getCrossing().push_back(j);
}


