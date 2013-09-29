#include "Pair.h"

template <class T> Pair<T>::Pair(T first, T second){
    _first = first;
    _second = second;
}

template <class T>  T Pair<T>::getFirst(){
	return _first;
}

template <class T>  T Pair<T>::getSecond(){
	return _second;
}

template <class T> void Pair<T>::setFirst(T f){
	_first = f;
}

template <class T> void Pair<T>::setSecond(T s){
	_second = s;
}