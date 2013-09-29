#include "Pair.h"

template <class T, class Y> Pair<T, Y>::Pair(T first, Y second){
    _first = first;
    _second = second;
}

template <class T, class Y> T Pair<T,Y>::getFirst(){
	return _first;
}

template <class T, class Y> Y Pair<T,Y>::getSecond(){
	return _second;
}

template <class T, class Y> void Pair<T,Y>::setFirst(T f){
	_first = f;
}

template <class T, class Y> void Pair<T,Y>::setSecond(Y s){
	_second = s;
}