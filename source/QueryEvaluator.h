//
//  QueryEvaluator.h
//  cs3202
//
//  Created by Zhao Weixiang on 16/9/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//
#pragma once
#include "QueryPreprocessor.h"
#include <iostream>
#include "PKB.h"
#include "string.h"
#include <vector>
#include <list>
#include <sstream>
#include <set>
using namespace std;


class QueryEvaluator{
public:
    /*template<class T>
	struct rated_desAbstr{
		QueryPreprocessor::designAbstraction desAbstr;
		int rating;
	}; */

    QueryEvaluator(PKB* pkb);
    
    list<string> processQuery(string query);
    

private:

	map<string, set<string>> valueTable;
    vector<QueryPreprocessor::entityReff> entities;
	vector<string> result_refs;
	list<BaseRelation*> constant_relations;
	list<list<BaseRelation*>> grouped_relations;
	PKB* pkb;

	QueryPreprocessor* Qprocessor;

    //void processAttrPair(attr_compare* attr_pair);
	// init
	void initialzeValueTable();
    void updateValueTable(string ref, vector<string> values);

	// main method
    bool processConstantRelations();
	bool processGroupedRelations();

	// designAbstraction
	bool processTwoConstantsDesignAbstraction(designAbstraction* da);
	vector<pair<string,string>> processDesignAbstraction(designAbstraction* da);
	// pattern
	vector<pair<string,string>> processPattern(pattern* p);
	vector<pair<string,string>> patternAssign(pattern *p);
	vector<pair<string,string>> patternIfOrWhile(pattern *p);

	list<string> getResults();
};