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
using namespace std;
class EvaluatorTree{
public:
    //Attributes
    //template<class T>
	struct e_node{
		string ref;
        e_node* before;
        e_node* next;
        int value;
        int weights;
	};
    
    struct Columns{
		string ref;
        vector<e_node*> links;
	};
    static vector<Columns*> columns;
    //Methods
    static e_node createNode(string ref, vector<e_node*> links, int value, int weights);
    static e_node merge(e_node parent, e_node child, bool flag);
    static e_node addLink(e_node parent, e_node child);
    static int findColumn(string ref);
    /*
     parameters: 1. list of enodes pointer, results to record all the nodes found
     2. vector of links
     3. target nodes
     Function: This function will find all the nodes from a given vector of links and store them inside a given list.
     */
    static void findNode(vector<EvaluatorTree::e_node*>& results, EvaluatorTree::e_node node);
    static e_node checkValid(int weight);
    
    EvaluatorTree();
};