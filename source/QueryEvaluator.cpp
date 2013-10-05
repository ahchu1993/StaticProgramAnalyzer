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
bool QueryEvaluator::processConstantRelations(){

	for(list<baseRelation*>::iterator it=constant_relations.begin(); it!=constant_relations.end(); it++){
		baseRelation* b = *it;
		if(b->type=="desAbstraction")
		{
			designAbstraction* da = static_cast<designAbstraction*>(b);
			string relation = da->relation_type;
			vector<Pair<string, string>> res;
			if(relation == "Modify"){
				res = pkb-> getModify(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
			}else if(relation == "Use"){
				res = pkb-> getUse(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
			}else if(relation == "Call"){
				res = pkb-> getCall(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
			}else if(relation == "Parent"){
				res = pkb-> getParent(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
			}else if(relation == "Next"){
				res = pkb-> getNext(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
			}

			if(res.size() == 0)
				return;

			vector<string> result;
			if(da->ref1_type == "integer"){
				for(unsigned i=0; i<res.size(); i++){
					result.push_back(res.at(i).getFirst());
				}
			}else if(da->ref2_type == "integer"){
				for(unsigned i=0; i<res.size(); i++){
					result.push_back(res.at(i).getSecond());
				}
			}
			//Update value table with result vector
		
			
		}
	}
		
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