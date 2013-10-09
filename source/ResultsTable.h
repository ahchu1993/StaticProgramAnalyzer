//
//  ResultsTable.h
//  cs3202
//
//  Created by Zhao Weixiang on 9/10/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//

#ifndef __cs3202__ResultsTable__
#define __cs3202__ResultsTable__

#include <vector>
#include "string"
#include <vector>
#include <set>
#include <utility>
#include <map>
using namespace std;
class ResultsTable{
public:
    //Attributes
    //template<class T>
    
    struct Table{
		vector<string> columns;
        vector<vector<string>> tuples;
	};
    //static set<Columns*> columns;
    vector<string> columns;
    //vector<cell*> tuple;
    vector<vector<string>> tuples;
    //Methods
    Table table;
    
    //void joinTable(cell parent, cell child, bool flag);
    void initTable(pair<string, string> refs,vector<pair<string,string>> results);
    vector<vector<string>> validation(int parent_index, int child_index, vector<pair<string,string>> results);
    int findColumn(string ref);
    void join(pair<string, string> refs, vector<pair<string,string>> results);
    vector<vector<string>> equiJoin(int join_index,int position, vector<pair<string,string>> results);
    void merge(ResultsTable table);
    void eliminateColumns(vector<string>refs);
    ResultsTable();
};

#endif /* defined(__cs3202__ResultsTable__) */
