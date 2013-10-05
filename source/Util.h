#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
class Util
{
public:
	Util(void);
	~Util(void);
	static string convertIntToString(int input);
	static int convertStringToInt(string input);
};

#endif