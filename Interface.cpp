#include "BusNetwork.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

bool isNumber(const string& s); // checks if string is a number
int toNumber(const string& s); // converts string to an integer

int main() {
	BusNetwork* net = new BusNetwork();

	string input;
	int option; // choice 
	int code;
	int start;
	int end;
	bool exit; // exit for loop
	bool exit2; // exit for loop
	string name;
	string bus_stops_file;
	string bus_lines_file;

	cout << "Dobrodosli u simulator mreze gradskog prevoza. Molimo Vas, odaberite opciju:" << endl;

	while (1) {
		cout << "1. Ucitavanje podataka o mrezi gradskog prevoza" << endl;
		cout << "0. Kraj rada" << endl;

		cin >> input;
		cout << endl;

		if (isNumber(input)) option = toNumber(input);
		else {
			cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
			continue;
		}

		switch (option) {
		case 1:
			cout << "Molimo Vas, unesite putanju do fajla sa stajalistima:" << endl;
			cin >> bus_stops_file;
			cout << endl;

			cout << "Molimo Vas, unesite putanju do fajla sa linijama:" << endl;
			cin >> bus_lines_file;
			cout << endl;

			try {
				net->makeNetwork(bus_stops_file, bus_lines_file);
			}
			catch (FileNotFound& e) {
				cout << e.what() << endl;
				cout << "Ucitavanje mreze gradskog saobracaja nije uspelo. Pokusajte ponovo." << endl;
				continue;
			}
			catch (InvalidInput& e) {
				cout << e.what() << endl;
				cout << "Ucitavanje mreze gradskog saobracaja nije uspelo. Pokusajte ponovo." << endl;
			}
			cout << "Mreza gradskog prevoza je uspesno ucitana." << endl;
			cout << endl;

			exit = false;

			while (!exit) {
				cout << "Molimo Vas, odaberite opciju :" << endl;
				cout << "1. Prikaz informacija o stajalistu" << endl;
				cout << "2. Prikaz osnovnih informacija o liniji gradskog prevoza" << endl;
				cout << "3. Prikaz statistickih informacija o liniji gradskog prevoza" << endl;
				cout << "4. Pronalazak putanje izmedju dva stajalista" << endl;
				cout << "0. Vrati se nazad" << endl;

				cin >> input;
				cout << endl;

				if (isNumber(input)) option = toNumber(input);
				else {
					cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
					continue;
				}

				switch (option) {
				case 1:
					// bus stop info

					cout << "Molimo Vas, unesite sifru stajalista:" << endl;
					cin >> input;
					cout << endl;

					if (isNumber(input)) code = toNumber(input);
					else {
						cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
						continue;
					}

					try {
						net->busStopInfo(code);
					}
					catch (BusStopDoesNotExist& e) {
						cout << e.what() << endl;
						cout << "Pokusajte ponovo" << endl;
						continue;
					}

					cout << "Uspesno kreiran fajl stajaliste_" << to_string(code) << ".txt" << endl;

					break;
				case 2:
					// bus line info

					cout << "Molimo Vas, unesite oznaku linije:" << endl;
					cin >> name;
					cout << endl;

					try {
						net->busLineInfo(name);
					}
					catch (BusLineDoesNotExist& e) {
						cout << e.what() << endl;
						cout << "Pokusajte ponovo" << endl;
						continue;
					}
					cout << "Uspesno kreiran fajl linija_" << name << ".txt" << endl;
					break;
				case 3:
					// bus line stat

					cout << "Molimo Vas, unesite oznaku linije:" << endl;
					cin >> name;
					cout << endl;

					try {
						net->busLineStat(name);
					}
					catch (BusLineDoesNotExist& e) {
						cout << e.what() << endl;
						cout << "Pokusajte ponovo" << endl;
						continue;
					}

					cout << "Uspesno kreiran fajl statistika_" << name << ".txt" << endl;

					break;
				case 4:
					// finding route

					exit2 = false;

					while (!exit2) {
						cout << "Molimo Vas, izaberite strategiju pronalaska putanje" << endl;
						cout << "1. Bilo koja strategija" << endl;
						cout << "2. Putanja sa prolaskom kroz najmanje stajalista" << endl;
						cout << "3. Putanja sa najmanje presedanaja" << endl;
						cout << "0. Vrati se nazad" << endl;

						cin >> input;
						cout << endl;

						if (isNumber(input)) option = toNumber(input);
						else {
							cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
							continue;
						}

						switch (option) {
						case 1:
							// random

							cout << "Molimo Vas, unesite sifru pocetnog stajalista" << endl;
							cin >> input;
							cout << endl;

							if (isNumber(input)) start = toNumber(input);
							else {
								cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
								continue;
							}

							cout << "Molimo Vas, unesite sifru krajnjeg stajalista" << endl;
							cin >> input;
							cout << endl;

							if (isNumber(input)) end = toNumber(input);
							else {
								cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
								continue;
							}

							try {
								net->randomRoute(start, end);
							}
							catch (BusStopDoesNotExist& e) {
								cout << e.what() << endl;
								cout << "Pokusajte ponovo" << endl;
								continue;
							}

							cout << "Uspesno kreiran fajl putanja_" << to_string(start) << "_" << to_string(end) << ".txt" << endl;
							break;
						case 2:
							// least stops

							cout << "Molimo Vas, unesite sifru pocetnog stajalista" << endl;
							cin >> input;
							cout << endl;

							if (isNumber(input)) start = toNumber(input);
							else {
								cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
								continue;
							}

							cout << "Molimo Vas, unesite sifru krajnjeg stajalista" << endl;
							cin >> input;
							cout << endl;

							if (isNumber(input)) end = toNumber(input);
							else {
								cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
								continue;
							}

							try {
								net->leastStopsRoute(start, end);
							}
							catch (BusStopDoesNotExist& e) {
								cout << e.what() << endl;
								cout << "Pokusajte ponovo" << endl;
								continue;
							}

							cout << "Uspesno kreiran fajl putanja_" << to_string(start) << "_" << to_string(end) << ".txt" << endl;
							break;
						case 3:
							// least lines
							
							cout << "Molimo Vas, unesite sifru pocetnog stajalista" << endl;
							cin >> input;
							cout << endl;

							if (isNumber(input)) start = toNumber(input);
							else {
								cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
								continue;
							}
							cout << "Molimo Vas, unesite sifru krajnjeg stajalista" << endl;
							cin >> input;
							cout << endl;

							if (isNumber(input)) end = toNumber(input);
							else {
								cout << "Neispravan ulaz! Molimo Vas pokusajte ponovo." << endl;
								continue;
							}

							try {
								net->leastLinesRoute(start, end);
							}
							catch (BusStopDoesNotExist& e) {
								cout << e.what() << endl;
								cout << "Pokusajte ponovo" << endl;
								continue;
							}

							cout << "Uspesno kreiran fajl putanja_" << to_string(start) << "_" << to_string(end) << ".txt" << endl;
							break;
						case 0:
							exit2 = true;
							break;
						default:
							cout << "Nemoguc izbor! Pokusajte ponovo." << endl;
							continue;
						}
					}

					break;
				case 0:
					exit = true;
					break;
				default:
					cout << "Nemoguc izbor! Pokusajte ponovo." << endl;
					continue;
				}
			}
			break;
		case 0:
			cout << "Dovidjenja!" << endl;
			delete net;
			return 0;
		default:
			cout << "Nemoguc izbor! Pokusajte ponovo." << endl;
			continue;
		}
	}
}

bool isNumber(const string& s)
{
	for (int i = 0; i < s.size(); i++)
		if (!(s[i] >= '0' && s[i] <= '9'))
			return false;

	return true;
}

int toNumber(const string& s)
{
	int number = 0;
	for (int i = 0; i < s.size(); i++)
		number = 10 * number + (s[i] - '0');

	return number;
}
