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

	for(list<BaseRelation*>::iterator it=constant_relations.begin(); it!=constant_relations.end(); it++){
		BaseRelation* b = *it;
		if(b->type=="desAbstraction")
		{
			designAbstraction* da = static_cast<designAbstraction*>(b);
			//Handle 2 integers case
			if(da->ref1_type == "integer" && da->ref2_type == "integer"){
				processTwoConstantsRelations(da);
			}
			string relation = da->relation_type;
			vector<pair<string, string>> res;

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
				return 0;

			vector<string> result;
			string ref;
			if(da->ref1_type == "integer"){
				for(unsigned i=0; i<res.size(); i++){
					result.push_back(res.at(i).first);
					ref = da->ref2;
				}
			}else if(da->ref2_type == "integer"){
				for(unsigned i=0; i<res.size(); i++){
					result.push_back(res.at(i).second);
					ref = da->ref1;
				}
			}
			//Update value table with result vector
			updateValueTable(ref, result);	
		}
	}
		
}

bool QueryEvaluator::processTwoConstantsRelations(designAbstraction* da){
	string relation = da->relation_type;

	if(relation == "Modify"){
		return pkb-> checkModify(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Use"){
		return pkb-> checkUse(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Call"){
		return pkb-> checkCall(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Parent"){
		return pkb-> checkParent(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Next"){
		return pkb-> checkNext(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}

	
}
bool processGroupedRelations(){
    
}
//store all the possible values for each synonmy
void QueryEvaluator::initialzeValueTable(vector<QueryPreprocessor::entityReff> entities){
    for (int i =0; i<entities.size(); i++) {
        QueryPreprocessor::entityReff entity = entities.at(i);
        valueTable[entity.synonym] = QueryEvaluator::pkb->getValues(entity.type);
    }
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