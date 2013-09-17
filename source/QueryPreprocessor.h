#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
using namespace std;

class QueryPreprocessor
{	
public:
	QueryPreprocessor();
	~QueryPreprocessor(void);

	typedef string DE_TYPE;
	typedef string DA_TYPE;
	typedef string Pattern_TYPE;

	struct entityReff {
		DE_TYPE type;
		string synonym;
	};

	struct designAbstraction{
    string relation_type;
    string ref1;
    string ref1_type;
    string ref2;
    string ref2_type;
};

	struct tree_node{
		string content;
		tree_node* left;
		tree_node* right;
	};

	struct pattern {
		Pattern_TYPE type;
		string synonym;
		string varRef;
		bool exact;   ///flag to tell whether it is a exact match or a partial match(depends on "_")
		tree_node expr_tree;

	};

	struct attrRef{
		string prefix;
		string postfix;  //use postfix determine type
	};
	struct attr_compare{
		attrRef left_ref;
		attrRef right_ref;
		string evaluation_type;
	};

	struct arg_type_list{
		vector<string> synonym_type;
		bool underscore;
		bool int_type;
		bool string_type;
	};

	struct entry{
		arg_type_list arg1_list;
		arg_type_list arg2_list;
	};
	
	struct attr_entry{
		string prefix_type;
		string evaluation_type;
	};

	/// ---------------------------build table for table driven parsing---------------------------------
	void fillArg(arg_type_list* arg_list,bool undersc, bool int_t, bool string_t);
	void addModifiesEntry();
	void addUsesEntry();
	void addCallsEntry();
	void addCallsTEntry();
	void addParentEntry();
	void addParentTEntry();
	void addFollowsEntry();
	void addFollowsTEntry();
	void addNextEntry();
	void addNextTEntry();
	void addAffectsEntry();
	void addAffectsTEntry();
	void buildTable();
	void print_relation(string r);
	void print_table();
	
	void build_attr_table();

	///------------------auxiliary functions---------------------------------
	string trim(string str);
	bool exists(string reff);
	void find_positions(string query);
	///--------------------Lexical rules-------------------------------------
	bool check_Letter(char c);
	bool check_Digit(char c);
	bool check_IDENT(string ident);
	bool check_Integer(string i);
	///------------------query grammar rules api-----------------------------
	bool check_attrName(string name);
	bool check_synonym(string synonym);
	bool check_attrRef(string attrRef);
	bool check_elem(string elem);
	bool check_varRef(string s);
	bool match_entity(string token);
	bool check_process_tuple(string t);

	///-----------------design abstraction validation & process------------------
	
	bool relRef(string relRef);
	bool relCond(string s);

	///------------- SIMPLE expression rules----------------------
	bool check_factor(string s);
	bool check_term(string s);
	bool check_expr(string s);   /// only deals with expression with no brackets.
	bool check_expr_f(string s); /// entry function, deals with complex expression

	/// ------------ build a syntax tree for SIMPLE expressions----------
	tree_node build_tree_factor(string f);
	tree_node build_tree_term(string s);
	tree_node build_tree_expr(string s);
	tree_node build_tree(string s);   ///entry function, build tree for complex expr.


	///-------------------validate pattern clauses------------------
	bool check_expr_spec(string s);
	DE_TYPE get_type(string synonym);
	bool pattern_assign(string s);
	bool pattern_if(string s);
	bool pattern_while(string s);
	bool patternCond(string patternCond);

	///------------------------functions to validate with clause---------------- 
	
	bool attrCompare(string s);
	bool attrCond(string s);

	///-------------------main functions to validate all clauses-------------
	bool validate_suchthat(string st_clause);
	bool validate_with(string with_clause);
	bool validate_pattern(string pattern_clause);
	bool validate_declaration(string declare_cl);
	bool validate_result(string result_clause);

	
	///--------------entry point to process a query-----------------------------------
	bool process_query(string st_clause);


	///----------helper functions to print info--------------
	void print_declaration();
	void print_result();
	void print_relations();
	void print_tree(tree_node t);
	void print_patterns();
	void print_attr_pairs();
	void print_query();

	///---------------------getters ---------------------------
	vector<entityReff> getSymbols();
	vector<string> getResults();
	vector<designAbstraction> getRelations();
	vector<pattern> getPatterns();
	vector<attr_compare> getAttrPairs(); 

	
private:

	string query, declaration_cl,result_cl,suchthat_cl,pattern_cl;
	std::map<int,string> positions;
	
	/// table is used for arguments type checking  -- table driven
	map<string,entry> table;
	map<string,attr_entry> attr_table;
	
	/// declaration_reff store all declarations(type, synonym);
	vector<entityReff> declaration_reffs;

	/// result_reffs store all result synonyms
	vector<string> result_reffs;

	/// relations store all design abstractions (designabstraction, ref1,ref2)
	vector<designAbstraction> relations;

	/// pattern store all patterns(pattern_type, synonym, varRef, expr_spec)
	vector<pattern> patterns;

	/// attr_pairs store all attr_compare(left attr , right attr) in with clause
	vector<attr_compare> attr_pairs;
	
	
};


