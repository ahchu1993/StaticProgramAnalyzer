#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include "QueryPreprocessor.h"
using namespace std;


static string designEntity[] = {"procedure","stmtList", "stmt", "assign", "call", "while", "if", "variable", "constant", "prog_line"};
static const int NumDE = 10;

void fillArg(QueryPreprocessor::arg_type_list* arg_list,bool undersc, bool int_t, bool string_t){
	arg_list->underscore = undersc;
	arg_list->int_type = int_t;
	arg_list->string_type = string_t;

}

void QueryPreprocessor::addModifiesEntry(){
	arg_type_list arg1_list,arg2_list;
	arg1_list.synonym_type.push_back("procedure");
	arg1_list.synonym_type.push_back("prog_line");
	arg1_list.synonym_type.push_back("stmt");
	arg1_list.synonym_type.push_back("assign");
	arg1_list.synonym_type.push_back("if");
	arg1_list.synonym_type.push_back("while");
	fillArg(&arg1_list,false,true,true);

	arg2_list.synonym_type.push_back("variable");
	fillArg(&arg2_list,true,false,true);
	entry e = {arg1_list,arg2_list};
	table["Modifies"] = e;
}

void QueryPreprocessor::addUsesEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("procedure");
	arg1.synonym_type.push_back("prog_line");
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("assign");
	arg1.synonym_type.push_back("if");
	arg1.synonym_type.push_back("while");
	fillArg(&arg1,false,true,true);

	arg2.synonym_type.push_back("variable");
	fillArg(&arg2,true,false,true);
	entry e = {arg1,arg2};
	table["Uses"] = e;
}

void QueryPreprocessor::addCallsEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("procedure");
	fillArg(&arg1,true,true,true);
	arg2.synonym_type.push_back("procedure");
	fillArg(&arg2,true,true,true);
	entry e = {arg1,arg2};
	table["Calls"] = e;
}

void QueryPreprocessor::addCallsTEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("procedure");
	fillArg(&arg1,true,true,true);
	arg2.synonym_type.push_back("procedure");
	fillArg(&arg2,true,true,true);
	entry e = {arg1,arg2};
	table["Calls*"] = e;
}

void QueryPreprocessor::addParentEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("prog_line");
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("if");
	arg1.synonym_type.push_back("while");
	fillArg(&arg1,true,true,false);

	arg2.synonym_type.push_back("prog_line");
	arg2.synonym_type.push_back("stmt");
	arg2.synonym_type.push_back("assign");
	arg2.synonym_type.push_back("if");
	arg2.synonym_type.push_back("while");
	fillArg(&arg2,true,true,false);
	entry e = {arg1,arg2};
	table["Parent"] = e;
}

void QueryPreprocessor::addParentTEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("prog_line");
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("if");
	arg1.synonym_type.push_back("while");
	fillArg(&arg1,true,true,false);

	arg2.synonym_type.push_back("prog_line");
	arg2.synonym_type.push_back("stmt");
	arg2.synonym_type.push_back("assign");
	arg2.synonym_type.push_back("if");
	arg2.synonym_type.push_back("while");
	fillArg(&arg2,true,true,false);
	entry e = {arg1,arg2};
	table["Parent*"] = e;
}

void QueryPreprocessor::addFollowsEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("prog_line");
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("assign");
	arg1.synonym_type.push_back("if");
	arg1.synonym_type.push_back("while");
	fillArg(&arg1,true,true,false);

	arg2.synonym_type.push_back("prog_line");
	arg2.synonym_type.push_back("stmt");
	arg2.synonym_type.push_back("assign");
	arg2.synonym_type.push_back("if");
	arg2.synonym_type.push_back("while");
	fillArg(&arg2,true,true,false);
	entry e = {arg1,arg2};
	table["Follows"] = e;
}

void QueryPreprocessor::addFollowsTEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("prog_line");
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("assign");
	arg1.synonym_type.push_back("if");
	arg1.synonym_type.push_back("while");
	fillArg(&arg1,true,true,false);

	arg2.synonym_type.push_back("prog_line");
	arg2.synonym_type.push_back("stmt");
	arg2.synonym_type.push_back("assign");
	arg2.synonym_type.push_back("if");
	arg2.synonym_type.push_back("while");
	fillArg(&arg2,true,true,false);
	entry e = {arg1,arg2};
	table["Follows*"] = e;
}

void QueryPreprocessor::addNextEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("prog_line");
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("assign");
	arg1.synonym_type.push_back("if");
	arg1.synonym_type.push_back("while");
	fillArg(&arg1,true,true,false);

	arg2.synonym_type.push_back("prog_line");
	arg2.synonym_type.push_back("stmt");
	arg2.synonym_type.push_back("assign");
	arg2.synonym_type.push_back("if");
	arg2.synonym_type.push_back("while");
	fillArg(&arg2,true,true,false);
	entry e = {arg1,arg2};
	table["Next"] = e;
}

void QueryPreprocessor::addNextTEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("prog_line");
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("assign");
	arg1.synonym_type.push_back("if");
	arg1.synonym_type.push_back("while");
	fillArg(&arg1,true,true,false);

	arg2.synonym_type.push_back("prog_line");
	arg2.synonym_type.push_back("stmt");
	arg2.synonym_type.push_back("assign");
	arg2.synonym_type.push_back("if");
	arg2.synonym_type.push_back("while");
	fillArg(&arg2,true,true,false);
	entry e = {arg1,arg2};
	table["Next*"] = e;
}

void QueryPreprocessor::addAffectsEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("assign");
	arg1.synonym_type.push_back("prog_line");
	fillArg(&arg1,true,true,false);

	arg2.synonym_type.push_back("stmt");
	arg2.synonym_type.push_back("assign");
	arg2.synonym_type.push_back("prog_line");
	fillArg(&arg2,true,true,false);
	entry e = {arg1,arg2};
	table["Affects"] = e;
}

void QueryPreprocessor::addAffectsTEntry(){
	arg_type_list arg1,arg2;
	arg1.synonym_type.push_back("stmt");
	arg1.synonym_type.push_back("assign");
	arg1.synonym_type.push_back("prog_line");
	fillArg(&arg1,true,true,false);

	arg2.synonym_type.push_back("stmt");
	arg2.synonym_type.push_back("assign");
	arg2.synonym_type.push_back("prog_line");
	fillArg(&arg2,true,true,false);
	entry e = {arg1,arg2};
	table["Affects*"] = e;
}

void QueryPreprocessor::buildTable(){
	addModifiesEntry();
	addUsesEntry();
	addCallsEntry();
	addCallsTEntry();
	addParentEntry();
	addParentTEntry();
	addFollowsEntry();
	addFollowsTEntry();
	addNextEntry();
	addNextTEntry();
	addAffectsEntry();
	addAffectsTEntry();

}

// constructor
QueryPreprocessor::QueryPreprocessor(){
	buildTable();
	build_attr_table();
}

/// --------------debugging use-------------------
void QueryPreprocessor::print_relation(string r){   
	QueryPreprocessor::entry e;
	QueryPreprocessor::arg_type_list arg1_list, arg2_list;
	cout<< r<<":\n";
	e = table[r];
	arg1_list = e.arg1_list;
	cout<< "arg1: ";
	for(unsigned int i =0;i<arg1_list.synonym_type.size();i++){
		cout<<arg1_list.synonym_type[i]<<" ";
	}
	cout<< "underscore "<<arg1_list.underscore<<
		" integer "<<arg1_list.int_type<<" string "<<arg1_list.string_type;

	arg2_list = e.arg2_list;
	cout<< "\narg2: ";
	for(unsigned int i =0;i<arg2_list.synonym_type.size();i++){
		cout<<arg2_list.synonym_type[i]<<" ";
	}
	cout<< "underscore "<<arg2_list.underscore<<
		" integer "<<arg2_list.int_type<<" string "<<arg2_list.string_type<<"\n";

}

void QueryPreprocessor::print_table(){
	print_relation("Calls");
	print_relation("Calls*");
	print_relation("Modifies");
	print_relation("Uses");
	print_relation("Parent");
	print_relation("Parent*");
	print_relation("Follows");
	print_relation("Follows*");
	print_relation("Next");
	print_relation("Next*");
	print_relation("Affects");
	print_relation("Affects*");
}



/// ------------------- auxiliary functions----------------------

string QueryPreprocessor::trim(string str){
	if(str.find_first_not_of(" ")>str.size()) {
		str = "";
		return str;
	}
	string::iterator It = str.begin();
	while(It!=str.end()){
		if(*It ==' '||*It =='\t')
			str.erase(It);
		else break;
	}

	while(str.find_last_of(' ')==str.size()-1||str.find_last_of('\t')==str.size()-1){
		str.erase(str.size()-1);
		str.shrink_to_fit();
	}

	return str;
}

bool QueryPreprocessor::exists(string reff){
	for(unsigned int i=0;i<declaration_reffs.size();i++){
		if(reff==declaration_reffs.at(i).synonym)
			return true;
	}
	return false;
}

void QueryPreprocessor::find_positions(string query){


	unsigned int p1 = query.find("such that");
	while(p1<query.size()){
		positions.insert(std::pair<int,string>(p1,"such that"));
		p1 = query.find("such that",p1+1);
	}

	unsigned int p2 = query.find("pattern");
	while(p2<query.size()){
		positions.insert(std::pair<int,string>(p2,"pattern"));
		p2 = query.find("pattern",p2+1);
	}

	unsigned int p3 = query.find("with");
	while(p3<query.size()){
		positions.insert(std::pair<int,string>(p3,"with"));
		p3 = query.find("with",p3+1);
	}

}

///--------------------Lexical rules-----------------

bool QueryPreprocessor::check_Letter(char c){
    if(isalpha(c)) return true;
    else return false;

}

bool QueryPreprocessor::check_Digit(char c){
    if(isdigit(c)) return true;
    else return false;

}

bool QueryPreprocessor::check_IDENT(string ident){
	if(ident=="") return false;
	if(check_Letter(ident.at(0))){

		string::iterator it = ident.begin()+1;
		while(it!=ident.end()){
			if((!check_Letter(*it))&&(!check_Digit(*it))&&(*it!='#'))
				return false;
			it++;
		}
		return true;
	}
	else return false;
}

bool QueryPreprocessor::check_Integer(string i){
	if(i=="") return false;
	if(check_Digit(i.at(0))){
		string::iterator it = i.begin()+1;
		while(it!=i.end()){
			if((!check_Digit(*it)))
				return false;
			it++;
		}
		return true;
	}
	else return false;
}

bool QueryPreprocessor::check_attrName(string name){
	return name=="procName"||name=="varName"||name=="value"||name=="stmt#";
}

bool QueryPreprocessor::check_synonym(string synonym){
	return check_IDENT(synonym);
}

bool QueryPreprocessor::check_attrRef(string attrRef){
	int p = attrRef.find(".");
	string s = attrRef.substr(0,p);
	string attr  = attrRef.substr(p+1,attrRef.size()-p-1);
	return check_synonym(s)&&check_attrName(attr);
}

bool QueryPreprocessor::check_elem(string elem){
	return check_synonym(elem)||check_attrRef(elem);
}

bool QueryPreprocessor::check_varRef(string s){
	if(s.at(0)=='\"'&&s.at(s.size()-1)=='\"')
		s = s.substr(1,s.size()-2);
	return check_synonym(s)||s=="_"||check_IDENT(s);
}

bool QueryPreprocessor::match_entity(string token){
	for(int i=0;i<NumDE;i++){
		if(token ==designEntity[i])
			return true;
	}
	return false;
}

bool QueryPreprocessor::check_process_tuple(string t){
	t = trim(t);

	if(check_elem(t)) {      ///single elem, no <>
		if(check_attrRef(t)){
			int p = t.find(".");
			string s = t.substr(0,p);
			if(exists(s)){
				result_reffs.push_back(t);
				return true;
			}
			else return false;
		}
		if(exists(t)){
			result_reffs.push_back(t);
			return true;
		}
		else return false;
	}
	else {


		if((t.at(0)=='<')&&(t.at(t.size()-1)=='>')){

			t = t.substr(1,t.size()-2);     ///take out "<" ">"
			unsigned int p = t.find(",");

			if(p>t.size()) {  ///no coma,single elem
				if(check_elem(t)){
					if(exists(t)){
						result_reffs.push_back(t);
						return true;
					}
					else return false;
				}
				else return false;
			}

			else {
				string t1 = trim(t.substr(0,p));   ///check first elem
				if(!check_elem(t1)) return false;
				else {
					if(check_attrRef(t1)){
						int p = t1.find(".");
						string s = t1.substr(0,p);
						if(exists(s)){
							result_reffs.push_back(t1);
							return true;
						}
						else return false;
					}
					if(exists(t1))
						result_reffs.push_back(t1);
					else return false;
				}

				while(p<t.size()){
					int p_next = t.find(",",p+1);
					if(p_next-p<=1) return false;

					t1 = trim(t.substr(p+1,p_next-p-1));
					if(!check_elem(t1)) return false;
					else {
						if(check_attrRef(t1)){
							int p = t1.find(".");
							string s = t1.substr(0,p);
							if(exists(s)){
								result_reffs.push_back(t1);
								return true;
							}
							else return false;
						}
						if(exists(t1))
							result_reffs.push_back(t1);
						else return false;
					}

					p = p_next;
				}
				return true;
			}

		}
		else return false;
	}
}

/// ---------------------   design abstractions-------------

string QueryPreprocessor::get_type(string synonym){
	for(unsigned int i=0;i<declaration_reffs.size();i++){
		if(synonym == declaration_reffs.at(i).synonym)
			return declaration_reffs.at(i).type;
	}
	return "";
}

bool QueryPreprocessor::relRef(string relation){
    int p1 = relation.find("(");
    int p2 = relation.find(",");
    int p3 = relation.find(")");
    string rel_type = trim(relation.substr(0,p1));
    string arg1 = trim(relation.substr(p1+1,p2-p1-1));
    string arg2 = trim(relation.substr(p2+1,p3-p2-1));
    entry e = table[rel_type];

    bool arg1_flag = false;
    bool arg2_flag = false;

    string arg1_type, arg2_type;
    arg_type_list arg1_list, arg2_list;

    if(check_synonym(arg1)){
        arg1_type = get_type(arg1);
        arg1_list= e.arg1_list;
        for(unsigned int i=0;i<arg1_list.synonym_type.size();i++){
            if (arg1_type == arg1_list.synonym_type[i])
                arg1_flag = true;
                break;
        }
    }else if(arg1=="_"&& arg1_list.underscore){
        arg1_type = "";
        arg1_flag = true;
    }else if(check_Integer(arg1)&&arg1_list.int_type){
        arg1_type = "integer";
        arg1_flag = true;
    }else{
        if (arg1[0]=='"' && arg1[arg1.size()-1]=='"' && arg1_list.string_type)
            arg1_type = "string";
            arg1_flag = true;
    }

    if(check_synonym(arg2)){
        arg2_type = get_type(arg2);
        arg2_list= e.arg2_list;
        for(unsigned int i=0;i<arg2_list.synonym_type.size();i++){
            if (arg2_type == arg2_list.synonym_type[i])
                arg2_flag = true;
                break;
        }
    }else if(arg2=="_"&& arg2_list.underscore){
        arg2_type = "";
        arg2_flag = true;
    }else if(check_Integer(arg2)&&arg2_list.int_type){
        arg2_type = "integer";
        arg2_flag = true;
    }else{
        if (arg2[0]=='"' && arg2[arg2.size()-1]=='"' && arg2_list.string_type)
            arg2_type = "string";
            arg2_flag = true;
    }

    if(arg1_flag&&arg2_flag){

        designAbstraction d(rel_type,arg1,arg1_type,arg2,arg2_type);
        designAbstraction* da = &d;

        bool t1 = arg1_type==""||arg1_type=="string"||arg1_type=="integer";
        bool t2 = arg2_type==""||arg2_type=="string"||arg2_type=="integer";
        if(t1||t2){
			if(rel_type=="Affects"||rel_type=="Affects*")
				constant_relations.push_back(da);
			else 
				constant_relations.push_front(da);
        }
		else {
			if(rel_type=="Affects"||rel_type=="Affects*")
				relations.push_back(da);
			else
				relations.push_front(da);
		}

        return true;
    }else return false;

}

bool QueryPreprocessor::relCond(string s){
	unsigned int p = s.find("and");
	if(p<s.size()){
		string s1 = trim(s.substr(0,p));
		string s2 = trim(s.substr(p+3,s.size()-p-3));
		return relRef(s1)&&relCond(s2);
	}
	else return relRef(s);
}


/// -----------SIMPLE expression validation-----------------------------------------
//static bool check_expr(string s);

bool QueryPreprocessor::check_factor(string s){
	s = trim(s);
	if(s=="") return false;

	if(s.at(0)=='('&&(s.at(s.size()-1)==')'))
		s = trim(s.substr(1,s.size()-2));
	return check_IDENT(s)||check_Integer(s)||check_expr(s);
}

bool QueryPreprocessor::check_term(string s){
	s = trim(s);
	if(s=="") return false;
	unsigned int p = s.find_last_of("*");
	if(p<s.size()){
		string s1 = trim(s.substr(0,p));
		string s2 = trim(s.substr(p+1,s.size()-p-1));
		return check_term(s1)&&check_factor(s2);
	}
	else return check_factor(s);
}

bool QueryPreprocessor::check_expr(string s){    ///handle simple expr without bracket

	s = trim(s);
	if(s.at(0)=='('&&s.at(s.size()-1)==')')
		s = s.substr(1,s.size()-2);

	unsigned int p1 = s.find_last_of("+");
	unsigned int p2 = s.find_last_of("-");
	if(p1<s.size()&&p2<s.size()){
		if(p1>p2){
			string s1 = trim(s.substr(0,p1));
			string s2 = trim(s.substr(p1+1,s.size()-p1-1));
			return check_term(s2)&&check_expr(s1);
		}
		else {
			string s1 = trim(s.substr(0,p2));
			string s2 = trim(s.substr(p2+1,s.size()-p2-1));
			return check_expr(s1)&&check_term(s2);
		}
	}
	else if(p1<s.size()){

		string s1 = trim(s.substr(0,p1));
		string s2 = trim(s.substr(p1+1,s.size()-p1-1));
		return check_term(s2)&&check_expr(s1);
	}
	else if(p2<s.size()){
		string s1 = trim(s.substr(0,p2));
		string s2 = trim(s.substr(p2+1,s.size()-p2-1));
		return check_term(s2)&&check_expr(s1);
	}
	else return check_term(s);


}

bool QueryPreprocessor::check_expr_f(string s){
	std::map<int,string> op_positions;
	std::map<int,string>::reverse_iterator rit;
	unsigned int p;

	p= s.find("+");
	while(p<s.size()){
		op_positions.insert(std::pair<int,string>(p,"+"));
		p = s.find("+",p+1);
	}

	p = s.find("-");
	while(p<s.size()){
		op_positions.insert(std::pair<int,string>(p,"-"));
		p = s.find("-",p+1);
	}

	p = s.find("*");
	while(p<s.size()){
		op_positions.insert(std::pair<int,string>(p,"*"));
		p = s.find("*",p+1);
	}

	p = s.find("(");
	while(p<s.size()){
		op_positions.insert(std::pair<int,string>(p,"("));
		p = s.find("(",p+1);
	}

	p = s.find(")");
	while(p<s.size()){
		op_positions.insert(std::pair<int,string>(p,")"));
		p = s.find(")",p+1);
	}

	rit = op_positions.rbegin();
	if(op_positions.empty()) return check_factor(s);
	int balance=0;

	while(rit!=op_positions.rend()){

		if(rit->second==")"){
			int p1 = rit->first;
			rit++;

			while(rit!=op_positions.rend()){
				//cout<<rit->first<<" "<<rit->second<<"\n";
				if(rit->second==")"){
					balance++;
					rit++;
				}

				else if(rit->second=="("&&balance!=0){
					balance--;
					rit++;
				}

				else if(rit->second=="("&&balance==0)
					break;
				else rit++;

			}

			if(rit==op_positions.rend()) return false;

			int p2 = rit->first;
			string s1 = s.substr(p2+1,p1-p2-1);
			unsigned int p_bracket = s1.find("(");
			if(p_bracket>s1.size()) {

				if(!check_expr(s1)) return false;

			}else {
				if(!check_expr_f(s1)) return false;
			}


			rit++;


		}else if(rit->second=="+"||rit->second=="-"){
			int p = rit->first;
			string s1 = trim(s.substr(0,p));
			string s2 = trim(s.substr(p+1,s.size()-p-1));


			if(s2.at(0)!='('){

				return check_term(s2)&&check_expr_f(s1);

			}else {

				return check_expr_f(s2)&&check_expr_f(s1);
			}


		}else if(rit->second=="*"){
			rit++;
		}
	}
	return true;
}


/// ------------validate pattern clause--------------------------------------------------------------------

//tree_node build_tree_expr(string expr);

QueryPreprocessor::tree_node QueryPreprocessor::build_tree_factor(string f){

	f = trim(f);
	if(f=="") {
		tree_node t = {"",NULL,NULL};
		return t;
	}

	if(check_IDENT(f)||check_Integer(f)) {
		tree_node a = {f,NULL,NULL};
		return a;
	}

	if(f.at(0)=='('&&(f.at(f.size()-1)==')')){
		f = trim(f.substr(1,f.size()-2));
		return build_tree_expr(f);
	}
	tree_node t ={"",NULL,NULL};
	return t;
}

QueryPreprocessor::tree_node QueryPreprocessor::build_tree_term(string s){

	s = trim(s);
	if(s=="") {
		tree_node t = {"",NULL,NULL};
		return t;
	}

	std::map<int,string> expr_map;
	std::map<int,string>::reverse_iterator rit;

	unsigned int p;

	p = s.find("*");
	while(p<s.size()){
		expr_map.insert(std::pair<int,string>(p,"*"));
		p = s.find("*",p+1);
	}

	p = s.find("(");
	while(p<s.size()){
		expr_map.insert(std::pair<int,string>(p,"("));
		p = s.find("(",p+1);
	}

	p = s.find(")");
	while(p<s.size()){
		expr_map.insert(std::pair<int,string>(p,")"));
		p = s.find(")",p+1);
	}

	if(expr_map.empty()) {
		tree_node t = build_tree_factor(s);
		return t;
	}
	int balance =0;
	rit = expr_map.rbegin();

	while(rit!=expr_map.rend()){

		if(rit->second==")"){
			rit++;
			while(rit!=expr_map.rend()){

				if(rit->second==")"){
					balance++;
					rit++;
				}

				else if(rit->second=="("&&balance!=0){
					balance--;
					rit++;
				}

				else if(rit->second=="("&&balance==0)
					break;
				else rit++;

			}
			rit++;
		}else if(rit->second=="*"){
			int p = rit->first;
			string s1 = trim(s.substr(0,p));
			string s2 = trim(s.substr(p+1,s.size()-p-1));

			tree_node* t1 = new tree_node;
			tree_node* t2 = new tree_node;

			*t1 = build_tree_term(s1);
			*t2 = build_tree(s2);
			tree_node t ={"*", t1,t2};

			return t;
		}

	}
	if(s.at(0)=='('&&s.at(s.size()-1)==')')
		s = s.substr(1,s.size()-2);
	tree_node t = build_tree(s);
	return t;
}

QueryPreprocessor::tree_node QueryPreprocessor::build_tree_expr(string s){
	s = trim(s);
	if(s.at(0)=='('&&s.at(s.size()-1)==')')
		s = s.substr(1,s.size()-2);

	unsigned int p1 = s.find_last_of("+");
	unsigned int p2 = s.find_last_of("-");
	if(p1<s.size()&&p2<s.size()){
		if(p1>p2){
			string s1 = trim(s.substr(0,p1));
			string s2 = trim(s.substr(p1+1,s.size()-p1-1));

			tree_node* t1 = new tree_node;
			tree_node* t2 = new tree_node;

			*t1 = build_tree_expr(s1);
			*t2 = build_tree_term(s2);

			tree_node a ={"+",t1,t2};

			return a;


		}
		else {
			string s1 = trim(s.substr(0,p2));
			string s2 = trim(s.substr(p2+1,s.size()-p2-1));

			tree_node* t1 = new tree_node;
			tree_node* t2 = new tree_node;

			*t1 = build_tree_expr(s1);
			*t2 = build_tree_term(s2);

			tree_node a = {"-",t1,t2};

			return a;

		}
	}
	else if(p1<s.size()){

		string s1 = trim(s.substr(0,p1));
		string s2 = trim(s.substr(p1+1,s.size()-p1-1));

		tree_node* t1 = new tree_node;
		tree_node* t2 = new tree_node;

		*t1 = build_tree_expr(s1);
		*t2 = build_tree_term(s2);
		tree_node a = {"+",t1,t2};

		return a;


	}
	else if(p2<s.size()){
		string s1 = trim(s.substr(0,p2));
		string s2 = trim(s.substr(p2+1,s.size()-p2-1));

		tree_node* t1 = new tree_node;
		tree_node* t2 = new tree_node;

		*t1 = build_tree_expr(s1);
		*t2 = build_tree_term(s2);

		tree_node a ={"-",t1,t2};

		return a;

	}
	else return build_tree_term(s);
}

QueryPreprocessor::tree_node QueryPreprocessor::build_tree(string s){

	s = trim(s);
	if(!check_expr_f(s)) {
		tree_node t ={"",NULL,NULL};
		return t;
	}
	if(check_IDENT(s)||check_Integer(s)) return build_tree_factor(s);

	std::map<int,string> expr_map;
	std::map<int,string>::reverse_iterator rit;

	unsigned int p;

	p= s.find("+");
	while(p<s.size()){
		expr_map.insert(std::pair<int,string>(p,"+"));
		p = s.find("+",p+1);
	}

	p = s.find("-");
	while(p<s.size()){
		expr_map.insert(std::pair<int,string>(p,"-"));
		p = s.find("-",p+1);
	}

	p = s.find("*");
	while(p<s.size()){
		expr_map.insert(std::pair<int,string>(p,"*"));
		p = s.find("*",p+1);
	}

	p = s.find("(");
	while(p<s.size()){
		expr_map.insert(std::pair<int,string>(p,"("));
		p = s.find("(",p+1);
	}

	p = s.find(")");
	while(p<s.size()){
		expr_map.insert(std::pair<int,string>(p,")"));
		p = s.find(")",p+1);
	}


	rit = expr_map.rbegin();

	int balance=0;

	while(rit!=expr_map.rend()){

		if(rit->second==")"){

			rit++;

			while(rit!=expr_map.rend()){

				if(rit->second==")"){
					balance++;
					rit++;
				}

				else if(rit->second=="("&&balance!=0){
					balance--;
					rit++;
				}

				else if(rit->second=="("&&balance==0)
					break;
				else rit++;
			}

			if(rit==expr_map.rend()) {
				tree_node t ={"",NULL,NULL};
				return t;
			}

			rit++;


		}else if(rit->second=="+"||rit->second=="-"){
			int p = rit->first;
			string s1 = trim(s.substr(0,p));
			string s2 = trim(s.substr(p+1,s.size()-p-1));


			if(s2.at(0)!='('){

				tree_node* t1 = new tree_node;
				tree_node* t2 = new tree_node;

				*t1 = build_tree(s1);
				*t2 = build_tree_term(s2);

				tree_node a ={rit->second,t1,t2};

				return a;

			}else {
				tree_node* t1 = new tree_node;
				tree_node* t2 = new tree_node;

				*t1 = build_tree(s1);
				*t2 = build_tree(s2);

				tree_node a ={rit->second,t1,t2};

				return a;
			}

		}else if(rit->second=="*"){
			rit++;
		}
	}
	tree_node t = build_tree_term(s);
	return t;
}

string flatten(QueryPreprocessor::tree_node* t){
    string result = "";
    if(t==NULL) return result;
    result += flatten(t->left);
    result += flatten(t->right);
    result += t->content;
    result += " ";
}


bool QueryPreprocessor::check_expr_spec(string s){

	s = trim(s);
	if(s.at(0)=='"'&&s.at(s.size()-1)=='"'){

		s = s.substr(1,s.size()-2);
		return check_expr_f(s);

	}else if(s.at(0)=='_'&&s.at(s.size()-1)=='_'){

		s = s.substr(1,s.size()-2);
		return check_expr_spec(s);

	}else return false;
}

bool QueryPreprocessor::pattern_assign(string s){

	s = trim(s);
	unsigned int p0 = s.find("(");
	if(p0>s.size()) return false;

	string synonym = trim(s.substr(0,p0));
	if(get_type(synonym)!="assign") return false;
	if(check_synonym(synonym)){
		unsigned int p1 = s.find(",");
		if(p1>s.size()) return false;
		string varRef = trim(s.substr(p0+1,p1-p0-1));
		string varRef_type;
		if(check_varRef(varRef)){
			
			//get varRef type
			if(varRef=="_")
				varRef_type = "";
			else if(check_synonym(varRef))
				varRef_type = get_type(varRef_type);
			else varRef_type = "string";


			unsigned int p2 = s.find_last_of(")");
			if(p2>s.size()||(p2!=s.size()-1)) return false;
			string expr_spec = trim(s.substr(p1+1,p2-p1-1));

			if(expr_spec=="_") {

				tree_node t = build_tree_expr("_");
				string exp_tree = flatten(&t);	
				pattern p("p_assign", synonym,varRef,varRef_type,false,exp_tree);

				if(varRef_type =="variable")
					relations.push_front(&p);
				else 
					constant_relations.push_front(&p);
				return true;

			}
			else {
				if(check_expr_spec(expr_spec)){
					unsigned int p =expr_spec.find("_");

					if(p<expr_spec.size()){
						int p1 = expr_spec.find("\"");
						int p2 = expr_spec.find("\"",p1+1);

						expr_spec = expr_spec.substr(p1+1,p2-p1-1);
						tree_node t = build_tree(expr_spec);
						string exp_tree = flatten(&t);
						pattern patt("p_assign", synonym,varRef,varRef_type,true,exp_tree);
						relations.push_back(&patt);
					}
					else {
						int p1 = expr_spec.find("\"");
						int p2 = expr_spec.find("\"",p1+1);

						expr_spec = expr_spec.substr(p1+1,p2-p1-1);
						tree_node t = build_tree(expr_spec);
						string exp_tree = flatten(&t);
						pattern patt("p_assign", synonym,varRef,varRef_type,false,exp_tree);
						relations.push_back(&patt);
					}

					return true;
				}
				else return false;
			}
		}
		else return false;
	}
	else return false;

}

bool QueryPreprocessor::pattern_if(string s){

	s = trim(s);
	unsigned int p0 = s.find("(");
	if(p0>s.size()) return false;

	string synonym = trim(s.substr(0,p0));
	if(get_type(synonym)!="if") return false;
	if(check_synonym(synonym)){
		unsigned int p1 = s.find(",");
		if(p1>s.size()) return false;
		string varRef = trim(s.substr(p0+1,p1-p0-1));
		string varRef_type;
		if(check_varRef(varRef)){

			//get varRef type
			if(varRef=="_")
				varRef_type = "";
			else if(check_synonym(varRef))
				varRef_type = get_type(varRef_type);
			else varRef_type = "string";


			p0 = s.find("_");
			if(p0>s.size()) return false;
			else {
				string s1 = s.substr(p1+1,p0-p1-1);

				if(trim(s1)!="") return false;
				else {
					p1 = s.find(",",p1+1);
					if(p1>s.size()) return false;
					else {
						s1 = s.substr(p0+1,p1-p0-1);

						if(trim(s1)!="") return false;
						else {
							p0 = s.find("_",p0+1);
							if(p0>s.size()) return false;
							else {
								s1 = s.substr(p1+1,p0-p1-1);
								if(trim(s1)!="") return false;
								else {
									p1 = s.find(")");
									if(p1>s.size()) return false;
									else {
										s1 = s.substr(p0+1,p1-p0-1);
										if(trim(s1)!="") return false;
										else {
											tree_node t = build_tree_expr("_");
											string exp_tree = flatten(&t);
											pattern p("p_if",synonym,varRef,varRef_type,false,exp_tree);

											if(varRef_type =="variable")
												relations.push_front(&p);
											else 
												constant_relations.push_front(&p);
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else return false;
	}
	else return false;
}

bool QueryPreprocessor::pattern_while(string s){
	s = trim(s);
	unsigned int p0 = s.find("(");
	if(p0>s.size()) return false;

	string synonym = trim(s.substr(0,p0));
	if(get_type(synonym)!="while") return false;
	if(check_synonym(synonym)){
		unsigned int p1 = s.find(",");
		if(p1>s.size()) return false;
		string varRef = trim(s.substr(p0+1,p1-p0-1));
		string varRef_type;
		if(check_varRef(varRef)){

			//get varRef type
			if(varRef=="_")
				varRef_type = "";
			else if(check_synonym(varRef))
				varRef_type = get_type(varRef_type);
			else varRef_type = "string";


			p0 = s.find("_");
			if(p0>s.size()) return false;
			else {
				string s1 = s.substr(p1+1,p0-p1-1);
				if(trim(s1)!="") return false;
				else {
					p1 = s.find(")");
					if(p1>s.size()) return false;
					else {
						s1 = s.substr(p0+1,p1-p0-1);
						if(trim(s1)!="") return false;
						else {
							tree_node t = build_tree_expr("_");
							string exp_tree = flatten(&t);
							pattern p("p_while",synonym,varRef,varRef_type,false,exp_tree);

							if(varRef_type=="variable")
								relations.push_front(&p);
							else
								constant_relations.push_front(&p);
							return true;
						}
					}
				}
			}
		}
		else return false;
	}
	else return false;
}

bool QueryPreprocessor::patternCond(string patternCond){
	return pattern_assign(patternCond)||pattern_while(patternCond)||pattern_if(patternCond);
}


/// ----------- with clause  functions-------------------------

void QueryPreprocessor::build_attr_table(){
    //attr_entry e;
	attr_entry e={ "procedure","string" };
    attr_table["procName"] =e;

   attr_entry e2 = { "variable","string"};
    attr_table["varName"] = e;

    attr_entry e3 = {"constant","integer"};
    attr_table["value"] = e;

    attr_entry e4 = {"stmt","integer"};
    attr_table["stmt#"] = e;

    attr_entry e5 = {"prog_line","integer"};
    attr_table["prog_line"];

    /*e = {"integer","integer"};
    attr_table["integer"] = e ;

    e = {"string","string"};
    attr_table["string"] = e ; */
}

bool QueryPreprocessor::attrCompare(string s){
    unsigned int p = s.find("=");
    if(p>s.size()) return false;
    string ref1 = trim(s.substr(0,p));
    string ref2 = trim(s.substr(p+1,s.size()-p-1));

    string evaluation_type;
    bool flag1 = false;
    bool flag2 = false;
    string ref1_prefix, ref1_postfix, ref2_prefix, ref2_postfix;
	string ref1_type, ref2_type;

    int unsigned p1 = ref1.find(".");
    if(p1<ref1.size()){
        ref1_prefix = trim(ref1.substr(0,p1));
        ref1_postfix = trim(ref1.substr(p1+1,ref1.size()-p1-1));
        attr_entry e = attr_table[ref1_postfix];
        string ref1_type = get_type(ref1_prefix);
        if(ref1_type == e.prefix_type){
            flag1 = true;
            evaluation_type = e.evaluation_type;
        }

    }else{
        if(check_Integer(ref1)){
            ref1_prefix = ref1;
			ref1_type = "integer";
            evaluation_type = "integer";
            flag1 = true;
        }else if(check_synonym(ref1)){
            string temp_type = get_type(ref1);
            if(temp_type == "prog_line"){
                ref1_prefix = ref1;
				ref1_type = "prog_line";
                evaluation_type = "integer";
                flag1 = true;
            }
        }else{
            if(ref1[0]=='"' && ref1[ref1.size()-1]=='"'){
                ref1_prefix = ref1;
                ref1_type = "string";
                evaluation_type = "string";
                flag1 = true;
            }
        }
    }

    unsigned int p2 = ref2.find(".");
    if(p2<ref2.size()){
        ref2_prefix = trim(ref2.substr(0,p2));
        ref2_postfix = trim(ref2.substr(p2+1,ref2.size()-p2-1));
        attr_entry e = attr_table[ref2_postfix];
        string ref2_type = get_type(ref2_prefix);
        if(ref2_type == e.prefix_type){
            if(evaluation_type!=e.evaluation_type)
                return false;
            flag2 = true;
        }

    }else{
        if(check_Integer(ref2)){
            ref2_prefix = ref2;
            ref2_type = "integer";
            if(evaluation_type != "integer")
                return false;
            flag2 = true;
        }else if(check_synonym(ref2)){
            string ref2_type = get_type(ref2);
            if(ref2_type == "prog_line"){
                ref2_prefix = ref2;
                if(evaluation_type != "integer")
                    return false;
                flag2 = true;
            }
        }else{
            if(ref2[0]=='"' && ref2[ref2.size()-1]=='"'){
                ref2_prefix = ref2;
                ref2_type = "string";
                if(evaluation_type != "string")
                    return false;
                flag2 = true;
            }
        }
    }

    if(flag1&&flag2){
       
        attr_compare compare(ref1_prefix,ref1_type,ref2_prefix,ref2_type);

		bool b1 = ref1_type=="string"||ref1_type=="integer";
		bool b2 = ref2_type=="string"||ref2_type=="integer";
		if(b1||b2)
			constant_relations.push_front(&compare);
        else 
			relations.push_front(&compare);
        return true;
    }
    else return false;
}

bool QueryPreprocessor::attrCond(string s){
	unsigned int p = s.find("and");
	if(p<s.size()){
		string s1 = trim(s.substr(0,p));
		string s2 = trim(s.substr(p+3, s.size()-p-3));
		return attrCompare(s1)&&attrCond(s2);
	}
	else return attrCompare(s);
}


/// -----------validate functions--------------------------

bool QueryPreprocessor::validate_suchthat(string st_clause){
	st_clause = trim(st_clause);
	int p = st_clause.find("such that");
	if(p==0) {
		st_clause = st_clause.substr(9,st_clause.size()-9);
		if(relCond(st_clause)){
			//cout<< "valid such that"<<"\n";
			return true;
		}
		else {
			//cout<< "invalid such that"<<"\n";
			return false;
		}

	}
	else return false;
}

bool QueryPreprocessor::validate_with(string with_clause){
	with_clause = trim(with_clause);
	int p = with_clause.find("with");
	if(p==0) {
		with_clause = with_clause.substr(4,with_clause.size()-4);
		if(attrCond(with_clause)){
			//cout<<"valid with clause"<<"\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool QueryPreprocessor::validate_pattern(string pattern_clause){
	pattern_clause = trim(pattern_clause);
	int p = pattern_clause.find("pattern");
	if(p==0) {
		pattern_clause = pattern_clause.substr(7,pattern_clause.size()-7);
		if(patternCond(pattern_clause)){
			//cout<< "valid pattern"<<"\n";
			return true;
		}
		else {
			//cout<< "invalid pattern"<<"\n";
			return false;
		}

	}
	else return false;

}

bool QueryPreprocessor::validate_declaration(string declare_cl){

	declare_cl = trim(declare_cl);
	if(declare_cl=="") return true;

	string declaration;
	unsigned int p_semicolon = declare_cl.find(";");
	declaration = trim(declare_cl.substr(0,p_semicolon));
	while(p_semicolon<declare_cl.size()){
		declaration = trim(declaration);
		int p_space = declaration.find(" ");
		string type = declaration.substr(0,p_space);
		if(match_entity(type)){

			unsigned int p_coma = declaration.find(",");
			if(p_coma-p_space<=1) return false;

			string synonym = trim(declaration.substr(p_space+1,p_coma-p_space-1));

			if(check_synonym(synonym)&&(!exists(synonym))){
				entityReff e = {type,synonym};
				declaration_reffs.push_back(e);
			}

			else return false;

			while(p_coma<declaration.size()){
				unsigned int p_next_coma = declaration.find(",",p_coma+1);
				if(p_next_coma-p_coma<=1) return false;

				if(p_next_coma>declaration.size()){
					synonym = trim(declaration.substr(p_coma+1,declaration.size()-p_coma-1));
				}
				else synonym = trim((declaration.substr(p_coma+1,p_next_coma-p_coma-1)));


				if(check_synonym(synonym)&&(!exists(synonym))){
					entityReff e ={type,synonym};
					declaration_reffs.push_back(e);
				}

				else return false;
				p_coma = p_next_coma;
			}
		}
		else return false;

		unsigned int p_next_semicolon = declare_cl.find(";",p_semicolon+1);
		if(p_next_semicolon-p_semicolon<=1) return false;
		if(p_next_semicolon<declare_cl.size())
			declaration  = declare_cl.substr(p_semicolon+1,p_next_semicolon-p_semicolon-1);
		//declaration  = trim(declaration);
		p_semicolon = p_next_semicolon;
	}
	//cout<<"valid declaration clause"<<"\n";
	return true;
}

bool QueryPreprocessor::validate_result(string result_clause){
	result_clause = trim(result_clause);
	if(result_clause=="BOOLEAN"){
		result_reffs.push_back("BOOLEAN");
		return true;
	}
	if(check_process_tuple(result_clause)){
		//cout<< "valid result"<<"\n";
		return true;
	}
	else return false;
}

void QueryPreprocessor::group_relations(){
    int total_count = relations.size();
    int before = 0;
    int c = 0;
    map<string,int> dependence;
    vector<int> relation_map;
    for(unsigned i=0;i<relations.size();i++){
        relation_map.push_back(0);
    }

    for(unsigned i=0;i<declaration_reffs.size();i++){
        entityReff e = declaration_reffs.at(i);
        string synonym = e.synonym;
        dependence[synonym] = 0;
    }



    while(total_count>0){
        list<BaseRelation*> group;

		//get the first relation not processed and mark its dependence
        for(unsigned j =0;j<relation_map.size();j++){ 
			std::list<BaseRelation*>::iterator it = relations.begin();
            if(relation_map[j]==0){

				BaseRelation* f = *it;
				++it;

				if(f->type=="designAbstraction"){
					designAbstraction* first_da= static_cast<designAbstraction*>(f);
					string r1 = first_da->ref1;
					string r2 = first_da->ref2;
                    
					dependence[r1] = 1;
					dependence[r2] = 1;
					break;
				}
						
				else if(f->type=="pattern"){
					pattern* first_p  = static_cast<pattern*>(f);

					string r1 = first_p->synonym;
					string r2 = first_p->varRef;
                    
					dependence[r1] = 1;
					if(first_p->varRef_type=="variable")
						dependence[r2] = 1;
					break;
						
				}else {
					attr_compare* first_c = static_cast<attr_compare*>(f);
					string r1 = first_c->left_ref;
					string r2 = first_c->right_ref;
                    
					dependence[r1] = 1;
					dependence[r2] = 1;
					break;
				} 
            }
        }

		///----- main loop-----------
        do{
            before = c;
			int i=0;
            for (std::list<BaseRelation*>::iterator it=relations.begin();it!=relations.end();++it){
                if(relation_map[i]==1) continue; //relation processed

				BaseRelation* re = *it;

				string r1,r2;
				if(re->type=="designAbstraction"){

					designAbstraction* da= static_cast<designAbstraction*>(re);
					r1 = da->ref1;
					r2 = da->ref2;

				}
				else if(re->type=="pattern"){
					pattern* p = static_cast<pattern*>(re);
					r1 = p->synonym;
					r2 = p->varRef;

				}else {
					attr_compare* c = static_cast<attr_compare*>(re);
					r1 = c->left_ref;
					r2 = c->right_ref;
				}
				
                if(dependence[r1]==1||dependence[r2]==1){
                    dependence[r1] = 1;
                    dependence[r2] = 1;

					if(re->type=="designAbstraction") //design abstraction goes to back of group
						group.push_back(re);
					else	/// pattern or attr_compare goes to the front of group
						group.push_front(re);
                    c++;
                    total_count--;
                    relation_map[i] =1;
                }
				i++;
            }
        }while(c!=before);
        //clear  dependence map
        for(unsigned i=0;i<declaration_reffs.size();i++){
            entityReff e = declaration_reffs.at(i);
            string synonym = e.synonym;
            dependence[synonym] = 0;
        }

        grouped_relations.push_back(group);
        c=0;
    }
}


bool QueryPreprocessor::process_query(string query){

	int p0 = query.find("Select");
	if(p0==-1) {
		//cout<< "no select";
		return false;
	}
	declaration_cl = query.substr(0,p0);

	find_positions(query);
	if(positions.empty()) {
		if(validate_declaration(declaration_cl)){
			result_cl = query.substr(p0+6,query.size()-p0-6);
			return validate_result(result_cl);
		} else return false;
	}

	std::map<int,string>::iterator it=positions.begin();
	int p1 = it->first;
	string cl_type = it->second;

	result_cl = query.substr(p0+6, p1-p0-6);
	if(!validate_declaration(declaration_cl)||!validate_result(result_cl)) return false;

	it++;
	int p2 = it->first;



	while ( it!=positions.end()){
		if(cl_type == "such that"){
			string suchthat = query.substr(p1,p2-p1);

			if(validate_suchthat(suchthat)){
				p1 = p2;
				cl_type = it->second;
			}
			else return false;
		}
		else if(cl_type == "with"){
			string with = query.substr(p1,p2-p1);
			if(validate_with(with)){
				p1 = p2;
				cl_type = it->second;
			}
			else return false;
		}
		else {
			string pattern = query.substr(p1,p2-p1);

			if(validate_pattern(pattern)){
				p1 = p2;
				cl_type = it->second;
			}
			else return false;
		}
		it++;
		p2 = it->first;
	}

	if(cl_type == "such that"){
		string suchthat = query.substr(p1,query.size()-p1);
		return validate_suchthat(suchthat);
	}
	else if(cl_type == "with"){
		string with = query.substr(p1,query.size()-p1);
		return validate_with(with);
	}
	else {
		string pattern = query.substr(p1,query.size()-p1);
		return validate_pattern(pattern);
	}

}

/// -----print functions-------------------------
void QueryPreprocessor::print_declaration(){
	for(unsigned int i=0;i<declaration_reffs.size();i++){
		cout<<declaration_reffs.at(i).type<<" "<<declaration_reffs.at(i).synonym<<"\n";
	}
}

void QueryPreprocessor::print_result(){
	for(unsigned int i=0;i<result_reffs.size();i++){
		cout<<result_reffs.at(i)<<" ";
	}
	cout<<"\n";
}
/*
void QueryPreprocessor::print_relations(){
    for(unsigned int i=0;i<relations.size();i++){
            BaseRelation* d = relations.at(i);
			designAbstraction* da = static_cast<designAbstraction*>(da);
            cout<<da->relation_type<<" "<<da->ref1<<" "<<da->ref1_type<<" "<<da->ref2<<" "<<da->ref2_type<<"\n";
    }
}

void QueryPreprocessor::print_tree(tree_node t){

	cout<< t.content<<" ";
	if(t.left!=NULL) print_tree(*(t.left));
	if(t.right!=NULL) print_tree(*(t.right));

}

void QueryPreprocessor::print_patterns(){
	for(unsigned int i=0;i<patterns.size();i++){
		/*cout<<patterns.at(i).type<<" "<<patterns.at(i).synonym<<" "<<patterns.at(i).varRef<<" "<<patterns.at(i).exact<<"\n";
		tree_node* t = patterns.at(i).expr_tree;
		//print_tree(t); 
	}
}

void QueryPreprocessor::print_attr_pairs(){
    /*for(unsigned int i=0;i<attr_pairs.size();i++){
            attr_compare compare = attr_pairs.at(i);
            attrRef left = compare.left_ref;
            attrRef right = compare.right_ref;
            cout<<left.prefix<<"  "<<left.postfix<<" "
            <<right.prefix<<" "<<right.postfix<<"\n"; 
    }
} */

void QueryPreprocessor::print_query(){
	print_declaration();
	print_result();
	/*print_relations();
	print_patterns();
	print_attr_pairs(); */
}





