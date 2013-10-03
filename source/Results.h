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
		string ref;
       // cell* before;
        cell* next;
        string value;
        int weights;
	};
    
    struct Columns{
		string ref;
        list<cell*> links;
	};
    static set<Columns*> columns;
    //Methods
    static cell createNode(string ref, vector<cell*> links, int value, int weights);
    static cell merge(cell parent, cell child, bool flag);
    static cell addLink(cell parent, cell child);
    //static int findColumn(string ref);
    static Pair<Columns*,int> findColumn(string ref);
    /*
     parameters: 1. list of enodes pointer, results to record all the nodes found
     2. vector of links
     3. target nodes
     Function: This function will find all the nodes from a given vector of links and store them inside a given list.
     */
   // static void findNode(vector<Pair<cell*, int>>& results, cell node);
    static cell checkValid(int weight);
    static void duplicateAndAdd(cell parent, cell child);
    Results();
};