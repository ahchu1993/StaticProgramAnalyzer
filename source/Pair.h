#ifndef PAIR_H
#define PAIR_H

#include <iostream>
#include <string>

using namespace std;

template <class T, class Y> class Pair{
private:
	T _first;
    Y _second;

public:
	Pair(T first, Y second);

	T getFirst();

	Y getSecond();

	void setFirst(T f);

	void setSecond(Y s);
};

#endif