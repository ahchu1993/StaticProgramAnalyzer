#ifndef PAIR_H
#define PAIR_H

#include <iostream>
#include <string>

using namespace std;

class Pair{
private:
	string _first;
	string _second;

public:
	Pair(string first, string second);

	string getFirst();

	string getSecond();
};

#endif