#include "TestWrapper.h"
#include <fstream>
// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;




// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program


}

PKB* pkb = new PKB;

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...

	
	Parser parser(pkb);
	string input="";
	ifstream infile;

	infile.open(filename);
	string temp;
	while(!infile.eof()){
		getline(infile,temp);
		input = input+" "+temp;

	}
	
	parser.parseInput(input);
	cout<<"==========================PKB PRINT=========================="<<endl;
	//pkb->printStmtTable();
	//pkb->printVarTable();
	//pkb->printProcTable();
	//pkb->printCallTable();
	//pkb->printAST();
	//pkb->printFollowTable();
	//pkb->printModifyTable();
	//pkb->printParentTable();
	//pkb->printUseTable();
	//pkb->printConstTable();

	getchar();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
	QueryEvaluator qe(pkb);
	results = qe.processQuery(query);
}