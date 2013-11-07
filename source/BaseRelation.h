#include <iostream>
using namespace std;


class BaseRelation{

public:
	string type;

};

class designAbstraction:public BaseRelation{
	public:
    string relation_type;
    string ref1;
    string ref1_type;
    string ref2;
    string ref2_type;

    designAbstraction(string r_t,string r1,string r1_t,string r2,string r2_t){
       type = "designAbstraction";
       relation_type = r_t;
       ref1 = r1;
       ref1_type = r1_t;
       ref2 = r2;
       ref2_type = r2_t;
    }
};

class pattern: public BaseRelation {
	
public:
	string pattern_type;
	string synonym;
	string varRef;
	string varRef_type;
	bool exact;   ///flag to tell whether it is a exact match or a partial match(depends on "_")
	string expr_tree;

	pattern(string p_t,string s,string v,string v_t,bool e,string exp_tree){
		type = "pattern";
		pattern_type = p_t;
		synonym = s;
		varRef = v;
		varRef_type = v_t;
		exact = e;  
		expr_tree = exp_tree;
	}
};

class attr_compare: public BaseRelation{
public:
	string left_ref;
	string left_ref_type;
	string right_ref;
	string right_ref_type;
	string evaluation_type;

	attr_compare(string left_r,string left_t,string right_r,string right_t,string e_type){
		type = "attr_compare";
		left_ref = left_r;
		left_ref_type = left_t;
		right_ref = right_r;
		right_ref_type = right_t;
		string evaluation_type = e_type;
	}

};