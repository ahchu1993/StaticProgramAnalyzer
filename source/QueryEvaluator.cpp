//
//  QueryEvaluator.cpp9
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
		entity = Qprocessor.declaration_reffs;//declaration type, name
		result = Qprocessor.result_reffs;//select clause
		constant_relations = Qprocessor.constant_relations;//all the relations
		//grouped_relations = & Qprocessor.grouped_relations;
       
        //start to evaluate query
        initialzeValueTable(entity);
        processConstantRelations();
        
        processGroupedRelations();
    }
    return results;
}
bool processConstantRelations(){
	/*go thru constant_relations
		baseRelation* b = constant_relations(i);
		if(b->type=="des")
		{
			designAbstraction* da = static_cast<designAbstraction*>(b);
			da->relation_type;
			pkb ->
		}else (b->type)
		*/
}

bool processGroupedRelations(){
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
void processPattern(vector<pattern> pattern){


}
void QueryEvaluator::updateValueTable(string ref, vector<string> values){
    
    for (set<string>::iterator g = valueTable[ref].begin(); g != valueTable[ref].end(); g++) {
        string value = *g;
        bool flag_find = false;
        for (int i =0; i<values.size(); i++) {
            if (value.compare(values.at(i))==0) {
                flag_find = true;
                break;
            }
        }
        if (!flag_find) {
            valueTable[ref].erase(g);//if not match at all, delete this value from set
        }
        
    }//for
}
void QueryEvaluator::processRelations(vector<designAbstraction> desAbstr){
    for (int i=0; i<desAbstr.size(); i++) {
        designAbstraction relation = desAbstr.at(i);
        if (relation.relation_type.compare("Modifies")==0) {
		
        }
        else if (relation.relation_type.compare("Calls")==0){
			pkb->getCall(relation.ref1,relation.ref1_type, relation.ref2, relation.ref2_type);
        }
        else if (relation.relation_type.compare("Calls*")==0){

        }
        else if (relation.relation_type.compare("Uses")==0){
			pkb->getUse(relation.ref1,relation.ref1_type, relation.ref2, relation.ref2_type);
        }
        else if (relation.relation_type.compare("Parent")==0){
			pkb->getParent(relation.ref1,relation.ref1_type, relation.ref2, relation.ref2_type);
        }
        else if (relation.relation_type.compare("Parent*")==0){
        }
        else if (relation.relation_type.compare("Follows")==0){
			pkb->getFollow(relation.ref1,relation.ref1_type, relation.ref2, relation.ref2_type);
        }
        else if (relation.relation_type.compare("Follows*")==0){
        }
        else if (relation.relation_type.compare("Next")==0){
			pkb->getNext(relation.ref1,relation.ref1_type, relation.ref2, relation.ref2_type);
        }
        else if (relation.relation_type.compare("Next*")==0){
        }
        else if (relation.relation_type.compare("Affects")==0){
			
        }
        else if (relation.relation_type.compare("Affects*")==0){
        }
    }

}