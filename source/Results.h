//
//  EvaluatorTree.h
//  cs3202
//
//  Created by Zhao Weixiang on 21/9/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//


#include <iostream>
#include "QueryPreprocessor.h"
#include <iostream>
#include "PKB.h"
#include "string.h"
#include <vector>
#include <list>
#include <sstream>
#include <set>
#include "Pair.h"
using namespace std;
class Results{
public:
    //Attributes
    //template<class T>
	struct cell{
       // string ref;
       // cell* before;
       // cell* next;
        string value;
       // int weights;
	};
    
    struct Columns{
		string ref;
        list<cell*> links;
	};
    //static set<Columns*> columns;
    set<string> columns;
    //list<cell*> tuple;
    list<list<cell*>*> tuple_list;
    //Methods

    
    //void joinTable(cell parent, cell child, bool flag);
    void initTable(Pair<string, string> refs,vector<Pair<string,string>> results);
    void validation(int parent_index, int child_index, vector<Pair<string,string>> results);
    int findColumn(string ref);
    void join(Pair<string, string> refs, vector<Pair<string,string>> results);
    list<list<cell*>*> equiJoin(int join_index,int position, vector<Pair<string,string>> results);
    void merge(Results table);
    Results();
};