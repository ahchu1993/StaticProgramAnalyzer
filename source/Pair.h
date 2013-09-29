#ifndef PAIR_H
#define PAIR_H

#include <iostream>
#include <string>

using namespace std;

template <class T> class Pair{
private:
	T _first, _second;

public:
	Pair(T first, T second);

	T getFirst();

	T getSecond();

	void setFirst(T f);

	void setSecond(T s);
};

#endif