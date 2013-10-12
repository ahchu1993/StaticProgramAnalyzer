#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
using namespace std;
class Util
{
public:
	Util(void);
	~Util(void);
	static string convertIntToString(int input);
	static int convertStringToInt(string input);
	static vector<string> removeDuplicate(vector<string> a);
};

#endif