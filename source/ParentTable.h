#pragma once
#include <string>
#include <vector>
using namespace std;
#ifndef PARENTTABLE_H
#define PARENTTABLE_H
typedef struct children_stm_t{
	int stm;
	string DE;
}children_stm;
typedef struct parent_row_t{
	int parent;
	string parentDE;
	vector<children_stm> children;
}parent_row;
class ParentTable
{
private:
	vector<parent_row> parentTable; 
	vector<parent_row>::iterator it;
public:
	ParentTable(void);
	void insert(int stm1, string DE1, int stm2, string DE2);
	int getParent (int stm);
	vector<int> getChildren(int stm, string DE);
	vector<int> getParentT(int stmt);
	vector<int> getChildrenT(int stmt,string DE);
	int getSize();
	void print();
};
#endif
