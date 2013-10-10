
#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB* p){
	pkb  = p;
	Qprocessor = new QueryPreprocessor();
}

list<string> QueryEvaluator::processQuery(string query){
    list<string> results;
	bool constant_pass;
	bool grouped_pass;
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
		constant_pass= processConstantRelations();
        if(constant_pass){     
			grouped_pass = processGroupedRelations();
			if(grouped_pass){
				if(result_refs.size()==1)
					results = getResults();
				else{ }
				//return results;
			}
		}//else return results; // empty list
    }
	if(result_refs.size()>0&&result_refs[0]=="BOOLEAN"){
		if(constant_pass&&grouped_pass)
			results.push_back("true");
		else results.push_back("false");
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

			
			bool b1 = da->ref1_type == "integer" || da->ref1_type == "string" || da->ref1_type == "";
			bool b2 = da->ref2_type == "integer" || da->ref2_type == "string" || da->ref2_type == "";
			if(b1&&b2){ //Handle 2 constants case
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
			

		}else if(b->type=="attr_compare"){
			attr_compare* attr_pairs = static_cast<attr_compare*>(b);
			result_pairs = processAttrCompare(attr_pairs);
			if(result_pairs.empty())
				return false;
		}
	}

	return true;
}

set<string> QueryEvaluator::getValueSet(string ref,string ref_type,string relation_type){
	set<string> ref_set;
	bool b = ref_type=="string"||ref_type=="integer"||ref_type=="";
	if(!b){
		ref_set = *valueTable[ref];
	}else if(ref_type =="string"||ref_type=="integer"){
		ref_set.insert(ref);
	}else{ // "_"
		if(relation_type=="Uses"||relation_type=="Modifies"){
			ref_set = pkb->getAllVars();
		}		
		else if(relation_type=="Calls"||relation_type=="Calls*"){
			ref_set = pkb->getAllProcs();
		}		
		else 
			ref_set = pkb->getAllStmts();		
	}
	return ref_set;
}
vector<pair<string,string>> QueryEvaluator::processDesignAbstraction(designAbstraction* da){
	string relation = da->relation_type;
	vector<pair<string, string>> res;
	// get ref_set 
	set<string>  ref1_set;
	set<string>  ref2_set;
	bool b1 = da->ref1_type=="string"||da->ref1_type=="integer"||da->ref1_type=="";
	bool b2 = da->ref2_type=="string"||da->ref2_type=="integer"||da->ref2_type=="";

	ref1_set = getValueSet(da->ref1,da->ref1_type,da->relation_type);
	ref2_set = getValueSet(da->ref2,da->ref2_type,da->relation_type);

	if(relation == "Modifies"){
		res = pkb-> getModify(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);

	}else if(relation == "Uses"){
		res = pkb-> getUse(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation == "Calls"){
		res = pkb-> getCalls(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation == "Parent"){
		res = pkb-> getParent(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation =="Parent*"){
		res = pkb-> getParentT(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation =="Follows"){
		res = pkb->getFollow(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation=="Follows*"){
		res = pkb->getFollowT(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation =="Next"){
		res = pkb-> getNext(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}

	if(da->ref1==da->ref2){  // ref1 is the same as ref2
		vector<pair<string,string>> temp;
		for(unsigned int i=0;i<res.size();i++){
			pair<string,string> t = res.at(i);
			if(t.first==t.second){
				temp.push_back(pair<string,string>(t.first,t.second));
			}
		}
		res = temp;
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

	if(relation == "Modifies"){
		return pkb-> checkModify(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Uses"){
		return pkb-> checkUse(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Calls"){
		return pkb-> checkCalls(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Parent"){
		return pkb-> checkParent(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation=="Parent*"){
		return pkb-> checkParent(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation =="Follows"){
		return pkb->checkFollow(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation =="Follows*"){
		return pkb->checkFollowT(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Next"){
		return pkb-> checkNext(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}

	return false;
}

vector<pair<string,string>> QueryEvaluator::processPattern(pattern* p){
	vector<pair<string,string>> result;	
	if(p->pattern_type=="p_assign"){
		result=patternAssign(p);	
	}
	else{
		result = patternIfOrWhile(p);
	}

	//update valueTable
	vector<string> temp;
	for(unsigned int i=0;i<result.size();i++){
		pair<string,string> t = result.at(i);
		temp.push_back(t.first);
	}
	//update synonym valueTable
	updateValueTable(p->synonym, temp);

	if(p->varRef_type=="variable"){
		vector<string> vars;
		for(unsigned int i=0;i<result.size();i++){
			pair<string,string> t = result.at(i);
			vars.push_back(t.second);
		}

		updateValueTable(p->varRef, vars);
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


vector<pair<string,string>> QueryEvaluator::processAttrCompare(attr_compare* a){
	vector<pair<string,string>> result;
	set<string> s1 = getValueSet(a->left_ref,a->left_ref_type,"");
	set<string> s2 = getValueSet(a->right_ref,a->right_ref_type,"");
	set<string>::iterator it2;
	for(set<string>::iterator it = s1.begin();it!=s1.end();it++){
		it2 = s2.find(*it);
		if(it2!=s2.end()){
			pair<string,string> p(*it,*it);
		}
	}

	string type1 = a->left_ref_type;
	string type2 = a->right_ref_type;
	bool b1 = type1=="integer"||type1=="string";
	bool b2 = type2=="integer"||type2=="string";
	if(!b1){
		vector<string> temp;
		for(unsigned int i=0;i<result.size();i++){
			pair<string,string> t = result.at(i);
			temp.push_back(t.first);
		}
		//update synonym valueTable
		updateValueTable(a->left_ref, temp);
	}

	if(!b2){
		vector<string> temp;
		for(unsigned int i=0;i<result.size();i++){
			pair<string,string> t = result.at(i);
			temp.push_back(t.second);
		}
		//update synonym valueTable
		updateValueTable(a->right_ref, temp);
	}
	return result;
}
//store all the possible values for each synonmy
void QueryEvaluator::initialzeValueTable(){
    for (int i =0; i<entities.size(); i++) {
        QueryPreprocessor::entityReff entity = entities.at(i);
		if(entity.type=="procedure"){
			set<string> s = pkb->getAllProcs();
			set<string> *t = new set<string>;
			for(set<string>::iterator it=s.begin();it!=s.end();it++){
				t->insert(*it);
			}
			valueTable[entity.synonym] = t;
		}else if(entity.type=="variable"){
			set<string> s = pkb->getAllVars();
			set<string> *t = new set<string>;
			for(set<string>::iterator it=s.begin();it!=s.end();it++){
				t->insert(*it);
			}
			valueTable[entity.synonym] = t;
			
		}else if(entity.type=="constant"){
			set<string> s = pkb->getAllConstants();
			set<string> *t = new set<string>;
			for(set<string>::iterator it=s.begin();it!=s.end();it++){
				t->insert(*it);
			}
			valueTable[entity.synonym] = t;
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


bool QueryEvaluator::processGroupedRelations(){
    ResultsTable temp_results_table;
    
    for(list<list<BaseRelation*>>::iterator it = grouped_relations.begin();it!=grouped_relations.end();it++){
        vector<pair<string,string>> result_pairs;
        ResultsTable temp_table;//for each group
        list<BaseRelation*> relations = *it;//for each group
        for(list<BaseRelation*>::iterator iter = relations.begin();iter!=relations.end();iter++){//for each relation
            BaseRelation* relation = *iter;
            if(relation->type=="designAbstraction")
            {
                designAbstraction* da = static_cast<designAbstraction*>(relation);
                
                result_pairs = processDesignAbstraction(da);
                pair<string,string>ref_pair;
                if(result_pairs.empty())
                    return false;
                else{
                    ref_pair.first = da->ref1;
                    ref_pair.second = da->ref2;
                    temp_table.join(ref_pair, result_pairs);
                }

            }
            else if(relation->type=="pattern"){
                
                pattern* p = static_cast<pattern*>(relation);
                result_pairs  = processPattern(p);
                
                if(result_pairs.empty())
                    return false;
                           
            }//
        }//for each relation
        temp_table.eliminateColumns(result_refs);
        temp_results_table.merge(temp_table);
    }//for each group
    resultTable=temp_results_table;y
    return true;
}


list<string> QueryEvaluator::getResults(){
	string r = result_refs.at(0);
	list<string> res;
	if(r=="BOOLEAN"){
		res.push_back("true");
		return res;
	}
	set<string> s = *valueTable[r];
	
	for(set<string>::iterator it = s.begin();it!=s.end();it++){
		res.push_back(*it);
	}
	return res;
}

