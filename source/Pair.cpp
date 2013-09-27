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

void Pair::setFirst(string f){
	_first = f;
}

void Pair::setSecond(string s){
	_second = s;
}