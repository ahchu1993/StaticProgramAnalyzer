#include "Pair.h"

Pair::Pair(string first, string second){
	_first = first;
	_second = second;
}

string Pair::getFirst(){
	return _first;
}

string Pair::getSecond(){
	return _second;
}