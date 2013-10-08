#ifndef FOLLOWTABLE_H
#define FOLLOWTABLE_H
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include "Util.h"

using namespace std;
typedef struct follow_row_t{
	int col1;
	int col2;
	string de1;
	string de2;
}follow_row;

class FollowTable
{

private:
	vector<follow_row> followTable; 
	
public:
	FollowTable(void);
	void insertFollow(int stm1, string DE1, int stm2, string DE2);
	int findFollowed(int stm);
	int findFollows(int stm);
	bool isFollowed(int stm1,int stm2);
	vector<int> findFollowedT(int stmt);
	vector<int> findFollowsT(int stmt);
	vector<int> getFollowsList(string DE);
	vector<int> getFollowedList(string DE);
	bool checkFollow(vector<int> set1, vector<int> set2);
	vector<pair<string,string>> getFollowPairList(vector<int> set1, vector<int> set2);
	int getSize();
	void print();
};
#endif
