
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
		result_refs_complex = Qprocessor->result_reffs;
		for(unsigned int i=0;i<Qprocessor->result_reffs.size();i++){
			string ref = Qprocessor->result_reffs[i];
			int p  = ref.find(".");
			if(p<ref.size()){
				ref = ref.substr(0,p);			
			}
			result_refs.push_back(ref);
		}
		constant_relations = Qprocessor->constant_relations;
		grouped_relations =  Qprocessor->grouped_relations;
        //start to evaluate query
        initialzeValueTable();
		constant_pass= processConstantRelations();
        if(constant_pass){     
			grouped_pass = processGroupedRelations();
			if(grouped_pass){
				
				if(grouped_relations.size()>0){
					clock_t t;
					t = clock();
					results = validateResults();
					t = clock()-t;
					printf ("getResult took (%f seconds).\n",((float)t)/CLOCKS_PER_SEC);
				}
					
				else 
					results = getResultsFromValueTable();
					
			
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


bool QueryEvaluator::processGroupedRelations(){

	if(grouped_relations.size()==0)
		return true;
    
    for(list<list<BaseRelation*>>::iterator it = grouped_relations.begin();it!=grouped_relations.end();it++){
        vector<pair<string,string>> result_pairs;
        ResultsTable temp_table;//for each group
        list<BaseRelation*> relations = *it;//for each group
        for(list<BaseRelation*>::iterator iter = relations.begin();iter!=relations.end();iter++){//for each relation
            BaseRelation* relation = *iter;
			pair<string,string>ref_pair;
            if(relation->type=="designAbstraction")
            {
                designAbstraction* da = static_cast<designAbstraction*>(relation);
               
                result_pairs = processDesignAbstraction(da);
               
				 //clock_t t;
				//t = clock();
                if(result_pairs.empty())
                    return false;
                else{
                    ref_pair.first = da->ref1;
                    ref_pair.second = da->ref2;
					
                    temp_table.join(ref_pair, result_pairs);
					 
					if (temp_table.tuples.size()==0) {
						return false;
					}
                }
				//t = clock()-t;
				//printf ("join took (%f seconds).\n",((float)t)/CLOCKS_PER_SEC);
            }
            else if(relation->type=="pattern"){
                
                pattern* p = static_cast<pattern*>(relation);
                result_pairs  = processPattern(p);
                
                if(result_pairs.empty())
                    return false;
				else{
					ref_pair.first = p->synonym;
					ref_pair.second = p->varRef;
                    temp_table.join(ref_pair, result_pairs);
					if (temp_table.tuples.size()==0) {
						return false;
					}
                }
                           
            }else if(relation->type=="attr_compare"){
				attr_compare* attr_pairs = static_cast<attr_compare*>(relation);
				result_pairs = processAttrCompare(attr_pairs);
				if(result_pairs.empty())
					return false;
				else{
					ref_pair.first = attr_pairs->left_ref;
					ref_pair.second = attr_pairs->right_ref;
                    temp_table.join(ref_pair, result_pairs);
					if (temp_table.tuples.size()==0) {
						return false;
					}
                }
			}
        }//for each relation
        
        
										
        temp_table.eliminateColumns(result_refs);
        resultTable.merge(temp_table);
       
		
    }//for each group
    //resultTable=temp_results_table;
    
    return true;
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
	}else if(relation=="Calls*"){
		res = pkb-> getCallsT(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
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
	}else if(relation=="Next*"){
		res = pkb-> getNextT(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation=="Affects"){
		res = pkb-> getAffects(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation=="Affects*"){
		res = pkb-> getAffectsT(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation =="NextBip"){
		res = pkb-> getNextBip(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation =="NextBip*"){
		res = pkb-> getNextTBip(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation =="AffectsBip"){
		res = pkb-> getAffectsBip(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}else if(relation =="AffectsBip*"){
		res = pkb-> getAffectsTBip(&ref1_set, da->ref1_type, &ref2_set, da->ref2_type);
	}

	if(da->ref1==da->ref2&&da->ref1_type==da->ref2_type){  // ref1 is the same as ref2
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
	}else if(relation == "Calls*"){
		return pkb-> checkCallsT(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Parent"){
		return pkb-> checkParent(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Parent*"){
		return pkb-> checkParentT(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation =="Follows"){
		return pkb->checkFollow(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation =="Follows*"){
		return pkb->checkFollowT(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Next"){
		return pkb-> checkNext(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Next*"){
		return pkb-> checkNextT(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Affects"){
		return pkb-> checkAffects(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "Affects*"){
		return pkb-> checkAffectsT(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "NextBip"){
		return pkb-> checkNextBip(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "NextBip*"){
		return pkb-> checkNextTBip(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "AffectsBip"){
		return pkb-> checkAffectsBip(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
	}else if(relation == "AffectsBip*"){
		return pkb-> checkAffectsTBip(da->ref1, da->ref1_type, da->ref2, da->ref2_type);
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
		
		if(p->expr_tree =="  "){ // expr_spec "_"
			for(set<string>::iterator it =s.begin();it!=s.end();it++){
				pair<string,string> pa(*it,"");
				result.push_back(pa);
			}
		}
		else {
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
	}
	return result;
}

vector<pair<string,string>> QueryEvaluator::patternIfOrWhile(pattern* p){
	
	vector<pair<string,string>> result;
	set<string> s = *valueTable[p->synonym];
	if(p->varRef_type=="variable"){
		for(set<string>::iterator it = s.begin();it!=s.end();it++){
			int lineno = Util::convertStringToInt(*it);
			// lineno - control variable
			string var = pkb->getControlVariable(lineno);
			pair<string,string> * pa = new pair<string,string>(*it,var);
			result.push_back(*pa);
	
		}
	}else if(p->varRef =="_"){ //varRef = "_"
		
		for(set<string>::iterator it = s.begin();it!=s.end();it++){
			string a = *it;
		
			pair<string,string> * pa = new pair<string,string>(a,"");
			result.push_back(*pa);

		}
	}else {
		for(set<string>::iterator it = s.begin();it!=s.end();it++){
			int lineno = Util::convertStringToInt(*it);
			// lineno - control variable
			string var = pkb->getControlVariable(lineno);
			if(var ==p->varRef){
				pair<string,string> * pa = new pair<string,string>(*it,var);
				result.push_back(*pa);
			}
		}
	}
	return result;
}


vector<pair<string,string>> QueryEvaluator::processAttrCompare(attr_compare* a){
	vector<pair<string,string>> result;
	set<string> s1 = getValueSet(a->left_ref,a->left_ref_type,"");
	set<string> s2 = getValueSet(a->right_ref,a->right_ref_type,"");
	string evaluation_type = a->evaluation_type;
	set<string>::iterator it2;

	if((a->left_ref_type=="call"||a->right_ref_type=="call")&&evaluation_type == "string"){
		if(a->left_ref_type=="call"&&a->right_ref_type=="call"){
			for(set<string>::iterator it = s1.begin();it!=s1.end();it++){
				it2 = s2.find(*it);
				if(it2!=s2.end()){
					pair<string,string> p(*it,*it);
					result.push_back(p);
				}
			}
		}else if(a->left_ref_type=="call"){
			for(set<string>::iterator it = s1.begin();it!=s1.end();it++){
				string temp = *it;
				int lineno = Util::convertStringToInt(temp);
				string proc = pkb->procAtLine[lineno];
				it2 = s2.find(proc);
				if(it2!=s2.end()){
					pair<string,string> p(*it,proc);
					result.push_back(p);
				}
			}
		}else {
			for(set<string>::iterator it = s2.begin();it!=s2.end();it++){
				string temp = *it;
				int lineno = Util::convertStringToInt(temp);
				string proc = pkb->procAtLine[lineno];
				it2 = s1.find(proc);
				if(it2!=s1.end()){
					pair<string,string> p(proc,*it);
					result.push_back(p);
				}
			}
		}
		
	}else {

		for(set<string>::iterator it = s1.begin();it!=s1.end();it++){
			it2 = s2.find(*it);
			if(it2!=s2.end()){
				pair<string,string> p(*it,*it);
				result.push_back(p);
			}
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
		}else if(entity.type=="stmtLst"){
			vector<int> s = pkb->getFirstStmtList();
			set<string> *t = new set<string>;
			for(unsigned int i=0;i<s.size();i++){
				string temp = Util::convertIntToString(s[i]);
				t->insert(temp);
			}
			valueTable[entity.synonym] = t;
		}else{
			vector<int> stmts;
			if(entity.type=="prog_line")
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


list<string> QueryEvaluator::validateResults(){
    
    // if (resultTable.columns.size()<result_refs.size()) {
	bool change_proc = false;
	//bool inside_result = false;
	vector<int> change_proc_index;
	for(int z =0; z<result_refs_complex.size();z++){
		if(result_refs_complex.at(z)!=result_refs.at(z)){
			change_proc =true;
			if(resultTable.findColumn(result_refs.at(z))!=-1){
				change_proc_index.push_back(z);
				//inside_result = true;
			}
		}
	}
	if(change_proc && change_proc_index.size()>0){//check if there is c.proc inside results_reff_complex
		//change the line number to proc name
		for(list<vector<string>>::iterator it=resultTable.tuples.begin();it!=resultTable.tuples.end();it++){
			for(int m = 0;m<change_proc_index.size();m++){
				int c_ind = change_proc_index[m];
				int ind = Util::convertStringToInt((*it).at(c_ind));
				string proc = pkb->procAtLine[ind];
				(*it).at(change_proc_index.at(m))=proc;
			}
		}
	}

	list<string> res;
	if (result_refs.at(0)=="BOOLEAN") {
		return res;
    }

	// initialize refs_appereared as 0
	map<string,int> refs_appeared;
	for(unsigned int i=0;i<result_refs.size();i++){ //simple form,include call.procName
		refs_appeared[result_refs[i]] = 0;
	}
	// cartesian flag
   	bool cartesian_flag = false;
	// tuple_used flag
	bool tuple_used = false;


	/// compute the first synonym in result refs
	string first = result_refs_complex[0];
	int p0 = first.find(".");
	if(p0<first.size()){
		string synonym = first.substr(0,p0);
		refs_appeared[synonym] = 1; //set ref appeared
		int index = resultTable.findColumn(synonym);
		if(index==-1){
			
			set<string> s = *valueTable[synonym];
			for(set<string>::iterator it = s.begin();it!=s.end();it++){
				int ind = Util::convertStringToInt(*it);
				string proc = pkb->procAtLine[ind];
				res.push_back(proc);
			}
		}else{
			tuple_used = true;
			for(list<vector<string>>::iterator it=resultTable.tuples.begin();it!=resultTable.tuples.end();it++){
				vector<string> tuple = *it;
				res.push_back(tuple.at(index));
			}
		}
	}else {
		refs_appeared[first] = 1; //set ref appeared
		int index = resultTable.findColumn(first);
		if(index==-1){
			set<string> s = *valueTable[first];
			for(set<string>::iterator it = s.begin();it!=s.end();it++){		
				res.push_back(*it);
			}
		}else{
			tuple_used = true;
			for(list<vector<string>>::iterator it=resultTable.tuples.begin();it!=resultTable.tuples.end();it++){
				vector<string> tuple = *it;
				res.push_back(tuple.at(index));
			}
		}
		
	}

	/// main loop
	for (int i=1; i<result_refs.size(); i++) {
		string ref = result_refs_complex.at(i);

		list<string> temp;	
			
		int p = ref.find(".");

		if(p<ref.size()){ //ref is call.procName
			string synonym = ref.substr(0,p);
			int index = resultTable.findColumn(synonym);
			
			if(index==-1){ //not in resultTable
				set<string> s = *valueTable[synonym];
				if(refs_appeared[synonym]){ // appeared, not do cartesian product
					if(cartesian_flag){ // count and jump 
						int move = 0;
						for(set<string>::iterator it = s.begin();it!=s.end();it++){
							int ind = Util::convertStringToInt(*it);
							string proc = pkb->procAtLine[ind];

							int count = s.size()+move;
							for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
								if(count==s.size()){
									string ref1 = *it_list;
					
									ref1.append(" ");
									ref1.append(proc);
									temp.push_back(ref1);
								}
								count--;
								if(count==move){								
									count = s.size()+move;
								}								
							}
							move++;
						}
						res = temp;

					}else {// not been cross producted, append itself
							
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
							string ref1 = *it_list;

							int ind = Util::convertStringToInt(ref1);
							string proc = pkb->procAtLine[ind];

							ref1.append(" ");
							ref1.append(proc);   //append itself.procName
							temp.push_back(ref1);
						
						}
						res = temp;
					}

				}else {// not appeared,cartesian product

					cartesian_flag = true;
					set<string> s = *valueTable[synonym];
					for(set<string>::iterator it = s.begin();it!=s.end();it++){
						int ind = Util::convertStringToInt(*it);
						string proc = pkb->procAtLine[ind];
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
				
							string ref1 = *it_list;
					
							ref1.append(" ");
							ref1.append(proc);
							temp.push_back(ref1);
						}
					}
					res = temp;
					refs_appeared[synonym] = 1;
				}

			}else{ // get from resultTable
				if(refs_appeared[synonym]){ // appeared, not do cartesian product
					if(cartesian_flag){ //count and jump
						int move = 0;
						for(list<vector<string>>::iterator it=resultTable.tuples.begin();it!=resultTable.tuples.end();it++){
							vector<string> tuple = *it;
							int ind = Util::convertStringToInt(tuple.at(index));
							string proc = pkb->procAtLine[ind];

							int count = resultTable.tuples.size()+move;
					
							for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
								if(count==resultTable.tuples.size()){
									string ref1 = *it_list;					
									ref1.append(" ");
									ref1.append(proc);
									temp.push_back(ref1);
								}
								count--;
								if(count==move){								
									count = resultTable.tuples.size()+move;
								}							
							}
							move++;
						}
						res  = temp;

					}else { //append
						list<vector<string>>::iterator it_tuples = resultTable.tuples.begin();
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
							string ref1 = *it_list;
							vector<string> t = *it_tuples;
							ref1.append(" ");
							int ind = Util::convertStringToInt(t.at(index));
							string proc = pkb->procAtLine[ind];
							ref1.append(proc);
							temp.push_back(ref1);
							it_tuples++;
						}
						res = temp;
					}
				}else {// not appeared
					refs_appeared[synonym] = 1; //set ref appeared
					if(tuple_used&&cartesian_flag){ //resultTable in use, count and jump
						int move = 0;
						for(list<vector<string>>::iterator it=resultTable.tuples.begin();it!=resultTable.tuples.end();it++){
							vector<string> tuple = *it;
							int ind = Util::convertStringToInt(tuple.at(index));
							string proc = pkb->procAtLine[ind];
							int count = resultTable.tuples.size()+move;
					
							for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
								if(count==resultTable.tuples.size()){
									string ref1 = *it_list;
					
									ref1.append(" ");
									ref1.append(proc);
									temp.push_back(ref1);
								}
								count--;
								if(count==move){
								
									count = resultTable.tuples.size()+move;
								}
								
							}
							move++;
						}
						res= temp;

					}else if(tuple_used&&!cartesian_flag){ //append
						list<vector<string>>::iterator it_tuples = resultTable.tuples.begin();
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
							string ref1 = *it_list;
							vector<string> t = *it_tuples;
							ref1.append(" ");
							int ind = Util::convertStringToInt(t.at(index));
							string proc = pkb->procAtLine[ind];
							ref1.append(proc);
							temp.push_back(ref1);
							it_tuples++;
						}
						res = temp;

					}else { // first time resultTable in use, cartesian, no matter cartesian_flag
						tuple_used = true;
						cartesian_flag = true;
						for(list<vector<string>>::iterator it_tuples = resultTable.tuples.begin();it_tuples!=resultTable.tuples.end();it_tuples++){
							for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
								string ref1 = *it_list;
								vector<string> t = *it_tuples;
								ref1.append(" ");
								int ind = Util::convertStringToInt(t.at(index));
								string proc = pkb->procAtLine[ind];
								ref1.append(proc);
								temp.push_back(ref1);
								
							}
						}
						res = temp;
						cartesian_flag = true;
					}
					
				}
			}

		}else{ // ref is snynonym
			int index = resultTable.findColumn(ref);
			if(index ==-1){ //not in resultTable
				set<string> s = *valueTable[ref];
				if(refs_appeared[ref]){ // appeared, not do cartesian product
					if(cartesian_flag){ // count and jump 
						int move = 0;
						for(set<string>::iterator it = s.begin();it!=s.end();it++){

							int count = s.size()+move;
							for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
								if(count==s.size()){
									string ref1 = *it_list;
									string ref2 = *it;
									ref1.append(" ");
									ref1.append(ref2);
									temp.push_back(ref1);
								}
								count--;
								if(count==move){								
									count = s.size()+move;
								}								
							}
							move++;
						}
						res = temp;

					}else {// not been cross producted, append itself
						set<string>::iterator it = s.begin();	
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
							string ref1 = *it_list;
						
							ref1.append(" ");
							ref1.append(*it);   //append itself
							temp.push_back(ref1);
							it++;
						}
						res = temp;
					}

				}else {// do cartesian product,set flag

					cartesian_flag = true;
					set<string> s = *valueTable[ref];
					for(set<string>::iterator it = s.begin();it!=s.end();it++){
						
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
				
							string ref1 = *it_list;
							string ref2 = *it;
							ref1.append(" ");
							ref1.append(ref2);
							temp.push_back(ref1);
						}
					}
					res = temp;
					refs_appeared[ref] = 1;
				}

			}else { //in resultTable
				if(refs_appeared[ref]){ // appeared, not do cartesian product
					if(cartesian_flag){ //count and jump
						int move = 0;
						for(list<vector<string>>::iterator it=resultTable.tuples.begin();it!=resultTable.tuples.end();it++){
							vector<string> tuple = *it;
							int count = resultTable.tuples.size()+move;
					
							for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
								if(count==resultTable.tuples.size()){
									string ref1 = *it_list;
					
									ref1.append(" ");
									ref1.append(tuple.at(index));
									temp.push_back(ref1);
								}
								count--;
								if(count==move){
								
									count = resultTable.tuples.size()+move;
								}							
							}
							move++;
						}
						res  = temp;

					}else { //append
						list<vector<string>>::iterator it_tuples = resultTable.tuples.begin();
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
							string ref1 = *it_list;
							vector<string> t = *it_tuples;
							ref1.append(" ");
							ref1.append(t.at(index));
							temp.push_back(ref1);
							it_tuples++;
						}
						res = temp;
					}
				}else {// not appeared
					refs_appeared[ref] = 1; //set ref appeared

					if(tuple_used && cartesian_flag){ // count and jump
						int move = 0;
						for(list<vector<string>>::iterator it=resultTable.tuples.begin();it!=resultTable.tuples.end();it++){
							vector<string> tuple = *it;
							int count = resultTable.tuples.size()+move;
					
							for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
								if(count==resultTable.tuples.size()){
									string ref1 = *it_list;
					
									ref1.append(" ");
									ref1.append(tuple.at(index));
									temp.push_back(ref1);
								}
								count--;
								if(count==move){
								
									count = resultTable.tuples.size()+move;
								}
								
							}
							move++;
						}
						res= temp;
					}else if(tuple_used& !cartesian_flag){ // append
						list<vector<string>>::iterator it_tuples = resultTable.tuples.begin();
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
							string ref1 = *it_list;
							vector<string> t = *it_tuples;
							ref1.append(" ");
							ref1.append(t.at(index));
							temp.push_back(ref1);
							it_tuples++;
						}
						res = temp;
					}else { // resultTable first time use, cartesian
						tuple_used = true;
						cartesian_flag = true;
						for(list<vector<string>>::iterator it_tuples = resultTable.tuples.begin();it_tuples!=resultTable.tuples.end();it_tuples++){
							for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
								string ref1 = *it_list;
								vector<string> t = *it_tuples;
								ref1.append(" ");
								ref1.append(t.at(index));
								temp.push_back(ref1);
								
							}
						}
						res = temp;
					}
				}
			}
		}					
	}
	return res;
}


list<string> QueryEvaluator::getResultsFromValueTable(){
	
	list<string> res;
	// initialize refs_appereared as 0
	map<string,int> refs_appeared;
	for(unsigned int i=0;i<result_refs.size();i++){ //simple form,include call.procName
		refs_appeared[result_refs[i]] = 0;
	}

	//cartesian flag
	bool cartesian_flag = false;

	string first = result_refs_complex[0];
	
	if(first=="BOOLEAN"){
		
		return res;

	} 

	int p0 = first.find(".");
	if(p0<first.size()){ //call.procName
		string str = first.substr(0,p0);
		refs_appeared[str] = 1; //set ref appeared
		set<string> s = *valueTable[str];
		for(set<string>::iterator it = s.begin();it!=s.end();it++){
			int ind = Util::convertStringToInt(*it);
			string proc = pkb->procAtLine[ind];
			res.push_back(proc);
		}
		

	}else {
		refs_appeared[first] = 1; //set ref appeared
		set<string> s = *valueTable[first];
		for(set<string>::iterator it = s.begin();it!=s.end();it++){
			res.push_back(*it);
		}
	}
	// main loop	
	for(unsigned int i=1;i<result_refs_complex.size();i++){
		list<string> temp;
		string ref = result_refs_complex[i];
		int p = ref.find(".");
		if(p<ref.size()){ //call.procName
			string str = ref.substr(0,p0);
			set<string> s = *valueTable[str];
			if(refs_appeared[str]){ // appeared,can't do cartesian product
				if(cartesian_flag){
					int move = 0;
					for(set<string>::iterator it = s.begin();it!=s.end();it++){
						int ind = Util::convertStringToInt(*it);
						string proc = pkb->procAtLine[ind];

						int count = s.size()+move;
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
							if(count==s.size()){
								string ref1 = *it_list;
					
								ref1.append(" ");
								ref1.append(proc);
								temp.push_back(ref1);
							}
							count--;
							if(count==move){
								
								count = s.size()+move;
							}
								
						}
						move++;
					}
					res = temp;

				}else {
					set<string>::iterator it = s.begin();
					for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
						string ref1 = *it_list;
						
						ref1.append(" ");
						ref1.append(*it);   //append itself
						temp.push_back(ref1);
						it++;
						
					}
					res = temp;
				}
				
			}else{ // not appeared, cartesian product
				cartesian_flag = true;
				for(set<string>::iterator it = s.begin();it!=s.end();it++){
					int ind = Util::convertStringToInt(*it);
					string proc = pkb->procAtLine[ind];
					for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
				
						string ref1 = *it_list;
					
						ref1.append(" ");
						ref1.append(proc);
						temp.push_back(ref1);
					}
				}
				res = temp;
				refs_appeared[str] = 1; //set ref appeared
			}
			
		}else{  // ref is synonym

			set<string> ref_set = *valueTable[ref];
			if(refs_appeared[ref]){
				if(cartesian_flag){
					int move = 0;
					for(set<string>::iterator it = ref_set.begin();it!=ref_set.end();it++){
						int count = ref_set.size()+move;
						for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
							if(count==ref_set.size()){
								string ref1 = *it_list;
								string ref2 = *it;
								ref1.append(" ");
								ref1.append(ref2);
								temp.push_back(ref1);
							}
							count--;
							if(count==move){
								
								count = ref_set.size()+move;
							}
								
						}
						move++;
					}
					res = temp;

				}else {
					set<string>::iterator it = ref_set.begin();
					for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
						string ref1 = *it_list;
						
						ref1.append(" ");
						ref1.append(*it);   //append itself
						temp.push_back(ref1);
						it++;
					}
					res = temp;
				}
				
			}else { // cartesian product
				cartesian_flag = true;
				for(set<string>::iterator it = ref_set.begin();it!=ref_set.end();it++){
					for(list<string>::iterator it_list = res.begin();it_list!=res.end();it_list++){
				
						string ref1 = *it_list;
						string ref2 = *it;
						ref1.append(" ");
						ref1.append(ref2);
						temp.push_back(ref1);
					}
				}
				res = temp;
				refs_appeared[ref] = 1; //set ref appeared
			}
			
		}
		
	}

	return res;
}
