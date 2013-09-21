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
class EvaluatorTree{
public:
    //Attributes
    //template<class T>
	struct e_node{
		string ref;
        vector<e_node*> links;
        int value;
        int weights;
	};
    //Methods
    e_node addNode(e_node node);
    void addLink(e_node parent, e_node child);
};