#ifndef FOLLOWTABLE_H
#define FOLLOWTABLE_H
#include <vector>
#include <string>
#include <iostream>
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
	vector<follow_row>::iterator it;
public:
	FollowTable(void);
	void insertFollow(int stm1, string DE1, int stm2, string DE2);
	int findFollowed(int stm);
	int findFollows(int stm);
	bool isFollowed(int stm1,int stm2);
	vector<int> findFollowedT(int stmt, string DE);
	vector<int> findFollowsT(int stmt, string DE);
	int getSize();
	void print();
};
#endif
