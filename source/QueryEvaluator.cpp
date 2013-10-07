
#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB* p){
	pkb  = p;
	Qprocessor = new QueryPreprocessor();
}

list<string> QueryEvaluator::processQuery(string query){
    list<string> results;
    if( Qprocessor->process_query(query)!=true){
        cout<<"invalid query\n";
    }
    else{
        //store all the parsed query infomation
		Qprocessor->group_relations();
		entities = Qprocessor->declaration_reffs;
		result_refs = Qprocessor->result_reffs;
		constant_relations = Qprocessor->constant_relations;
		grouped_relations =  Qprocessor->grouped_relations;
        //start to evaluate query
        initialzeValueTable();
        if(processConstantRelations()){     
			if(processGroupedRelations()){
				results = getResults();
				//return results;
			}
		}//else return results; // empty list
    }
    return results;
}
bool QueryEvaluator::processConstantRelations(){

	for(list<BaseRelation*>::iterator it=constant_relations.begin(); it!=constant_relations.end(); it++){
		BaseRelation* b = *it;

		vector<pair<string,string>> result_pairs; //store pairs of result for this relation

		if(b->type=="designAbstraction")
		{
			designAbstraction* da = static_cast<designAbstraction*>(b);

			//Handle 2 constants case
			bool b1 = da->ref1_type == "integer" || da->ref1_type == "string" || da->ref1_type == "_";
			bool b2 = da->ref2_type == "integer" || da->ref2_type == "string" || da->ref2_type == "_";
			if(b1&&b2){
				bool pass =  processTwoConstantsDesignAbstraction(da);
				if(!pass) return false;
				else continue;
			}

			result_pairs = processDesignAbstraction(da);
			if(result_pairs.empty()) return false; 
		}
		else if(b->type=="pattern"){
			
			pattern* p = static_cast<pattern*>(b);
			result_pairs  = processPattern(p);

			if(result_pairs.empty())
				return false;
			//update valueTable
			vector<string> temp;
			for(unsigned int i=0;i<result_pairs.size();i++){
				pair<string,string> t = result_pairs.at(i);
				temp.push_back(t.first);
			}
			//update synonym valueTable
			updateValueTable(p->synonym, temp);

			if(p->varRef_type=="variable"){
				vector<string> vars;
				for(unsigned int i=0;i<result_pairs.size();i++){
					pair<string,string> t = result_pairs.at(i);
					vars.push_back(t.second);
				}

				updateValueTable(p->varRef, vars);
			}

		}else {}
	}

	return true;
}

vector<pair<string,string>> QueryEvaluator::processDesignAbstraction(designAbstraction* da){
	string relation = da->relation_type;
	vector<pair<string, string>> res;
	// get ref_set 
	set<string> * ref1_set;
	set<string> * ref2_set;
	bool b1 = da->ref1_type=="string"||da->ref1_type=="integer"||da->ref1_type=="";
	bool b2 = da->ref2_type=="string"||da->ref1_type=="integer"||da->ref2_type=="";

	if(!b1) // ref1 is synonym
		ref1_set = valueTable[da->ref1];
	else if (da->ref2_type=="string"||da->ref2_type=="integer"){
		ref1_set = new set<string>;
		ref1_set->insert(da->ref1);
	}else {
	
	}

	if(!b2) //ref2 is synonym
		ref2_set = valueTable[da->ref2];
	else if (da->ref2_type=="string"||da->ref2_type=="integer"){
		ref2_set = new set<string>;
		ref2_set->insert(da->ref2);
	}
	else { // ref2 ="_"
		if(da->relation_type=="Uses"||da->relation_type=="Modifies")
			ref2_set = &pkb->getAllVars();
		else if(da->relation_type=="Calls"||da->relation_type=="Calls*")
			ref2_set = &pkb->getAllProcs();
		else 
			ref2_set = &pkb->getAllStmts();
	}

	if(relation == "Modifies"){
		res = pkb-> getModify(ref1_set, da->ref1_type, ref2_set, da->ref2_type);

	}else if(relation == "Uses"){
		res = pkb-> getUse(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Calls"){
		res = pkb-> getCall(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Parents"){
		res = pkb-> getParent(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Nexts"){
		res = pkb-> getNext(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}

	//update valueTable
	
	
	if(!b1){ //ref1 is a synonym
		vector<string> result;
		for(vector<pair<string,string>>::iterator it = res.begin();it!=res.end();it++){
			pair<string,string> p = *it;
			result.push_back(p.first);
		}
		updateValueTable(da->ref1, result);
	}
	
	if(!b2){ // ref2 is a synonym
		vector<string> result;
		for(vector<pair<string,string>>::iterator it = res.begin();it!=res.end();it++){
			pair<string,string> p = *it;
			result.push_back(p.second);
		}
		updateValueTable(da->ref2, result);
	}

	return res;

}
bool QueryEvaluator::processTwoConstantsDesignAbstraction(designAbstraction* da){
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

vector<pair<string,string>> QueryEvaluator::processPattern(pattern* p){
	vector<pair<string,string>> result;	
	if(p->pattern_type=="p_assign"){
		result=patternAssign(p);	
	}
	else{
		result = patternIfOrWhile(p);
	}
	return result;
}

vector<pair<string,string>> QueryEvaluator::patternAssign(pattern* p){
	map<int,PKB::postfixNode*> exp_list = pkb->postfixExprList;
	vector<pair<string,string>> result;
	if(p->varRef_type=="string"){

		set<string> s = *valueTable[p->synonym]; //redeced set

		for(set<string>::iterator it =s.begin();it!=s.end();it++){

			string a = *it;
			int aint = Util::convertStringToInt(a); 
			PKB::postfixNode* n = exp_list[aint];
			// get a node from the reduced set

			if(n->type=="assign"&&p->varRef==n->varRef){ //assign stmt, same varRef
				string q_expr = p->expr_tree;
				string p_expr = n->postfixExpr;

				if(p->exact&&q_expr==p_expr){ //complete match, and found
					string first = Util::convertIntToString(n->lineNum);
					string second = n->varRef;
					pair<string,string> * pa = new pair<string,string>(first,second);
					result.push_back(*pa);
					
				}
				else if(!(p->exact)){
					int pint = p_expr.find(q_expr); //postfix expr string matching
					if(pint<p_expr.size())
					{
						string first = Util::convertIntToString(n->lineNum);
						string second = n->varRef;
						pair<string,string> * pa = new pair<string,string>(first,second);
						result.push_back(*pa);
						
					}
				}else{}
			}
		}
	}else if(p->varRef_type=="variable"){ //varRef =="variable"
		set<string> s = *valueTable[p->synonym]; //redeced set
		set<string> vars = *valueTable[p->varRef]; //redeced set for vars
		for(set<string>::iterator it =s.begin();it!=s.end();it++){
			string a = *it;
			int aint = Util::convertStringToInt(a); 

			PKB::postfixNode* n = exp_list[aint]; //get a node from reduced set

			string q_expr = p->expr_tree;
			string p_expr = n->postfixExpr;
			for(set<string>::iterator it_vars = vars.begin();it_vars!=vars.end();it_vars++){				
				if(*it_vars==n->varRef){ // varRef from var_set match the node 
					if(p->exact){					
						if(q_expr==p_expr){
							string first = Util::convertIntToString(n->lineNum);
							string second = n->varRef;
							pair<string,string> * pa = new pair<string,string>(first,second);
							result.push_back(*pa);
							break;
						}
					}
					else {
						int pint = p_expr.find(q_expr);
						if(pint<p_expr.size()){
							string first = Util::convertIntToString(n->lineNum);
							string second = n->varRef;
							pair<string,string> * pa = new pair<string,string>(first,second);
							result.push_back(*pa);
							break;
						}
					}
				}				
			}			
		}
	}else { // varRef = "_"
		set<string> s = *valueTable[p->synonym]; //redeced set

		for(set<string>::iterator it =s.begin();it!=s.end();it++){

			string a = *it;
			int aint = Util::convertStringToInt(a); 
			PKB::postfixNode* n = exp_list[aint];
			// get a node from the reduced set

			
			string q_expr = p->expr_tree;
			string p_expr = n->postfixExpr;

			if(p->exact&&q_expr==p_expr){ //complete match, and found
				string first = Util::convertIntToString(n->lineNum);
				string second = n->varRef;
				pair<string,string> * pa = new pair<string,string>(first,second);
				result.push_back(*pa);
					
			}
			else if(!(p->exact)){
				int pint = p_expr.find(q_expr); //postfix expr string matching
				if(pint<p_expr.size())
				{
					string first = Util::convertIntToString(n->lineNum);
					string second = n->varRef;
					pair<string,string> * pa = new pair<string,string>(first,second);
					result.push_back(*pa);
						
				}
			}			
		}
	}
	return result;
}

vector<pair<string,string>> QueryEvaluator::patternIfOrWhile(pattern* p){
	map<int,PKB::postfixNode*> exp_list = pkb->postfixExprList;
	vector<pair<string,string>> result;
	set<string> s = *valueTable[p->synonym];
	if(p->varRef_type=="string"){
		for(set<string>::iterator it = s.begin();it!=s.end();it++){
			string a = *it;
			int aint = Util::convertStringToInt(a); //if stmt#

			PKB::postfixNode* n = exp_list[aint];

			if(p->varRef==n->varRef){
				string first = Util::convertIntToString(n->lineNum);
				string second = n->varRef;
				pair<string,string> * pa = new pair<string,string>(first,second);
				result.push_back(*pa);
				
			}
		}
	}else{
		set<string> vars = *valueTable[p->varRef];
		for(set<string>::iterator it = s.begin();it!=s.end();it++){
			string a = *it;
			int aint = Util::convertStringToInt(a); //if stmt#

			PKB::postfixNode* n = exp_list[aint];

			for(set<string>::iterator it_vars = vars.begin();it_vars!=vars.end();it_vars++)
				if(*it_vars==n->varRef){
					string first = Util::convertIntToString(n->lineNum);
					string second = n->varRef;
					pair<string,string> * pa = new pair<string,string>(first,second);
					result.push_back(*pa);
					break;
				}
		}
	}
	return result;
}

//store all the possible values for each synonmy
void QueryEvaluator::initialzeValueTable(){
    for (int i =0; i<entities.size(); i++) {
        QueryPreprocessor::entityReff entity = entities.at(i);
		if(entity.type=="procedure"){
			valueTable[entity.synonym] = &pkb->getAllProcs();
		}else if(entity.type=="variable"){
			valueTable[entity.synonym] = &pkb->getAllVars();
		}else if(entity.type=="constant"){
			valueTable[entity.synonym] = &pkb->getAllConstants();
		}else{
			vector<int> stmts;
			if(entity.synonym=="prog_line")
				stmts= QueryEvaluator::pkb->getStmtNo("stmt");
			else stmts = QueryEvaluator::pkb->getStmtNo(entity.type);
			set<string>* s = new set<string>;
			for(unsigned int i=0;i<stmts.size();i++){
				string a = Util::convertIntToString(stmts.at(i));		
				s->insert(a);
				valueTable[entity.synonym] = s;
			}
		}
    }
}
void QueryEvaluator::updateValueTable(string ref, vector<string> values){
    set<string> s = *valueTable[ref];
	set<string> * result = new set<string>;
    for (set<string>::iterator g = s.begin(); g != s.end(); g++) {
		for(unsigned int i=0;i<values.size();i++){
			if(*g==values.at(i))
				result->insert(*g);
		}
        
    }
	valueTable[ref] = result;
}
/*
void QueryEvaluator::updateValueTable(designAbstraction* da, vector<pair<string,string>> values){
    set<string> f = *valueTable[da->ref1];
    set<string> s = *valueTable[da->ref2];
	set<string> * result1 = new set<string>;
    set<string> * result2 = new set<string>;
    for (set<string>::iterator g = f.begin(); g != f.end(); g++) {
		for(unsigned int i=0;i<values.size();i++){
			if(*g==values.at(i).first)
				result1->insert(*g);
		}
        
    }
    for (set<string>::iterator m = s.begin(); m!= s.end(); m++) {
		for(unsigned int i=0;i<values.size();i++){
			if(*m==values.at(i).second)
				result2->insert(*m);
		}
        
    }
	valueTable[da->ref1] = result1;
    valueTable[da->ref2] = result2;
}*/

bool QueryEvaluator::processGroupedRelations(){
     /*Results results_table;
    list<list<BaseRelation*>> group_relations = Qprocessor->grouped_relations;
    for(list<list<BaseRelation*>>::iterator it = group_relations.begin();it!=group_relations.end();it++){
        vector<pair<string,string>> result_pairs;
        Results temp_table;
        list<BaseRelation*> relations = *it;//for each group
        for(list<BaseRelation*>::iterator iter = relations.begin();iter!=relations.end();iter++){//for each relation
            BaseRelation* relation = *iter;
            if(relation->type=="designAbstraction")
            {
                designAbstraction* da = static_cast<designAbstraction*>(relation);
                
                result_pairs = processDesignAbstraction(da);
                if(result_pairs.empty())
                    return false;
                else{
                    pair<string,string>ref_pair;
                    ref_pair.first = da->ref1;
                    ref_pair.second = da->ref2;
                    temp_table.join(ref_pair, result_pairs);
                }
                updateValueTable(da, result_pairs);
            }
            /*
            else if(relation->type=="pattern"){
                
                pattern* p = static_cast<pattern*>(relation);
                result_pairs  = processPattern(p);
                
                if(result_pairs.empty())
                    return false;
                //update valueTable
                vector<string> temp;
                for(unsigned int i=0;i<result_pairs.size();i++){
                    pair<string,string> t = result_pairs.at(i);
                    temp.push_back(t.first);
                }
                //update synonym valueTable
                updateValueTable(p->synonym, temp);
                
                if(p->varRef_type=="variable"){
                    vector<string> vars;
                    for(unsigned int i=0;i<result_pairs.size();i++){
                        pair<string,string> t = result_pairs.at(i);
                        vars.push_back(t.second);
                    }
                    
                    updateValueTable(p->varRef, vars);
                }
                
            }//
             
            results_table.merge(temp_table);
        }//for each relation
        table.merge(results_table);
    }//for each group */
    return true;
}


list<string> QueryEvaluator::getResults(){
	string r = result_refs.at(0);
	set<string> s = *valueTable[r];
	list<string> res;
	for(set<string>::iterator it = s.begin();it!=s.end();it++){
		res.push_back(*it);
	}
	return res;
}

