
#pragma once
#include <iostream>
#include "string.h"
#include <vector>
#include <list>
#include <sstream>
#include <set>
#include "PKB.h"
#include "QueryPreprocessor.h"
//#include "Results.h"
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

	map<string, set<string>*> valueTable;
    vector<QueryPreprocessor::entityReff> entities;
	vector<string> result_refs;
	list<BaseRelation*> constant_relations;
	list<list<BaseRelation*>> grouped_relations;
	PKB* pkb;
    //Results table;
	QueryPreprocessor* Qprocessor;

    //void processAttrPair(attr_compare* attr_pair);
	// init
	void initialzeValueTable();
    void updateValueTable(string ref, vector<string> values);
    void updateValueTable(designAbstraction* da, vector<pair<string,string>> values);
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