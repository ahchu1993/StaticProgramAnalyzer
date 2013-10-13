#include "Util.h"

Util::Util(void)
{
}


Util::~Util(void)
{
}
string Util::convertIntToString(int input){
	ostringstream convert;
	convert << input;
	return convert.str();
}

int Util::convertStringToInt(string input){
	return atoi( input.c_str() );
}
vector<string> Util::removeDuplicate(vector<string> a){
	set<string> s(a.begin(), a.end());
	vector<string> result;
	result.assign(s.begin(), s.end());
	return result;
}

vector<int> Util::removeDuplicateInt(vector<int> a){
	set<int> s(a.begin(), a.end());
	vector<int> result;
	result.assign(s.begin(), s.end());
	return result;
}