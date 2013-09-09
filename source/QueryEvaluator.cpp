
#include "QueryEvaluator.h"
int TRUE_VALUE = -999999999;

QueryEvaluator::QueryEvaluator(PKB* a){
	QueryEvaluator::has_pattern = false;
	QueryEvaluator::has_relation = false;
	QueryEvaluator::has_pattern_result = false;
	QueryEvaluator::pkb = a;
	
}

//used for compare
template<typename T>
int QueryEvaluator::compare(T a, T b) {
	if (a < b) return -1;
	if (b < a) return 1;
	return 0;
}
QueryPreprocessor::entityReff QueryEvaluator::getEntity(string synonym){
	QueryPreprocessor::entityReff null;
	if(synonym.at(0)=='"'&&synonym.at(synonym.size()-1)=='"'){
		QueryPreprocessor::entityReff temp;
		temp.type="constant";
		return temp;
	}
	if(Qprocessor.check_Integer(synonym)){
		QueryPreprocessor::entityReff temp;
		temp.type="constant";
		return temp;
	}
	if(synonym.compare("_")==0){
		QueryPreprocessor::entityReff temp;
		temp.type="_";
		return temp;
	}
	for(int i = 0;i < entity.size();i++){
		if (entity.at(i).synonym == synonym){
			if(entity.at(i).type == "prog_line"){
				QueryPreprocessor::entityReff temp;
				temp.type="stmt";
				return temp;
			}
			else
				return entity.at(i);
		}
		else{
			continue;
		}
	}
	return null;
}
string QueryEvaluator::getType(string synonym){
	string null;
	if(synonym.compare("_")==0)
		return "stmt";
	else if(Qprocessor.check_Integer(synonym))
		return "contant";

	else for(int i = 0;i < QueryEvaluator::entity.size();i++){
		if (QueryEvaluator::entity.at(i).synonym == synonym)
			return QueryEvaluator::entity.at(i).type;
		
	}
	return null;
}

//check the type of each colunm
//string 1
//int 2
//error 0

bool QueryEvaluator::isConstant(string synonym){
	if(synonym.at(0)=='"'&&synonym.at(synonym.size()-1)=='"'){
		return true;
	}
	if(Qprocessor.check_Integer(synonym)){
		return true;
	}
	return false;
}

list<string> QueryEvaluator::processQuery(string query){
	bool valid;
	valid = Qprocessor.process_query(query);
	QueryEvaluator::Colunm<int> with_result;
	QueryEvaluator::Colunm<int> pattern_result;
	QueryEvaluator::Colunm<int> relation_result;
	QueryEvaluator::Colunm<int> query_result;
	QueryEvaluator::Colunm<int> select_result;
	//cout<<valid;
	if(valid){
		//get all the query elements
		entity = Qprocessor.getSymbols();//declareation type, name
		result = Qprocessor.getResults();//select clause
		desAbstr = Qprocessor.getRelations();
		pattern = Qprocessor.getPatterns();
		//start to evaluate query
		//Qprocessor.print_query();

		//check if there is pattern
		if(pattern.size()!=0){
			has_pattern = true;
			pattern_result = evaluatePattern();
			if(pattern_result.cell.size()!=0)
				QueryEvaluator::has_pattern_result = true;
		}
		//check if there is design abstraction
		if(desAbstr.size()!=0){
			QueryEvaluator::has_relation = true;
		}
		string select_type = QueryEvaluator::getEntity(result.at(0)).type;
		if(select_type=="constant")
				select_type = "assign";
		if(QueryEvaluator::has_relation){//if there is design abstraction
			if(QueryEvaluator::has_pattern){//if there is a pattern
				if(QueryEvaluator::has_pattern_result){//if there is pattern result
					if(pattern.at(0).synonym.compare(desAbstr.at(0).ref1)==0){//relation(a,b) pattern a
						if(desAbstr.at(0).ref1.compare(result.at(0))==0){//if select a relation(a,b) pattern a
							string evaluate_type = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;

							query_result=QueryEvaluator::evaluateRelation(2,select_type,evaluate_type,pattern_result,1);
						}
						else if(desAbstr.at(0).ref2.compare(result.at(0))==0){//if select b relation(a,b) pattern a
							string evaluate_type = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;

							query_result=QueryEvaluator::evaluateRelation(1,select_type,evaluate_type, pattern_result,1);
						}
						else{ //if select c relation(a,b) pattern a
							string evaluate_type1 = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;
							string evaluate_type2 = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;
							query_result=evaluateRelation(0,evaluate_type1,evaluate_type2,pattern_result,1);
							if(query_result.cell.size()>0)
								query_result =QueryEvaluator::getAll(select_type);
						}
					}//if relation(a,b) pattern a
					else if(pattern.at(0).synonym.compare(desAbstr.at(0).ref2)==0){//relation(a,b) pattern b
						if(desAbstr.at(0).ref1.compare(result.at(0))==0){//if select a relation(a,b) pattern b
							string evaluate_type = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;
							query_result=QueryEvaluator::evaluateRelation(2,select_type,evaluate_type,pattern_result,2);
							if(query_result.cell.size()==1&&*(query_result.cell.begin())==TRUE_VALUE)
								query_result=pattern_result;
						}
						else if(desAbstr.at(0).ref2.compare(result.at(0))==0){//if select b relation(a,b) pattern b
							string evaluate_type = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;
							query_result=QueryEvaluator::evaluateRelation(1,select_type,evaluate_type, pattern_result,2);
							if(query_result.cell.size()==1&&*(query_result.cell.begin())==TRUE_VALUE)
								query_result=pattern_result;
						}
						else{ //if select c relation(a,b) pattern b
							string evaluate_type1 = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;
							string evaluate_type2 = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;
							relation_result=QueryEvaluator::evaluateRelation(0,evaluate_type1,evaluate_type2,pattern_result,2);
							if(relation_result.cell.size()>0)
								query_result =QueryEvaluator::getAll(select_type);
						}
					}//else if relation(a,b) pattern b
					else{//relation(a,b) pattern c
						if(desAbstr.at(0).ref1.compare(result.at(0))==0){//if select a relation(a,b) pattern c
							string evaluate_type = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;
							query_result=QueryEvaluator::evaluateRelation(2,select_type,evaluate_type,pattern_result,3);
							if(query_result.cell.size()==1&&*(query_result.cell.begin())==TRUE_VALUE)
								query_result=pattern_result;
						}
						else if(desAbstr.at(0).ref2.compare(result.at(0))==0){//if select b relation(a,b) pattern c
							string evaluate_type = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;
							query_result=QueryEvaluator::evaluateRelation(1,select_type,evaluate_type,pattern_result,3);
							if(query_result.cell.size()==1&&*(query_result.cell.begin())==TRUE_VALUE)
								query_result=pattern_result;

						}
						else if(pattern_result.header.compare(result.at(0))==0){//if select c relation(a,b) pattern c
							string evaluate_type1 = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;
							string evaluate_type2 = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;
							query_result=QueryEvaluator::evaluateRelation(0,evaluate_type1,evaluate_type2,pattern_result,3);
							if(query_result.cell.size()==1&&*(query_result.cell.begin())==TRUE_VALUE)
								query_result=pattern_result;

						}
						else{ //if select d relation(a,b) pattern c
							string evaluate_type1 = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;
							string evaluate_type2 = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;
							query_result=QueryEvaluator::evaluateRelation(0,evaluate_type1,evaluate_type2,pattern_result,4);
							if(query_result.cell.size()>0)
								query_result =QueryEvaluator::getAll(select_type);
						}
					}//else relation(a,b) pattern c
				}//if there is pattern result
			}//if there is a pattern
			else{//if there is no pattern
				if(desAbstr.at(0).ref1.compare(result.at(0))==0){//if select a relation(a,b)
					select_type= getType(result.at(0));
					string evaluate_type = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;
					
					query_result=QueryEvaluator::evaluateRelation(2,select_type,evaluate_type,pattern_result,0);

				}
				else if(desAbstr.at(0).ref2.compare(result.at(0))==0){//if select b relation(a,b)
					
					select_type= getType(result.at(0));
					string evaluate_type = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;
					query_result=QueryEvaluator::evaluateRelation(1,select_type,evaluate_type,pattern_result,0);

				}
				else{//if select c relation(a,b)
					select_type= getType(result.at(0));
					if(select_type=="constant")
						select_type = "assign";
					string evaluate_type1 = QueryEvaluator::getEntity(desAbstr.at(0).ref1).type;
					string evaluate_type2 = QueryEvaluator::getEntity(desAbstr.at(0).ref2).type;
					if(evaluate_type1.compare("constant")==0)
						evaluate_type1 = (*pkb).getStmtType(atoi(desAbstr.at(0).ref1.c_str()));
					if(evaluate_type2.compare("constant")==0)
						evaluate_type2 = (*pkb).getStmtType(atoi(desAbstr.at(0).ref2.c_str()));
					relation_result=QueryEvaluator::evaluateRelation(0,evaluate_type1,evaluate_type2,pattern_result,0);
					if(relation_result.cell.size()>0)
						query_result =QueryEvaluator::getAll(select_type);
				}
			}
		}//if there is deAbs
		else{//if there is no deAbs
			select_type= getType(result.at(0));
			if(select_type=="constant")
				select_type = "assign";
			if(QueryEvaluator::has_pattern){//if there is a pattern
				if(QueryEvaluator::has_pattern_result){//if there is pattern result
					if(pattern_result.header.compare(result.at(0))==0)//if select a pattern a
						query_result = pattern_result;
					else//if select b pattern a
						query_result =QueryEvaluator::getAll(select_type);
				}//if there is pattern result
			}//if there is a pattern
			else//select *
				query_result =QueryEvaluator::getAll(select_type);
		}//else //if there is no deAbs
	}//if valid
	
	QueryEvaluator::Colunm<int> constant_result;
	if((getEntity(result.at(0)).type.compare("constant")==0)&&query_result.cell.size()>0){
		vector<int> all = (*pkb).getStmtNo("assign");
		int i;
		int j;
		for(i=0;i<all.size();i++){
			vector<int> constant = (*pkb).getConst(all.at(i));
			if(constant.size()>0&&constant.at(0)!=-1)
				for(j=0;j<constant.size();j++)
				constant_result.cell.insert(constant.at(j));
		}
		query_result=constant_result;
	}
	list<string> list;
	for (set<int>::iterator g = query_result.cell.begin(); g != query_result.cell.end(); g++) {
		if(getEntity(result.at(0)).type.compare("variable")==0)
			list.push_back((*pkb).getVarName(*g));
		else{
			int element = *g;
			ostringstream ss;
			ss << element;
			list.push_back(ss.str());
		}
	}
	return list;
}


QueryEvaluator::Colunm<int> QueryEvaluator::getAll(string type){
	QueryEvaluator::Colunm<int> result;
	if(type.compare("bool")==0){
		result.cell.insert(1);
	}
	else if (type.compare("variable")==0){
		int i;
		for(i=0;i<(*pkb).getSizeVarTable();i++){
			result.cell.insert(i);
		}
	}
	else{
		unsigned int i;
		vector<int> all;
		all = (*pkb).getStmtNo(type);
		for(i=0;i<all.size();i++){
			result.cell.insert(all.at(i));
		}
	}
	return result;
}

QueryEvaluator::Colunm<int> QueryEvaluator::evaluatePattern(){
	QueryEvaluator::Colunm<int> result;
	
	unsigned int i;

	for(i=0;i<(*pkb).getAST().size();i++){
		//set result.header info
		result.header = pattern.at(0).synonym;
		result.type = pattern.at(0).type;

		TNode* ast = (*pkb).getAST().at(i);
		if(QueryEvaluator::compareAST(ast, pattern.at(0))){
			result.cell.insert((*pkb).getLineNo((*pkb).getAST().at(i)));
		}

	}//for i
	return result;
}
bool QueryEvaluator::compareAST(TNode* ast, QueryPreprocessor::pattern pattern){
	bool result = false;

	TNode* left = (*pkb).getLeftChild(ast); //left of root
	TNode* right = (*pkb).getRightChild(ast);//right of root

	if(pattern.varRef.compare("_")==0){//if pattern(_,)  
		if(pattern.exact){//if pattern(_,"a")
			if (pattern.expr_tree.content.compare((*pkb).getVarName((*pkb).getContent(right)))==0)
				return true;		
		}
		else{
			if(pattern.expr_tree.content.compare("")==0){//if pattern(_,_) 
				return true;
			}
			else{                                   //else pattern(_,_"x"_)
				flag=false;
				compareAST_Node(right,pattern);
				return flag;
			}//else pattern(_,"_x_")
		}//else
	}//if pattern(_,)
	else{   //else pattern("a",)
		//	cout<<(*pkb).getVarName((*pkb).getContent(left))<<"\n"<<pattern.varRef<<"\n";
		vector<string> var;
		if(isConstant(pattern.varRef)){
			var.push_back(pattern.varRef.substr(1,pattern.varRef.size()-2));
		}
		else{
			int i;
			for(i=0;i<(*pkb).getSizeVarTable();i++)
				var.push_back((*pkb).getVarName(i));
		}
		int j;
		for(j=0;j<var.size();j++){
			string varName = (*pkb).getVarName((*pkb).getContent(left));
			if(varName.compare(var.at(j))==0){//if left="a"
				if(pattern.exact){//if pattern("a","b")
					if (pattern.expr_tree.content.compare((*pkb).getVarName((*pkb).getContent(right)))==0)
						return true;		
				}
				else{
					if(pattern.expr_tree.content.compare("")==0){//if pattern("a",_)  
						string varRef = pattern.varRef;
						if(isConstant(varRef)){
							varRef=varRef.substr(1,pattern.varRef.size()-2);
							int index = (*pkb).getContent(left);
							string content = (*pkb).getVarName(index);
							if(varRef.compare(content)==0)
								return true;
						}
						else 
							return true;
					}
					else{                                   //else pattern("a","_x_")
						flag=false;
						compareAST_Node(right,pattern);
						return flag;
					}//else pattern("a","_x_")
				}//else
			}//if pattern("a",)
		}//for all the var
	}// else pattern("a",)
	return false;
}
void QueryEvaluator::compareAST_Node(TNode* ast, QueryPreprocessor::pattern pattern){
	TNode* right = (*pkb).getRightChild(ast);
	TNode* left = (*pkb).getLeftChild(ast);
	if (left!=NULL && right!=NULL){ //if has children	
		if((*pkb).getType(right).compare("var")==0){
			if((*pkb).getVarName((*pkb).getContent(right)).compare(pattern.expr_tree.content)==0)	{
				flag=true;
				return;
			}
		}
		else
			compareAST_Node(right, pattern);	

		if((*pkb).getType(left).compare("var")==0){
			if((*pkb).getVarName((*pkb).getContent(left)).compare(pattern.expr_tree.content)==0){
				flag=true;
				return;
			}
		}
		else
			compareAST_Node(left, pattern);
	}
}

//evaluate select* relation(*,*) pattern()
QueryEvaluator::Colunm<int> QueryEvaluator::evaluateRelation(int evaluate_position,string select_type, string evaluate_type,
	QueryEvaluator::Colunm<int> pattern_result,int pattern_position){
		QueryEvaluator::Colunm<int> result;
		unsigned int i=0;
		bool flag=false;
		bool exists = false;
	
		
		vector<int> temp;
		for(i = 0;i < desAbstr.size();i++){
			
			if(desAbstr.at(i).type.compare( "ModifiesP")==0) {
				
				result = QueryEvaluator::evaluate_modify_uses(evaluate_position,select_type,evaluate_type,pattern_result,pattern_position, "modify");
			}//if modifiesS
			else if(desAbstr.at(i).type.compare( "UsesP")==0) {//if Uses
				result=QueryEvaluator::evaluate_modify_uses(evaluate_position,select_type,evaluate_type,pattern_result,pattern_position, "uses");
			}
			else if(desAbstr.at(i).type.compare( "Parent")==0) {//if Parent
				
				result=QueryEvaluator::evaluate_parents_follows(evaluate_position,select_type,evaluate_type,pattern_result,pattern_position, "parent");
			}//parent
			else if(desAbstr.at(i).type.compare( "ParentT")==0) {//if Parent*
				
				result=QueryEvaluator::evaluate_parents_follows(evaluate_position,select_type,evaluate_type,pattern_result,pattern_position, "parentT");
			}//parent*
			else if(desAbstr.at(i).type.compare( "Follows")==0) {//if follow
				
				result=QueryEvaluator::evaluate_parents_follows(evaluate_position,select_type,evaluate_type,pattern_result,pattern_position, "follow");
			}//follow
			else if(desAbstr.at(i).type.compare( "FollowsT")==0) {//if follow*
				
				result=QueryEvaluator::evaluate_parents_follows(evaluate_position,select_type,evaluate_type,pattern_result,pattern_position, "followT");
			}//follow*
		}//for all the desAbs
		return result;
}
QueryEvaluator::Colunm<int> QueryEvaluator::evaluate_modify_uses(int evaluate_position,string select_type, string evaluate_type,
	QueryEvaluator::Colunm<int> pattern_result,int pattern_position, string relation){
		QueryEvaluator::Colunm<int> result;
		unsigned int i=0;
		bool flag=false;
		bool exists = false;
		unsigned int k=0,m=0;
		int n=0;
		
		vector<int> temp;
		if(evaluate_position==2){
			if(select_type.compare("_")==0)
				select_type = "stmt";
			if(evaluate_type.compare("_")==0)
				evaluate_type="constant";
		}
		if(evaluate_position==1){
			if(select_type.compare("_")==0)
				select_type = "constant";
			if(evaluate_type.compare("_")==0)
				evaluate_type="stmt";
		}
		if(evaluate_position==0){
			if(select_type.compare("_")==0)
				select_type = "stmt";
			if(evaluate_type.compare("_")==0)
				evaluate_type="constant";
		}
		if(evaluate_position==2){//select a modify(a,b)
			
			if(pattern_position==1){//select a modify(a,b) pattern(a)
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {//for all the pattern result
					int element = *g;
					//get all the possible a
					if(relation.compare("modify")==0)
						temp = (*pkb).getModifiedStmt(element);//use the pattern result to evaluate Modify
					else if (relation.compare("uses")==0)
						temp = (*pkb).getUsedStmt(element);
					if(isConstant(desAbstr.at(i).ref2)){//if b is a constant
						string var = desAbstr.at(i).ref2.substr(1,desAbstr.at(i).ref2.size()-2);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								if(temp.at(k)==(*pkb).getVarIndex(var)&&temp.at(k)!=-1){ //compare it with ref1 result
									result.cell.insert(element); //add a
								}//if
							}
						}//for temp
					}//if ref2 is constant
					else{//if ref2 is variable
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){
								if(temp.at(k)!=-1)
									result.cell.insert(element);
							}
						}
					}//else
				}//for all the pattern result
			}//if select a modify(a,b) pattern(a)
			else if(pattern_position==2){//select a modify(a,b) pattern(b)
			}//this wont happened since b must be a var
			else if(pattern_position==3){//select a modify(a,b), patternC
				if(pattern_result.cell.size()>0){
					if(isConstant(desAbstr.at(i).ref2)){
						string var = desAbstr.at(i).ref2.substr(1,desAbstr.at(i).ref2.size()-2);
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedList((*pkb).getVarIndex(var),select_type);//get all the a with specific type
						else
							temp = (*pkb).getUsedList((*pkb).getVarIndex(var),select_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){
								if(temp.at(k)!=-1)//if not -1 add to result
									result.cell.insert(temp.at(k));
							}
						}
					}//if b is constant
					else{//if b is variable
						vector<int> all;
						temp = (*pkb).getStmtNo(select_type); //get all the a
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){
								if(relation.compare("modify")==0&&temp.at(k)!=-1)
									all = (*pkb).getModifiedStmt(temp.at(k)); //get possible a with b
								else if(relation.compare("uses")==0&&temp.at(k)!=-1)
									all = (*pkb).getUsedStmt(temp.at(k));
								if(all.size()>0){
									result.cell.insert(temp.at(k));
								}
							}
						}
					}//else b is variable
				}//if pattern has result
			}//if pattern_postion = 3
			else if(pattern_position==0){//select a modify(a,b)
				
				if(isConstant(desAbstr.at(i).ref2)){

					string var = desAbstr.at(i).ref2.substr(1,desAbstr.at(i).ref2.size()-2);
					
					int index = (*pkb).getVarIndex(var);
					if(index!=-1){
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedList(index,select_type);//get all the a with specific type
						else
							temp = (*pkb).getUsedList(index,select_type);
					}
					if(temp.size()>0){
						for(k=0;k<temp.size();k++){
							result.cell.insert(temp.at(k));
						}//for all the possible a
					}
				}//if b is constant
				else{//if b is variable
					vector<int> all;
					temp = (*pkb).getStmtNo(select_type);
					if(temp.size()>0){
						for(k=0;k<temp.size();k++){
							if(relation.compare("modify")==0)
								all = (*pkb).getModifiedStmt(temp.at(k));
							else
								all = (*pkb).getUsedStmt(temp.at(k));
							if(all.size()>0&&temp.at(k)!=-1)
								result.cell.insert(temp.at(k));
						}
					}//for all the result
				}//else b is variable
			}//if select a modify(a,b)
		}//if select a relation(a,b)
		else if(evaluate_position==1){//select b modify(a,b)
			if(pattern_position==1){//select b modify(a,b) pattern(a)
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
					int element = *g;
					if(isConstant(desAbstr.at(i).ref1)){
						string var = desAbstr.at(i).ref1.substr(1,desAbstr.at(i).ref1.size()-2);
						//wont happen in this case
					}//if ref1 is constant
					else{//if ref1 is variable
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedStmt(element);//use the pattern result to evaluate Modify
						else
							temp = (*pkb).getUsedStmt(element);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){
								if(temp.at(k)!=-1)
									result.cell.insert(temp.at(k));
							}//for
						}
					}//else
				}//for
			}//if select b modify(a,b) pattern(a)
			else if(pattern_position==2){//select b modify(a,b) pattern(b)
			}//this wont happened since b must be a var
			else if(pattern_position==3){//select b modify(a,b),pattern(c)
				if(isConstant(desAbstr.at(i).ref1)){ //select b modify(#TRUE_VALUE,b) pattern c
					if(relation.compare("modify")==0)
						temp = (*pkb).getModifiedStmt(atoi(desAbstr.at(i).ref1.c_str()));
					else
						temp = (*pkb).getUsedStmt(atoi(desAbstr.at(i).ref1.c_str()));
					if(temp.size()>0 && pattern_result.cell.size()>0){
						for(k=0;k<temp.size();k++){
							if(temp.at(k)!=-1)
								result.cell.insert(temp.at(k));
						}//for
					}
				}//if select b modify(#TRUE_VALUE,b) pattern c
				else{//if select b modify(a,b) pattern c
					if(pattern_result.cell.size()>0){
						temp = (*pkb).getStmtNo(evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){
								if(temp.at(k)!=-1)
									result.cell.insert(temp.at(k));
							}//for
						}
					}
				}//if select b modify(a,b) pattern c
			}
			else if(pattern_position==0){//select b modify(a,b)
				if(isConstant(desAbstr.at(i).ref2)){
					//not happened in this case
				}//if b is constant
				else{//if b is variable
					if(isConstant(desAbstr.at(i).ref1)){ //select b modify(#TRUE_VALUE,b)
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						else
							temp = (*pkb).getUsedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){
								if(temp.at(k)!=-1)
									result.cell.insert(temp.at(k));
							}//for
						}
					}//if select b modify(#TRUE_VALUE,b)
					else{//if select b modify(a,b)
						for(n=0;n<(*pkb).getSizeVarTable();n++){
							temp = (*pkb).getModifiedList(n,evaluate_type);
							if(temp.size()>0){
								for(k=0;k<temp.size();k++){
									if(temp.at(k)!=-1)
										result.cell.insert(n);
								}//for
							}
						}
					}//if select b modify(a,b) pattern c
				}//else
			}//if select b modify(a,b)
		}//else if select b modify(a,b)
		else if(evaluate_position==0){//select c relation(a,b)
			if(pattern_position==1){//select c relation(a,b) pattern a
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
					int element = *g;
					if(relation.compare("modify")==0)
						temp = (*pkb).getModifiedStmt(element);//use the pattern result to find all the a
					else
						temp = (*pkb).getUsedStmt(element);
					if(temp.size()>0){
						if(isConstant(desAbstr.at(i).ref1)){
							//wont happen in this case
						}//if ref1 is constant
						else{//if ref1 is variable
							if(!isConstant(desAbstr.at(i).ref2)){//if ref2 is variable
								for(k=0;k<temp.size();k++){
									for(m=0;m<(*pkb).getSizeVarTable();m++){//get all the variables
										if(temp.at(k)==m){            //if find one match, break;
											exists = true;
											break;
										}
									}//if m is inside result
									if(exists)
										break;
								}

							}//if ref2 is variable
							else{ //if ref2 is contant   selectype = evaluate_type1 evaluate type = evaluate type2
								vector<int> all;
								if(relation.compare("modify")==0)
									all = (*pkb).getModifiedList((*pkb).getVarIndex(desAbstr.at(i).ref2),select_type);//from b we get all the possible A
								else
									all = (*pkb).getUsedList((*pkb).getVarIndex(desAbstr.at(i).ref2),select_type);
								if(all.size()>0)
									exists = true;

							}//else if ref2 is constant
						}//if ref1 is variable
					}
					if(exists)//
						break;
				}//for all the pattern result
				if(exists){ //just make the length of result>0
					result.cell.insert(1);
					exists = false;
				}
			}//if no pattern position =1
			else if(pattern_position==2){//select c relation(a,b) pattern(b)
			}//this wont happened since b must be a var
			else if(pattern_position==3){//select c relation(a,b) pattern(c)
				if(isConstant(desAbstr.at(i).ref2)){ //select c modify(a,"b") selectype = evaluate_type1 evaluate type = evaluate type2
					string var = desAbstr.at(i).ref2.substr(1,desAbstr.at(i).ref2.size()-2);
					if(isConstant(desAbstr.at(i).ref1)){//if select c modify(#TRUE_VALUE,"b")
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						else
							temp = (*pkb).getUsedStmt(atoi(desAbstr.at(i).ref1.c_str()));

						if(temp.size()>0){
							for(k=0;k<temp.size();k++){
								if(temp.at(k)==(*pkb).getVarIndex(var)){
									result = pattern_result;
									break;
								}
							}
						}
					}
					else{
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedList((*pkb).getVarIndex(var),select_type);//get all the a with specific type
						else
							temp = (*pkb).getUsedList((*pkb).getVarIndex(var),select_type);
						if(temp.size()>0){//if has result
							result = pattern_result;
						}
					}
				}//if b is constant
				else{//if b is variable
					if(isConstant(desAbstr.at(i).ref1)){ //select c modify(#TRUE_VALUE,b)
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						else
							temp = (*pkb).getUsedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						if(temp.size()>0){
							result = pattern_result;
						}
					}//if select c modify(#TRUE_VALUE,b) pattern c
					else{//if select c modify(a,b) pattern c
						temp = (*pkb).getStmtNo(evaluate_type);
						if(temp.size()>0){
							result = pattern_result;
						}
					}//if select c modify(a,b) pattern c
				}//else if b is variable

			}//else if select c modify(a,b) pattern(c)
			else if(pattern_position==0){//select c modify(a,b) 
				if(isConstant(desAbstr.at(i).ref2)){ //select c modify(a,"b") selectype = evaluate_type1 evaluate type = evaluate type2
					string var = desAbstr.at(i).ref2.substr(1,desAbstr.at(i).ref2.size()-2);
					if(isConstant(desAbstr.at(i).ref1)){//if select c modify(#TRUE_VALUE,"b")
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						else
							temp = (*pkb).getUsedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						for(k=0;k<temp.size();k++){
							if(temp.at(k)==(*pkb).getVarIndex(var)&&temp.at(k)!=-1){
								result.cell.insert(temp.at(k));
								break;
							}

						}
					}
					else{
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedList((*pkb).getVarIndex(var),select_type);//get all the a with specific type
						else
							temp = (*pkb).getUsedList((*pkb).getVarIndex(var),select_type);
						if(temp.size()>0){//if has result
							result.cell.insert(TRUE_VALUE);
						}
					}
				}//if b is constant
				else{//if b is variable
					if(isConstant(desAbstr.at(i).ref1)){ //select c modify(#TRUE_VALUE,b)
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						else
							temp = (*pkb).getUsedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						if(temp.size()>0){
							result.cell.insert(TRUE_VALUE);
						}
					}//if select c modify(#TRUE_VALUE,b)
					else{//if select c modify(a,b)
						temp = (*pkb).getStmtNo(select_type);
						if(temp.size()>0){
							result.cell.insert(TRUE_VALUE);
						}
					}//if select c modify(a,b)
				}//else if b is variable
			}//select c modify(a,b)
		}//else if select c modify(a,b)
		else if(evaluate_position==3){//select d modified(a,b) pattern c
			if(pattern_result.cell.size()>0){//if pattern has results
				if(isConstant(desAbstr.at(i).ref2)){ //select c modify(a,"b") selectype = evaluate_type1 evaluate type = evaluate type2
					string var = desAbstr.at(i).ref2.substr(2,desAbstr.at(i).ref2.size()-2);
					if(isConstant(desAbstr.at(i).ref1)){//if select c modify(#TRUE_VALUE,"b")
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						else
							temp = (*pkb).getUsedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){
								if(temp.at(k)==(*pkb).getVarIndex(var)){
									result.cell.insert(temp.at(k));
									break;
								}
							}
						}
					}
					else{
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedList((*pkb).getVarIndex(var),select_type);//get all the a with specific type
						else
							temp = (*pkb).getUsedList((*pkb).getVarIndex(var),select_type);
						if(temp.size()>0){//if has result
							result.cell.insert(temp.at(k));
						}
					}
				}//if b is constant
				else{//if b is variable
					if(isConstant(desAbstr.at(i).ref1)){ //select c modify(#TRUE_VALUE,b)
						if(relation.compare("modify")==0)
							temp = (*pkb).getModifiedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						else
							temp = (*pkb).getUsedStmt(atoi(desAbstr.at(i).ref1.c_str()));
						if(temp.size()>0){
							result.cell.insert(temp.at(k));
						}
					}//if select c modify(#TRUE_VALUE,b)
					else{//if select c modify(a,b)
						temp = (*pkb).getStmtNo(evaluate_type);
						if(temp.size()>0){
							result.cell.insert(temp.at(k));
						}
					}//if select c modify(a,b)
				}//else if b is variable
			}//if pattern has results
		}//select d modified(a,b) pattern c
		return result;
}
QueryEvaluator::Colunm<int> QueryEvaluator::evaluate_parents_follows(
	int evaluate_position,string select_type, string evaluate_type,
	QueryEvaluator::Colunm<int> pattern_result,int pattern_position, string relation){
		QueryEvaluator::Colunm<int> result;
		int i=0;
		bool flag=false;
		bool flag_go= true;
		bool exists = false;
		unsigned int k=0,n=0;
		
		vector<int> temp;
		if(select_type=="_"&&evaluate_type=="_"){
			result.cell.insert(TRUE_VALUE);
			return result;
		}
		if(select_type=="_"||evaluate_type=="_"){
			if((select_type.compare("_")==0)&&(evaluate_type.compare("_")==0))
				flag_go=false;
			
		}
		if(select_type=="constant"||select_type=="_")
			select_type="stmt";
		if(evaluate_type=="constant"||evaluate_type=="_")
			evaluate_type="stmt";
		if((select_type=="stmt"&&evaluate_type=="stmt"&&desAbstr.at(0).ref1!=desAbstr.at(0).ref2)&&flag){
			result.cell.insert(TRUE_VALUE);
			return result;
		}
		if(select_type==evaluate_type&&desAbstr.at(0).ref1==desAbstr.at(0).ref2)
			return result;

		if(evaluate_position==2){//select a modify(a,b)
			if(pattern_position==1){
				//select a relation(a,b) pattern(a)
				//for all the pattern result
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
					int element = *g;
					vector<int> temp;
					//get possoble b with pattern
					if(isConstant(desAbstr.at(i).ref2)){//if b is a constant
						if (relation.compare("parent")==0)// one to many
							temp = (*pkb).getChildren(element,evaluate_type);
						else if (relation.compare("parentT")==0)//one to many or many to one
							temp = (*pkb).getChildrenT(element,evaluate_type);
						else if (relation.compare("follow")==0)//one to one
							temp.push_back((*pkb).findFollowed(element));
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowedT(element,evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								if(temp.at(k)==atoi(desAbstr.at(i).ref2.c_str())) //compare it with ref1 result
									result.cell.insert(element);//insert a
							}
						}//for temp
					}//if ref2 is constant
					else{//if ref2 is variable  //select a relation(a,b) pattern(a)
						//for all the pattern result
						for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
							int element = *g;
							//get possoble b with pattern
							if (relation.compare("parent")==0)// one to many
								temp = (*pkb).getChildren(element,evaluate_type);
							else if (relation.compare("parentT")==0)//one to many or many to one
								temp = (*pkb).getChildrenT(element,evaluate_type);
							else if (relation.compare("follow")==0){//one to one
								int o;
								o = (*pkb).findFollowed(element);
								if (o!=-1){
									if(evaluate_type.compare((*pkb).getStmtType(o))==0||evaluate_type=="stmt")
										temp.push_back(o);
								}
							}
							else if (relation.compare("followT")==0)//one to many or many to one
								temp = (*pkb).findFollowedT(element,evaluate_type);
							if(temp.size()>0){
								for(k=0;k<temp.size();k++)
									result.cell.insert(temp.at(k));
							}
						}//for temp
					}//else
				}//for all the pattern result
			}//if select a relation(a,b) pattern(a)
			else if(pattern_position==2){//select a relation(a,b) pattern(b)
				//find possible value of b with pattern
				//get possoble a with pattern b
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
					int element = *g;
					vector<int> temp;
					if(isConstant(desAbstr.at(i).ref2)){//if b is a constant
						if(element==atoi(desAbstr.at(i).ref2.c_str())){
							if (relation.compare("parent")==0&&select_type.compare("while")==0){// one to many
								int o = (*pkb).getParent(element);
								if(o!=-1){
									temp.push_back(o);
								}
							}
							else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
								temp = (*pkb).getParentT(element);
							else if (relation.compare("follow")==0){//one to one
								int o = (*pkb).findFollows(element);
								if(o!=-1&&(*pkb).getStmtType(o).compare(select_type)==0)//check type
									temp.push_back(o);
							}
							else if (relation.compare("followT")==0)//one to many or many to one
								temp = (*pkb).findFollowsT(element,select_type);
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								if(temp.at(k)!=-1&&(*pkb).getStmtType(temp.at(k)).compare(select_type)==0) //
									result.cell.insert(temp.at(k));
							}//for temp
						}//if b can be found in pattern
					}//if ref2 is constant
					else{//if ref2 is variable  //select a relation(a,b) pattern(b)
						//for all the pattern result
						//get possoble a with pattern
						if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0))){// one to many
							int o = (*pkb).getParent(element);
							if(o!=-1){
								temp.push_back(o);
							}
						}
						else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowsT(element,select_type);
						for(k=0;k<temp.size();k++){//all the index of possible ref1
							if(temp.at(k)!=-1) //
								result.cell.insert(temp.at(k));
						}//for temp
					}//if b is variable
				}//for all the pattern result
			}//pattern_position == 2
			else if(pattern_position==3){//select a modify(a,b), patternC
				if(pattern_result.cell.size()>0){//if has result
					if(isConstant(desAbstr.at(i).ref2)){//if b is a constant
						int element=atoi(desAbstr.at(i).ref2.c_str());
						if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0))){// one to many
							int o = (*pkb).getParent(element);
							if(o!=-1){
								temp.push_back(o);
							}
						}
						else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))//check type
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowsT(element,select_type);
						for(k=0;k<temp.size();k++){//all the index of possible ref1
							string t = (*pkb).getStmtType(temp.at(k));
							//	if((temp.at(k)!=-1&&t.compare(select_type)==0)||(t=="while"&&select_type=="stmt") //
							if(temp.at(k)!=-1&&(t.compare(select_type)==0) || (select_type=="stmt"))
								result.cell.insert(temp.at(k));
						}//for temp
					}//if ref2 is constant
					else{//if ref2 is variable 
						//for all the pattern result
						//get possoble a with pattern
						vector<int> all = (*pkb).getStmtNo(evaluate_type);
						for(n=0;n<all.size();n++){
							int element = all.at(n);
							if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0))){// one to many
								int o = (*pkb).getParent(element);
								if(o!=-1){
									temp.push_back(o);
								}
							}
							else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
								temp = (*pkb).getParentT(element);
							else if (relation.compare("follow")==0){//one to one
								int o = (*pkb).findFollows(element);
								if(o!=-1&&(*pkb).getStmtType(o).compare(select_type)==0)
									temp.push_back(o);
							}
							else if (relation.compare("followT")==0)//one to many or many to one
								temp = (*pkb).findFollowsT(element,select_type);
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								if(temp.at(k)!=-1) //
									result.cell.insert(temp.at(k));
							}//for temp
						}
					}//if b is variable
				}//if pattern has result
			}//if pattern_postion = 3
			else if(pattern_position==0){//select a modify(a,b)
				if(isConstant(desAbstr.at(i).ref2)){//if b is a constant
					int element=atoi(desAbstr.at(i).ref2.c_str());
					if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0))){// one to many

						int o = (*pkb).getParent(element);

						if(o!=-1){
							temp.push_back(o);
						}
					}
					else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
						temp = (*pkb).getParentT(element);
					else if (relation.compare("follow")==0){//one to one
						int o = (*pkb).findFollows(element);
						if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))//check type
							temp.push_back(o);
					}
					else if (relation.compare("followT")==0)//one to many or many to one
						temp = (*pkb).findFollowsT(element,select_type);
					for(k=0;k<temp.size();k++){//all the index of possible ref1
						if(temp.at(k)!=-1&&((*pkb).getStmtType(temp.at(k)).compare(select_type)==0||select_type=="stmt")){ //
							//cout<<"jump here: "<<temp.at(k)<<"\n";
							result.cell.insert(temp.at(k));
						}
					}//for temp
				}//if ref2 is constant
				else{//if ref2 is variable  //select a relation(a,b)
					//for all the pattern result
					//get possoble a with pattern
					vector<int> all = (*pkb).getStmtNo(evaluate_type);
					for(n=0;n<all.size();n++){
						vector<int> temp;
						int element = all.at(n);
						if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0))){// one to many
							int o = (*pkb).getParent(element);
							if(o!=-1){
								temp.push_back(o);
							}
						}
						else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowsT(element,select_type);
						for(k=0;k<temp.size();k++){//all the index of possible ref1
							if(temp.at(k)!=-1) //
								result.cell.insert(temp.at(k));
						}//for temp
					}
				}//if b is variable
			}//if select a modify(a,b)
		}//if select a relation(a,b)
		else if(evaluate_position==1){//select b modify(a,b)
			
			if(pattern_position==1){//select b relation(a,b) pattern(a)
				//for all the pattern result
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
					int element = *g;
					vector<int> temp;
					//get possoble b with pattern
					if(isConstant(desAbstr.at(i).ref1)){//if a is a constant
						if(element==atoi(desAbstr.at(i).ref1.c_str())){
							if (relation.compare("parent")==0)// one to many
								temp = (*pkb).getChildren(element,select_type);
							else if (relation.compare("parentT")==0)//one to many or many to one
								temp = (*pkb).getChildrenT(element,select_type);
							else if (relation.compare("follow")==0){//one to one
								int o = (*pkb).findFollowed(element);
								if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
									temp.push_back(o);
							}
							else if (relation.compare("followT")==0)//one to many or many to one
								temp = (*pkb).findFollowedT(element,select_type);
							if(temp.size()>0){
								for(k=0;k<temp.size();k++){//all the index of possible ref1
									result.cell.insert(element);//insert a
								}
							}//for temp
						}//if a is in the pattern
					}//if ref1 is constant
					else{//if ref1 is variable  //select b relation(a,b) pattern(a)
						//for all the pattern result
						//get possoble b with pattern
						if (relation.compare("parent")==0)// one to many
							temp = (*pkb).getChildren(element,select_type);
						else if (relation.compare("parentT")==0)//one to many or many to one
							temp = (*pkb).getChildrenT(element,select_type);
						else if (relation.compare("follow")==0){//one to one
							int o = (*pkb).findFollowed(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowedT(element,select_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								result.cell.insert(element);//insert a
							}
						}//for temp
					}//for all the pattern result
				}//if a is variable 
			}//if select a relation(a,b) pattern(a)
			else if(pattern_position==2){//select b relation(a,b) pattern(b)
				//find possible value of b with pattern
				//get possoble a with pattern b
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
					int element = *g;
					vector<int> temp;
					//get possoble b with pattern
					if(isConstant(desAbstr.at(i).ref1)){//if a is a constant
						if (relation.compare("parent")==0)// one to many
							temp.push_back((*pkb).getParent(element));
						else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0))){//one to many or many to one
							temp = (*pkb).getParentT(element);
						}
						else if (relation.compare("follow")==0){//one to one
							temp.push_back((*pkb).findFollows(element));
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowsT(element,evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								if(temp.at(k)==atoi(desAbstr.at(i).ref1.c_str())) //compare it with ref2 result
									result.cell.insert(element);//insert a
							}
						}//for temp
					}//if ref1 is constant
					else{//if ref1 is variable  //select b relation(a,b) pattern(b)
						if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))// one to many
							temp.push_back((*pkb).getParent(element));
						else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							temp.push_back((*pkb).findFollows(element));
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowedT(element,evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								result.cell.insert(element);//insert a
							}
						}//for temp
					}//else
				}//for all the pattern result
			}//pattern_position == 2
			else if(pattern_position==3){//select b relation(a,b), patternC
				
				vector<int> all =(*pkb).getStmtNo(select_type);
				for(n=0;n<all.size();n++){
					vector<int> temp;
					int element = all.at(n);
					if(isConstant(desAbstr.at(i).ref1)){//if a is a constant
						if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
							temp.push_back((*pkb).getParent(element));
						else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowsT(element,evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								if(temp.at(k)==atoi(desAbstr.at(i).ref1.c_str())) //compare it with ref2 result
									result.cell.insert(element);//insert a
							}
						}//for temp
					}//if ref1 is constant
					else{//if ref1 is variable  //select b relation(a,b) pattern(b)
						if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
							temp.push_back((*pkb).getParent(element));
						else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowedT(element,evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								result.cell.insert(element);//insert a
							}
						}//for temp
					}//else
				}//for all the given type b
			}//if pattern_postion = 3
			else if(pattern_position==0){//select b modify(a,b)
				if(isConstant(desAbstr.at(i).ref2)){//if b is constant
					//do nothing
				}
				else{//b is variable

					vector<int> all =(*pkb).getStmtNo(select_type);//get all the b
					for(n=0;n<all.size();n++){
						int indexB = all.at(n);
						vector<int> temp;
						if(isConstant(desAbstr.at(i).ref1)){//if a is constant
							int indexA = atoi(desAbstr.at(i).ref1.c_str());
							if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
								temp.push_back((*pkb).getParent(indexB));
							else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
								temp = (*pkb).getParentT(indexB);
							else if (relation.compare("follow")==0){//one to one
								int index_a = (*pkb).findFollows(indexB);
								if(index_a==indexA)
									temp.push_back(indexA);
							}
							else if (relation.compare("followT")==0)//one to many or many to one
								temp = (*pkb).findFollowsT(indexB,evaluate_type);
							if(temp.size()>0){
								for(k=0;k<temp.size();k++){//all the index of possible ref1
									if(temp.at(k)==atoi(desAbstr.at(i).ref1.c_str())) //compare it with ref2 result
										result.cell.insert(indexB);//insert b
								}
							}//for temp
						}//a is constant
						else{//a is variable
							if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
								temp.push_back((*pkb).getParent(indexB));
							else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
								temp = (*pkb).getParentT(indexB);
							else if (relation.compare("follow")==0){//one to one
								int index_a_follow_b=(*pkb).findFollows(indexB);
								if(index_a_follow_b!=-1)
									temp.push_back(index_a_follow_b);
							}
							else if (relation.compare("followT")==0){//one to many or many to one
								temp = (*pkb).findFollowsT(indexB,select_type);
							}
							for(k=0;k<temp.size();k++){
								if(temp.at(k)!=-1){
									result.cell.insert(indexB);
									break;
								}
							}			
						}//a is variable
					}//b is variable
				}
			}
		}
		else if(evaluate_position==0){//select c relation(a,b)
			if(pattern_position==1){//select c relation(a,b) pattern(a)
				//for all the pattern result
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
					int element = *g;
					vector<int> temp;
					bool flag1 = false;
					//get possoble b with pattern
					if(isConstant(desAbstr.at(i).ref2)){//if b is constant
						int indexB = atoi(desAbstr.at(i).ref2.c_str());
						if(isConstant(desAbstr.at(i).ref1)){//if a is constant
							int indexA = atoi(desAbstr.at(i).ref1.c_str());
							if(indexA==element){
								if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
									temp.push_back((*pkb).getParent(indexB));
								else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
									temp = (*pkb).getParentT(indexB);
								else if (relation.compare("follow")==0){//one to one
									int index_a_follow_b=(*pkb).findFollows(indexB);
									if(index_a_follow_b!=-1)
										temp.push_back(index_a_follow_b);
								}
								else if (relation.compare("followT")==0){//one to many or many to one
									temp = (*pkb).findFollowsT(indexB,select_type);
								}
								for(k=0;k<temp.size();k++){
									if(temp.at(k)==indexA){
										flag1=true;
										result.cell.insert(TRUE_VALUE);
										break;
									}
								}
							}
						}//if a is a constant
						else{//if a is a variable
							int indexA = element;
							if (relation.compare("follow")==0){//one to one
								int index_a_follow_b=(*pkb).findFollows(indexB);
								if(index_a_follow_b!=-1)
									temp.push_back(index_a_follow_b);
							}
							else if (relation.compare("followT")==0){//one to many or many to one
								temp = (*pkb).findFollowsT(indexB,select_type);
							}
							for(k=0;k<temp.size();k++){
								if(temp.at(k)==indexA){
									flag1=true;
									result.cell.insert(TRUE_VALUE);
									break;
								}
							}
						}//a is a variable
					}//if b is constant
					else{//if b is variable
						vector<int> all = (*pkb).getStmtNo(evaluate_type);
						for(n=0;n<all.size();n++){
							int indexB = all.at(n);
							if(isConstant(desAbstr.at(i).ref1)){//if a is constant
								int indexA = atoi(desAbstr.at(i).ref1.c_str());
								if(indexA==element){
									if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
										temp.push_back((*pkb).getParent(indexB));
									else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
										temp = (*pkb).getParentT(indexB);
									else	if (relation.compare("follow")==0){//one to one
										int index_a_follow_b=(*pkb).findFollows(indexB);
										if(index_a_follow_b!=-1)
											temp.push_back(index_a_follow_b);
									}
									else if (relation.compare("followT")==0){//one to many or many to one
										temp = (*pkb).findFollowsT(indexB,select_type);
									}
									for(k=0;k<temp.size();k++){
										if(temp.at(k)==indexA){
											flag1=true;
											result.cell.insert(TRUE_VALUE);
											break;
										}
									}
								}
							}//if a is a constant
							else{//if a is a variable
								int indexA = element;
								if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
									temp.push_back((*pkb).getParent(indexB));
								else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
									temp = (*pkb).getParentT(indexB);
								else	if (relation.compare("follow")==0){//one to one
									int index_a_follow_b=(*pkb).findFollows(indexB);
									if(index_a_follow_b!=-1)
										temp.push_back(index_a_follow_b);
								}
								else if (relation.compare("followT")==0){//one to many or many to one
									temp = (*pkb).findFollowsT(indexB,select_type);
								}
								for(k=0;k<temp.size();k++){
									if(temp.at(k)==indexA){
										flag1=true;
										result.cell.insert(TRUE_VALUE);
										break;
									}
								}
							}//a is a variable
						}//for all the b
					}//if b is variable 
				}//if select a relation(a,b) pattern(a)
			}
			else if(pattern_position==2){//select c relation(a,b) pattern(b)
				//find possible value of b with pattern
				//get possoble a with pattern b
				for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
					int element = *g;
					vector<int> temp;
					//get possoble b with pattern
					if(isConstant(desAbstr.at(i).ref1)){//if a is a constant
						if (relation.compare("parent")==0)// one to many
							temp.push_back((*pkb).getParent(element));
						else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0))){//one to many or many to one
							temp = (*pkb).getParentT(element);
						}
						else if (relation.compare("follow")==0){//one to one
							temp.push_back((*pkb).findFollows(element));
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowedT(element,evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								if(temp.at(k)==atoi(desAbstr.at(i).ref1.c_str())) //compare it with ref2 result
									result.cell.insert(element);//insert a
							}
						}//for temp
					}//if ref1 is constant
					else{//if ref1 is variable  //select b relation(a,b) pattern(b)
						if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
							temp.push_back((*pkb).getParent(element));
						else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							temp.push_back((*pkb).findFollows(element));
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowedT(element,evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								result.cell.insert(element);//insert a
							}
						}//for temp
					}//else
				}//for all the pattern result
			}//pattern_position == 2
			else if(pattern_position==3){//select c relation(a,b), patternC
				if(pattern_result.cell.size()>0){
					if(isConstant(desAbstr.at(i).ref2)){//if b is constant
						int indexB = atoi(desAbstr.at(i).ref2.c_str());
						if(isConstant(desAbstr.at(i).ref1)){//if a is constant
							int indexA = atoi(desAbstr.at(i).ref1.c_str());
							if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
								temp.push_back((*pkb).getParent(indexB));
							else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
								temp = (*pkb).getParentT(indexB);
							else if (relation.compare("follow")==0){//one to one
								int index_a_follow_b=(*pkb).findFollows(indexB);
								if(index_a_follow_b!=-1)
									temp.push_back(index_a_follow_b);
							}
							else if (relation.compare("followT")==0){//one to many or many to one
								temp = (*pkb).findFollowsT(indexB,select_type);
							}
							for(k=0;k<temp.size();k++){
								if(temp.at(k)==indexA){
									result.cell.insert(TRUE_VALUE);
									break;
								}
							}
						}//if a is a constant
						else{//if a is a variable
							if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
								temp.push_back((*pkb).getParent(indexB));
							else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
								temp = (*pkb).getParentT(indexB);
							if (relation.compare("follow")==0){//one to one
								int index_a_follow_b=(*pkb).findFollows(indexB);
								if(index_a_follow_b!=-1)
									temp.push_back(index_a_follow_b);
							}
							else if (relation.compare("followT")==0){//one to many or many to one
								temp = (*pkb).findFollowsT(indexB,select_type);
							}
							for(k=0;k<temp.size();k++){
								if(temp.at(k)!=-1 && ((*pkb).getStmtType(temp.at(k)).compare(select_type)==0||select_type=="stmt")){
									result.cell.insert(TRUE_VALUE);
									break;
								}
							}
						}//a is a variable
					}//if b is constant
					else{//if b is variable
						vector<int> all = (*pkb).getStmtNo(evaluate_type);//all b
						for(n=0;n<all.size();n++){
							int indexB = all.at(n);
							vector<int> temp;
							if(isConstant(desAbstr.at(i).ref1)){//if a is constant
								int indexA = atoi(desAbstr.at(i).ref1.c_str());
								if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
									temp.push_back((*pkb).getParent(indexB));
								else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
									temp = (*pkb).getParentT(indexB);
								if (relation.compare("follow")==0){//one to one
									int index_a_follow_b=(*pkb).findFollows(indexB);
									if(index_a_follow_b!=-1)
										temp.push_back(index_a_follow_b);
								}
								else if (relation.compare("followT")==0){//one to many or many to one
									temp = (*pkb).findFollowsT(indexB,select_type);
								}
								for(k=0;k<temp.size();k++){
									if(temp.at(k)==indexA){
										result.cell.insert(TRUE_VALUE);
										break;
									}
								}

							}//if a is a constant
							else{//if a is a variable
								if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
									temp.push_back((*pkb).getParent(indexB));
								else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
									temp = (*pkb).getParentT(indexB);
								else if (relation.compare("follow")==0){//one to one
									int index_a_follow_b=(*pkb).findFollows(indexB);
									if(index_a_follow_b!=-1)
										temp.push_back(index_a_follow_b);
								}
								else if (relation.compare("followT")==0){//one to many or many to one
									temp = (*pkb).findFollowsT(indexB,select_type);
								}
								for(k=0;k<temp.size();k++){
									if(temp.at(k)!=-1 && ((*pkb).getStmtType(temp.at(k)).compare(select_type)==0||select_type=="stmt")){
										result.cell.insert(TRUE_VALUE);
										break;
									}
								}
							}//a is a variable
						}//for all the b
					}

				}//if pattern has result
			}//if pattern_postion = 3
			else if(pattern_position==4){//select c relation(a,b), pattern D
				vector<int> all =(*pkb).getStmtNo(select_type);
				for(n=0;n<all.size();n++){
					vector<int> temp;
					int element = all.at(n);
					if(isConstant(desAbstr.at(i).ref1)){//if a is a constant
						if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
							temp.push_back((*pkb).getParent(element));
						else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowsT(element,evaluate_type);//xiaowei
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								if(temp.at(k)==atoi(desAbstr.at(i).ref1.c_str())) //compare it with ref2 result
									result.cell.insert(element);//insert a
							}
						}//for temp
					}//if ref1 is constant
					else{//if ref1 is variable  //select c relation(a,b) pattern(b)
						if (relation.compare("parent")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))// one to many
							temp.push_back((*pkb).getParent(element));
						else if (relation.compare("parentT")==0&&(evaluate_type.compare("while")==0||(evaluate_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(element);
						else if (relation.compare("follow")==0){//one to one
							temp.push_back((*pkb).findFollows(element));
							int o = (*pkb).findFollows(element);
							if(o!=-1&&((*pkb).getStmtType(o).compare(select_type)==0||select_type=="stmt"))
								temp.push_back(o);
						}
						else if (relation.compare("followT")==0)//one to many or many to one
							temp = (*pkb).findFollowedT(element,evaluate_type);
						if(temp.size()>0){
							for(k=0;k<temp.size();k++){//all the index of possible ref1
								result.cell.insert(element);//insert a
							}
						}//for temp
					}//else
				}//for all the given type b
			}//if pattern has result
			else if(pattern_position==0){//select c modify(a,b)
				bool flag1 = false;
				if(isConstant(desAbstr.at(i).ref2)){
					int indexB = atoi(desAbstr.at(i).ref2.c_str());
					if(isConstant(desAbstr.at(i).ref1)){//if a is a constant
						int indexA = atoi(desAbstr.at(i).ref1.c_str());
						if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))// one to many
							temp.push_back((*pkb).getParent(indexB));
						else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
							temp = (*pkb).getParentT(indexB);
						else if (relation.compare("follow")==0){//one to one
							int index_a_follow_b=(*pkb).findFollows(indexB);
							if(index_a_follow_b!=-1)
								temp.push_back(index_a_follow_b);
						}
						else if (relation.compare("followT")==0){//one to many or many to one
							temp = (*pkb).findFollowsT(indexB,select_type);
						}
						for(k=0;k<temp.size();k++){
							if(temp.at(k)==indexA){
								flag1=true;
								result.cell.insert(TRUE_VALUE);
								break;
							}
						}
					}//if a is a constant
					//followed() find b
					//follows() find a
					else if(!isConstant(desAbstr.at(i).ref1)){//select c modify(a,"b")
						if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))// one to many //xiaowei
							temp.push_back((*pkb).getParent(indexB));
						else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one //xiaowei
							temp = (*pkb).getParentT(indexB);
						else if (relation.compare("follow")==0){//one to one
							int index_a_follow_b=(*pkb).findFollows(indexB);
							if(index_a_follow_b!=-1)
								temp.push_back(index_a_follow_b);
						}
						else if (relation.compare("followT")==0){//one to many or many to one
							temp = (*pkb).findFollowsT(indexB,select_type);
						}
						for(k=0;k<temp.size();k++){
							if(temp.at(k)!=-1){
								flag1=true;
								result.cell.insert(TRUE_VALUE);
								break;
							}
						}
					}
				}//if b is a constant
				else{//b is variable
					vector<int> all_b = (*pkb).getStmtNo(evaluate_type);
					for(n=0;n<all_b.size();n++){//for all the b
						vector<int> temp;
						int indexB = all_b.at(n);
						if(isConstant(desAbstr.at(i).ref1)){//if a is a constant
							int indexA = atoi(desAbstr.at(i).ref1.c_str());
							if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))// one to many
								temp.push_back((*pkb).getParent(indexB));
							else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
								temp = (*pkb).getParentT(indexB);
							else if (relation.compare("follow")==0){//one to one
								int index_a_follow_b=(*pkb).findFollows(indexB);
								if(index_a_follow_b!=-1)
									temp.push_back(index_a_follow_b);
							}
							else if (relation.compare("followT")==0){//one to many or many to one
								temp = (*pkb).findFollowsT(indexB,select_type);
							}
							for(k=0;k<temp.size();k++){
								if(temp.at(k)==indexA){
									flag1=true;
									result.cell.insert(TRUE_VALUE);
									break;
								}
							}
							if(flag1)
								break;
						}//if a is a constant
						//followed() find b
						//follows() find a
						else if(!isConstant(desAbstr.at(i).ref1)){//select c modify(a,b)
							if (relation.compare("parent")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))// one to many
								temp.push_back((*pkb).getParent(indexB));
							else if (relation.compare("parentT")==0&&(select_type.compare("while")==0||(select_type.compare("stmt")==0)))//one to many or many to one
								temp = (*pkb).getParentT(indexB);
							else if (relation.compare("follow")==0){//one to one
								int index_a_follow_b=(*pkb).findFollows(indexB);
								if(index_a_follow_b!=-1)
									temp.push_back(index_a_follow_b);
							}
							else if (relation.compare("followT")==0){//one to many or many to one
								temp = (*pkb).findFollowsT(indexB,select_type);
							}
							for(k=0;k<temp.size();k++){
								if(temp.at(k)!=-1){
									flag1=true;
									result.cell.insert(TRUE_VALUE);
									break;
								}
							}
							if(flag1)
								break;
						}
					}//for all the b
				}//b is variable
			}//else if select c modify(a,b)

		}
		return result;
}
