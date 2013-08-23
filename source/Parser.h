#include <iostream>
#include <fstream>
#include <vector>
#include "PKB.h"
using namespace std;

struct PairNumber {
	int index;
	string DE;
	PairNumber (int i, string de) {
		index = i;
		DE = de;
	}
};

class Parser
{
public:
	Parser(PKB* p);
	bool parseInput(string in);

private:
	string nextToken;
	int line;
	string procName;
	int currentPossition;
	char *inputArray;
	int inputSize;
	PKB *pkb;

	string checkVariable (string variable);
	void initializeInput(string input);
	bool isNumber(string numberString);
	bool codeProcess();
	void stop();
	void error();
	void processIf (vector < PairNumber > useModifyList);
	void processWhile (vector < PairNumber > useModifyList);
	void processAssignment (vector < PairNumber > useModifyList);
	int processStmt(vector<int> stmtListNumber, vector < PairNumber > useModifyList);
	bool isOneLetterToken (char letter);
	bool isSpecialChar (char letter);
	void processWhile ();
	TNode* expr (vector < PairNumber > useModifyList, vector < string > factorList);
	bool program ();
	bool ifProcess(vector < PairNumber > useModifyList);
	bool w(vector < PairNumber > useModifyList);
	bool assign(vector < PairNumber > useModifyList);
	int stmt(vector<int> stmtListNumber,vector < PairNumber > useModifyList);
	bool stmtLst(vector < PairNumber > useModifyList);
	bool procedure();
	string getToken();
	vector < string > getFactorList ();
	TNode* processFactor (string factor, vector < PairNumber > useModifyList);
	bool match(string token);
};

