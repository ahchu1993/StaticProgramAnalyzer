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
	return 0;
}