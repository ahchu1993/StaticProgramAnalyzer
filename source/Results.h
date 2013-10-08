//
//  EvaluatorTree.h
//  cs3202
//
//  Created by Zhao Weixiang on 21/9/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//
#include <vector>
#include "string"
#include <list>
#include <set>
#include <utility>
using namespace std;
class Results{
public:
    //Attributes
    //template<class T>
    
    struct Table{
		set<string> columns;
        list<list<string>> tuple_list;
	};
    //static set<Columns*> columns;
    set<string> columns;
    //list<cell*> tuple;
    list<list<string>> tuple_list;
    //Methods
    Table table;
    
    //void joinTable(cell parent, cell child, bool flag);
    void initTable(pair<string, string> refs,vector<pair<string,string>> results);
    list<list<string>> validation(int parent_index, int child_index, vector<pair<string,string>> results);
    int findColumn(string ref);
    void join(pair<string, string> refs, vector<pair<string,string>> results);
    list<list<string>> equiJoin(int join_index,int position, vector<pair<string,string>> results);
    void merge(Results table);
    Results();
};