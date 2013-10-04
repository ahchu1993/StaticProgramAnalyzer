//
//  QueryEvaluator.cpp
//  cs3202
//
//  Created by Zhao Weixiang on 16/9/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//

#include "QueryEvaluator.h"

list<string> QueryEvaluator::processQuery(string query){
    list<string> results;
    if( QueryEvaluator::Qprocessor.process_query(query)!=true){
        cout<<"invalid querty/n";
    }
    else{
        //store all the parsed query infomation
        QueryEvaluator::Qprocessor.group_relations();//group the relations
        entity = Qprocessor.getSymbols();//declaration type, name
		result = Qprocessor.getResults();//select clause
		desAbstr = Qprocessor.getRelations();//all the relations
		pattern = Qprocessor.getPatterns();//all the patterns
        attr_pairs = Qprocessor.getAttrPairs();//with
        //start to evaluate query
        initialzeValueTable(entity);
        processAttrParis(attr_pairs);
        processPattern(pattern);
        processRelations(desAbstr);
    }
    return results;
}

//store all the possible values for each synonmy
void QueryEvaluator::initialzeValueTable(vector<QueryPreprocessor::entityReff> entities){
    for (int i =0; i<entity.size(); i++) {
        QueryPreprocessor::entityReff entity;
        QueryEvaluator::value_set temp;
        temp.ref = entity.synonym;
        temp.values = QueryEvaluator::pkb->getValues(entity.at(i).type);
        valueSet.push_back(temp);
    }


}
void processAttrPairs(vector<QueryPreprocessor::attr_compare> attr_pairs){



}
void processPattern(vector<QueryPreprocessor::pattern> pattern){


}
void processRelations(vector<QueryPreprocessor::designAbstraction> desAbstr){
    for (int i=0; i<desAbstr.size(); i++) {
        QueryPreprocessor::designAbstraction relation = desAbstr.at(i);
        if (relation.relation_type.compare("Modifies")==0) {
            
        }
        else if (relation.relation_type.compare("Calls")==0){
        }
        else if (relation.relation_type.compare("Calls*")==0){
        }
        else if (relation.relation_type.compare("Uses")==0){
        }
        else if (relation.relation_type.compare("Parent")==0){
        }
        else if (relation.relation_type.compare("Parent*")==0){
        }
        else if (relation.relation_type.compare("Follows")==0){
        }
        else if (relation.relation_type.compare("Follows*")==0){
        }
        else if (relation.relation_type.compare("Next")==0){
        }
        else if (relation.relation_type.compare("Next*")==0){
        }
        else if (relation.relation_type.compare("Affects")==0){
        }
        else if (relation.relation_type.compare("Affects*")==0){
        }
    }

}