#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include "QueryPreprocessor.h"
using namespace std;


static string designEntity[] = {"procedure","stmtList", "stmt", "assign", "call", "while", "if", "variable", "constant", "prog_line"};
static const int NumDE = 10;


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

bool QueryPreprocessor::check_entRef(string entRef){

	if(check_synonym(entRef)){
		if(!exists((entRef))) return false;
		else return true;
	}

	if(entRef.at(0)=='\"'&&entRef.at(entRef.size()-1)=='\"'){

		entRef = entRef.substr(1,entRef.size()-2);
	}

	return entRef=="_"||check_IDENT(entRef)||check_Integer(entRef);
}

bool QueryPreprocessor::check_stmtRef(string stmtRef){

	if(check_synonym(stmtRef)){
		if(!exists((stmtRef))) return false;
		else return true;
	}
	return stmtRef=="_"||check_Integer(stmtRef);
}

bool QueryPreprocessor::check_lineRef(string lineRef){
	return check_stmtRef(lineRef);
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

bool QueryPreprocessor::ModifiesP(string s){
	s = trim(s);
	unsigned int p = s.find("Modifies");
	if(p!=0) return false;
	else {
		s = s.substr(8,s.size()-8);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_entRef(s1)&&check_varRef(s2))
					{
						designAbstraction da = {"ModifiesP",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::ModifiesS(string s){
	s = trim(s);
	unsigned int p = s.find("Modifies");
	if(p!=0) return false;
	else {
		s = s.substr(8,s.size()-8);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_stmtRef(s1)&&check_varRef(s2))
					{
						designAbstraction da = {"ModifiesS",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::UsesP(string s){
	s = trim(s);
	unsigned int p = s.find("Uses");
	if(p!=0) return false;
	else {
		s = s.substr(4,s.size()-4);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_entRef(s1)&&check_varRef(s2))
					{
						designAbstraction da = {"UsesP",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::UsesS(string s){
	s = trim(s);
	unsigned int p = s.find("Uses");
	if(p!=0) return false;
	else {
		s = s.substr(4,s.size()-4);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_stmtRef(s1)&&check_varRef(s2))
					{
						designAbstraction da = {"UsesS",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::Calls(string s){
	s = trim(s);
	unsigned int p = s.find("Calls");
	if(p!=0) return false;
	else {
		s = s.substr(5,s.size()-5);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_entRef(s1)&&check_entRef(s2))
					{
						designAbstraction da = {"Calls",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::CallsT(string s){
	s = trim(s);
	unsigned int p = s.find("Calls*");
	if(p!=0) return false;
	else {
		s = s.substr(6,s.size()-6);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_entRef(s1)&&check_entRef(s2))
					{
						designAbstraction da = {"CallsT",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::Parent(string s){
	s = trim(s);
	unsigned int p = s.find("Parent");
	if(p!=0) return false;
	else {
		s = s.substr(6,s.size()-6);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_stmtRef(s1)&&check_stmtRef(s2))
					{
						designAbstraction da ={"Parent",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::ParentT(string s){
	s = trim(s);
	unsigned int p = s.find("Parent*");
	if(p!=0) return false;
	else {
		s = s.substr(7,s.size()-7);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_stmtRef(s1)&&check_stmtRef(s2))
					{
						designAbstraction da = {"ParentT",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::Follows(string s){
	s = trim(s);
	unsigned int p = s.find("Follows");
	if(p!=0) return false;
	else {
		s = s.substr(7,s.size()-7);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_stmtRef(s1)&&check_stmtRef(s2))
					{
						designAbstraction da ={"Follows",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::FollowsT(string s){
	s = trim(s);
	unsigned int p = s.find("Follows*");
	if(p!=0) return false;
	else {
		s = s.substr(8,s.size()-8);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_stmtRef(s1)&&check_stmtRef(s2))
					{
						designAbstraction da = {"FollowsT",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::Next(string s){
	s = trim(s);
	unsigned int p = s.find("Next");
	if(p!=0) return false;
	else {
		s = s.substr(4,s.size()-4);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_lineRef(s1)&&check_lineRef(s2))
					{
						designAbstraction da ={"Next",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::NextT(string s){
	s = trim(s);
	unsigned int p = s.find("Next*");
	if(p!=0) return false;
	else {
		s = s.substr(5,s.size()-5);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_lineRef(s1)&&check_lineRef(s2)){
						designAbstraction da = {"NextT",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::Affects(string s){
	s = trim(s);
	unsigned int p = s.find("Affects");
	if(p!=0) return false;
	else {
		s = s.substr(7,s.size()-7);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_stmtRef(s1)&&check_stmtRef(s2)){
						designAbstraction da = {"Affects",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::AffectsT(string s){
	s = trim(s);
	unsigned int p = s.find("Affects*");
	if(p!=0) return false;
	else {
		s = s.substr(8,s.size()-8);
		s = trim(s);
		p = s.find("(");
		if(p!=0) return false;
		else {
			s = s.substr(1,s.size()-1);
			s = trim(s);

			p = s.find(",");
			if(p>s.size()) return false;
			else {
				string s1 = trim(s.substr(0,p));
				string s2 = trim(s.substr(p+1,s.size()-p-2));

				if(s.at(s.size()-1)==')'){
					if(check_stmtRef(s1)&&check_stmtRef(s2)){
						designAbstraction da ={"AffectsT",s1,s2};
						relations.push_back(da);
						return true;
					}
					else return false;

				}
				else return false;
			}
		}
	}
}

bool QueryPreprocessor::relRef(string relRef){
	return ModifiesP(relRef)||ModifiesS(relRef)||UsesP(relRef)||UsesS(relRef)||Calls(relRef)||CallsT(relRef)||Parent(relRef)||ParentT(relRef)||Follows(relRef)||FollowsT(relRef)||Next(relRef)||NextT(relRef)||Affects(relRef)||AffectsT(relRef);
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

/// -------------------------------------End such that-----------------------------



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

QueryPreprocessor::DE_TYPE QueryPreprocessor::get_type(string synonym){
	for(unsigned int i=0;i<declaration_reffs.size();i++){
		if(synonym == declaration_reffs.at(i).synonym)
			return declaration_reffs.at(i).type;
	}
	return "";
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
		if(check_varRef(varRef)){
			unsigned int p2 = s.find_last_of(")");
			if(p2>s.size()||(p2!=s.size()-1)) return false;
			string expr_spec = trim(s.substr(p1+1,p2-p1-1));
			if(expr_spec=="_") {
				tree_node t = build_tree_expr("_");
				pattern p = {"p_assign", synonym,varRef,false,t};
				patterns.push_back(p);
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
						pattern patt = {"p_assign", synonym,varRef,false,t};
						patterns.push_back(patt);
					}
					else {
						int p1 = expr_spec.find("\"");
						int p2 = expr_spec.find("\"",p1+1);

						expr_spec = expr_spec.substr(p1+1,p2-p1-1);
						tree_node t = build_tree(expr_spec);
						pattern patt = {"p_assign", synonym,varRef,true,t};
						patterns.push_back(patt);
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
		if(check_varRef(varRef)){
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
											pattern p = {"p_if",synonym,varRef,false,t};
											patterns.push_back(p);
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
		if(check_varRef(varRef)){
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
							pattern p ={"p_while",synonym,varRef,false,t};
							patterns.push_back(p);
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

bool QueryPreprocessor::check_ref(string s){
	s = trim(s);
	if(check_synonym(s)){
		if(exists(s)) return true;
		else return false;
	}
	if(s.at(0)=='"'&&s.at(s.size()-1)=='"')
		s = s.substr(1,s.size()-2);
	return check_IDENT(s)||check_Integer(s)||check_attrRef(s);
}


bool QueryPreprocessor::attrCompare(string s){
	unsigned int p = s.find("=");
	if(p>s.size()) return false;
	string s1 = trim(s.substr(0,p));
	string s2 = trim(s.substr(p+1,s.size()-p-1));
	if(check_ref(s1)&&check_ref(s2)){
		attr_compare a = {s1,s2};
		attr_pairs.push_back(a);
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

    if(positions.size()>1){
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
			if(it!=positions.end()){			
				p2 = it->first;
			}
            
        }
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

void QueryPreprocessor::print_relations(){
	for(unsigned int i=0;i<relations.size();i++){
		cout<<relations.at(i).type<<" "<<relations.at(i).ref1<<" "<<relations.at(i).ref2<<"\n";
	}
}

void QueryPreprocessor::print_tree(tree_node t){

	cout<< t.content<<" ";
	if(t.left!=NULL) print_tree(*(t.left));
	if(t.right!=NULL) print_tree(*(t.right));

}
void QueryPreprocessor::print_patterns(){
	for(unsigned int i=0;i<patterns.size();i++){
		cout<<patterns.at(i).type<<" "<<patterns.at(i).synonym<<" "<<patterns.at(i).varRef<<" "<<patterns.at(i).exact<<"\n";
		tree_node t = patterns.at(i).expr_tree;
		print_tree(t);
	}
}


void QueryPreprocessor::print_attr_pairs(){
	for(unsigned int i=0;i<attr_pairs.size();i++){
		cout<<attr_pairs.at(i).attr_left<<"  "<<attr_pairs.at(i).attr_right<<"\n";
	}
}

void QueryPreprocessor::print_query(){
	print_declaration();
	print_result();
	print_relations();
	print_patterns();
	print_attr_pairs();
}

///---------------------getter functions--------------------------
vector<QueryPreprocessor::entityReff> QueryPreprocessor::getSymbols(){
	return declaration_reffs;
}

vector<string> QueryPreprocessor::getResults(){
	return result_reffs;
}

vector<QueryPreprocessor::designAbstraction> QueryPreprocessor::getRelations(){
	return relations;
}

vector<QueryPreprocessor::pattern> QueryPreprocessor::getPatterns(){
	return patterns;
}

vector<QueryPreprocessor::attr_compare> QueryPreprocessor::getAttrPairs(){
	return attr_pairs;
}
/// -------------main function-------------------------------------------
/*int main()
{
string query;
ifstream infile;

infile.open("query.txt");

while(!infile.eof()){
getline(infile,query);
cout<<query<<"\n";
if(process_query(query)){

cout<<"valid query\n";
print_query();
}
else cout<<"invalid query\n";

/// clear vectors
positions.clear();
declaration_reffs.clear();
result_reffs.clear();
patterns.clear();
relations.clear();
attr_pairs.clear();
cout<<"\n";
}

return 0;
}  */


