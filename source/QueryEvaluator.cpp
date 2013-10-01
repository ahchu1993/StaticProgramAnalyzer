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
        QueryEvaluator::Qprocessor.group_relations();//group the relations
        entity = Qprocessor.getSymbols();//declaration type, name
		result = Qprocessor.getResults();//select clause
		desAbstr = Qprocessor.getRelations();//all the relations
		pattern = Qprocessor.getPatterns();//all the patterns
        attr_pairs = Qprocessor.getAttrPairs();//with
        
        processAttrParis(attr_pairs);
        processPattern(pattern);
        processRelations(desAbstr);
    }
    return results;
}