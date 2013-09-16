#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "DesignExtractor.h"
using namespace std;

DesignExtractor::DesignExtractor (PKB *pkb) 
{
//	int findFollowed(int stm);
//	vector<int> findFollowedT(int stmt, string DE);
//	int findFollows(int stm);
//	vector<int> findFollowsT(int stmt, string DE);
//	bool isFollowed(int stm1,int stm2);
//	void printFollowTable();
	pkb->printFollowTable();
	cout<<"??????"<<endl;
}