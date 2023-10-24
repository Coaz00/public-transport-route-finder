#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include<exception>

using namespace std;

class FileNotFound : public exception {
public:
	FileNotFound() : exception("Fajl nije pronadjen!"){}
};

class BusStopDoesNotExist : public exception {
public:
	BusStopDoesNotExist() : exception("Stajaliste ne postoji!") {}
};

class BusLineDoesNotExist : public exception {
public:
	BusLineDoesNotExist() : exception("Linija ne postoji!") {}
};

class InvalidInput : public exception {
public:
	InvalidInput() : exception("Sadrzaj ulaznog fajla nije odgovarajuci!") {}
};

#endif