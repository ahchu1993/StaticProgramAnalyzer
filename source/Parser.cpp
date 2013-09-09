#include "Parser.h"
#include <string.h>
#include <stdlib.h>

using namespace std;

Parser::Parser(PKB* p) {
	pkb = p;
	inputArray = 0;
	currentPossition = 0;
	line = 0;
	inputSize = 0;
}

bool Parser::parseInput (string in) {
	initializeInput(in);
	codeProcess();
	return true;
}

bool Parser::isNumber(string numberString) {
	if (atoi(numberString.c_str()) == 0 && numberString.compare("0") != 0) {
		return false;
	}
	return true;
}

bool Parser::match(string token) {
	if (nextToken.compare(token) == 0) {
		getToken();
		return true;
	} else {
		error();
		return false;
	}
}

void Parser::initializeInput (string input) {
	inputSize = input.size();
	inputArray = new char[inputSize];
	memcpy(inputArray,input.c_str(), inputSize);
	currentPossition = 0;
}

bool Parser::isSpecialChar (char letter) {
	if(letter != '\t' && letter != ' ' && letter != ';'	&& letter != '=' && letter != '{' && letter != '}' && letter != '+') {
		return true;
	}
	return false;
}

bool Parser::isOneLetterToken (char letter) {
	if (letter == ';' || letter == '=' || letter == '{' || letter == '}' || letter == '+' ) {
		return true;
	}
	return false;
}

string Parser::getToken() {
	nextToken = "";
	while (currentPossition < inputSize) {
		if(isSpecialChar(inputArray[currentPossition])) {
			nextToken += inputArray[currentPossition++];
		} else {
			if (isOneLetterToken(inputArray[currentPossition])) {
				if (nextToken.length() == 0) {
					nextToken += inputArray[currentPossition++];
					break;
				} else {
					break;
				}
			} else {
				if (nextToken.length() == 0) {
					currentPossition++;
				} else {
					break;
				}
			}
		}
	}
	return nextToken;
}




bool Parser::codeProcess() {
	program();
	return true;
}



bool Parser::program () {
	//getToken();
	//return procedure ();
	//fstream myfile;
	//myfile.open ("hh.txt");
	


	getToken();
	while(nextToken.length()!=0){
		procedure();
	}
	return true;
}

bool Parser::procedure() {
	match ("procedure");
	procName = nextToken;
	pkb->insertProc(procName);
	getToken();
	match ("{");
	vector<int> stmtListNumber;
	stmtLst(vector < PairNumber >());
	match ("}");
	if (currentPossition < inputSize - 1) {
		//error();
		// zhao yang add, not error anymore
	}
	return true;
}

bool Parser::stmtLst(vector < PairNumber > useModifyList) {
	int previousStmtLine = 0;
	vector<int> stmtListNumber;
	while (nextToken.compare("}") != 0) {
		stmtListNumber.push_back(processStmt(stmtListNumber, useModifyList));
		 
	}
	return true;
}

int Parser::processStmt(vector<int> stmtListNumber, vector < PairNumber > useModifyList) {
	int t=line++;
	stmtListNumber.push_back(line);
	//*previousStmtLine = line;
	stmt(stmtListNumber, useModifyList);
	return t+1;
}

void Parser::processWhile (vector < PairNumber > useModifyList) {
	pkb->insertStmt(line, "while");
	if (useModifyList.size() > 0) {
		pkb->insert(useModifyList[useModifyList.size() - 1].index, useModifyList[useModifyList.size() - 1].DE, line, "while");
	}
	useModifyList.push_back(PairNumber(line, "while"));
	w(useModifyList);
}

void Parser::processIf (vector < PairNumber > useModifyList) {
	pkb->insertStmt(line, "if");
	if (useModifyList.size() > 0) {
		pkb->insert(useModifyList[useModifyList.size() - 1].index, useModifyList[useModifyList.size() - 1].DE, line, "if");
	}
	useModifyList.push_back(PairNumber(line, "if"));
	ifProcess(useModifyList);
}

void Parser::processAssignment (vector < PairNumber > useModifyList) {
	pkb->insertStmt(line, "assign");
	if (useModifyList.size() > 0) {
		pkb->insert(useModifyList[useModifyList.size() - 1].index, useModifyList[useModifyList.size() - 1].DE, line, "assign");
	}
	assign(useModifyList);
}

int Parser::stmt(vector<int> stmtListNumber,vector < PairNumber > useModifyList) {
	if (nextToken.compare("while") == 0) {
		if (stmtListNumber.size()>1) {
			int p = stmtListNumber[stmtListNumber.size()-2];
			//printf("##Follow  %d  %d \n",*previousStmtLine,line);
			pkb->insertFollow(p,pkb->getStmtType(p), line,"while");
		}
		processWhile(useModifyList);
	} else if(nextToken.compare("if") == 0) {
		if (stmtListNumber.size()>1) {
			int p = stmtListNumber[stmtListNumber.size()-2];
			//printf("##Follow  %d  %d \n",*previousStmtLine,line);
			pkb->insertFollow(p,pkb->getStmtType(p), line,"if");
		}
		processIf(useModifyList);
	} else if(nextToken.compare("call") == 0) {
		getToken();
		string calledProcedure = nextToken;
		pkb->insert(procName,calledProcedure);
		getToken();
		match(";");
		// zhao yang, procName call calledProcedure;
	}else {
		if (stmtListNumber.size()>1) {
			int p = stmtListNumber[stmtListNumber.size()-2];
			//printf("##Follow  %d  %d \n",*previousStmtLine,line);
			pkb->insertFollow(p,pkb->getStmtType(p), line,"assign");
		}
		processAssignment(useModifyList);
	}
	return -1;
}

bool Parser::assign(vector < PairNumber > useModifyList) {
	TNode* assignNode = pkb->createRootNode("root", -1, line);
	string varName = checkVariable(nextToken);
	getToken();
	int varIndex = pkb->getVarIndex(varName);
	if (varIndex < 0) {
		varIndex = (*pkb).insertVar(varName);
	}

	//modify
	pkb->insertModifyProc(pkb->getProcIndex(procName), varIndex);
	pkb->insertModifyStmt(line, varIndex, "assign");
	for (unsigned i = 0; i < useModifyList.size(); i++) {
		pkb->insertModifyStmt(useModifyList.at(i).index, varIndex, useModifyList.at(i).DE);
	}

	//AST
	TNode* varNode = pkb->createNode("var", varIndex, line);
	pkb->makeLeftChild(assignNode, varNode);
	match("=");
	pkb->makeRightChild(assignNode, expr(useModifyList, getFactorListString()));
	match (";");
	return true;
}

bool Parser::w(vector < PairNumber > useModifyList) {
	getToken();
	int varIndex = pkb->getVarIndex(checkVariable(nextToken));
	if (varIndex < 0) {
		varIndex = (*pkb).insertVar(nextToken);
	}
	//use
	pkb->insertUseProc(pkb->getProcIndex(procName), varIndex);
	useModifyList.push_back(PairNumber(line, "while"));
	for (unsigned i = 0; i < useModifyList.size(); i++) {
		pkb->insertUseStmt(useModifyList.at(i).index, varIndex, useModifyList.at(i).DE);
	}
	getToken();
	match ("{");
	int whileLine = line;
	stmtLst(useModifyList);
	if (whileLine == line) {
		error();
	}
	match ("}");
	return true;
}

bool Parser::ifProcess(vector < PairNumber > useModifyList) {
	getToken();
	int varIndex = pkb->getVarIndex(checkVariable(nextToken));
	if (varIndex < 0) {
		varIndex = (*pkb).insertVar(nextToken);
	}
	//use
	pkb->insertUseProc(pkb->getProcIndex(procName), varIndex);
	useModifyList.push_back(PairNumber(line, "if"));
	for (unsigned i = 0; i < useModifyList.size(); i++) {
		pkb->insertUseStmt(useModifyList.at(i).index, varIndex, useModifyList.at(i).DE);
	}
	getToken();
	match ("then");
	match ("{");
	int ifLine = line;
	stmtLst(useModifyList);
	if (ifLine == line) {
		error();
	}
	match ("}");
	return true;
}

string Parser::getFactorListString () {
	string factorList = "";
	bool flag = false;
	while (nextToken.compare(";") != 0) {
		if (flag) {
			match("+");
		}
		string factorName = nextToken;
		factorList=factorList+factorName;
		//flag = true;
		getToken();
	}
	if (factorList.length() == 0) {
		error();
	}
	return factorList;
}

TNode* Parser::processFactor (string factor, vector < PairNumber > useModifyList) {
	if (isNumber(factor)) {
		int factorNumber = atoi(factor.c_str());
		TNode* factorNode = pkb->createNode("const", factorNumber, line);
		pkb->insertConst(line,factorNumber);
		return factorNode;
	} else {
		int varIndex = pkb->getVarIndex(checkVariable(factor));
		if (varIndex < 0) {
			varIndex = (*pkb).insertVar(factor);
		}
		//AST
		TNode* factorNode = pkb->createNode("var", varIndex, line);

		//use
		pkb->insertUseProc(pkb->getProcIndex(procName), varIndex);
		pkb->insertUseStmt(line, varIndex, "assign");
		for (unsigned i = 0; i < useModifyList.size(); i++) {
			pkb->insertUseStmt(useModifyList.at(i).index, varIndex, useModifyList.at(i).DE);
		}
		return factorNode;
	}
}

int Parser::findRightMostOperation(string factorList, int flag){
	if(flag==0){ // find + -
		int plusIndex = factorList.find_last_of('+');
		int minusIndex = factorList.find_last_of('-');
		return max(plusIndex,minusIndex);
	}else{
		return factorList.find_last_of('*');
	}
	return -1;
}

TNode* Parser::expr (vector < PairNumber > useModifyList, string factorList) {
	// factorList     x  +  y * z + y * x + 1;
		int indexOfPlusMinusOperation = findRightMostOperation(factorList,0);
		//cout<<"??  "<<indexOfPlusMinusOperation<<"   "<<factorList<<endl;
		//getchar();
		if(indexOfPlusMinusOperation==-1){  // no +/-
			int indexOfMulOperation = findRightMostOperation(factorList,1);
			if(indexOfMulOperation==-1){  // no *
				return processFactor(factorList, useModifyList);
			}else{
				string factorList1 = factorList.substr(0,indexOfMulOperation);
				string factor = factorList.substr(indexOfMulOperation+1,factorList.length());

				TNode* mulNode = pkb->createNode("opt", -1, line);  //TMP
				pkb->makeRightChild(mulNode, processFactor(factor, useModifyList));
				pkb->makeLeftChild(mulNode, expr(useModifyList, factorList1));
				return mulNode;
			}
		}else{
			string factorList1 = factorList.substr(0,indexOfPlusMinusOperation);
			string factorList2 = factorList.substr(indexOfPlusMinusOperation+1,factorList.length());

			if(factorList[indexOfPlusMinusOperation]=='+'){// +
				TNode* plusNode = pkb->createNode("opt", -1, line);  //TMP
				pkb->makeRightChild(plusNode, expr(useModifyList, factorList2));
				pkb->makeLeftChild(plusNode, expr(useModifyList, factorList1));
				return plusNode;
			}else{ //-
				TNode* minusNode = pkb->createNode("opt", -1, line);  //TMP
				pkb->makeRightChild(minusNode, expr(useModifyList, factorList2));
				pkb->makeLeftChild(minusNode, expr(useModifyList, factorList1));
				return minusNode;
			}
		}
		


		//return pkb->createNode("opt", -1, line); // dummy
	/*else if (factorList.size() == 1) {
		return processFactor(factorList[0], useModifyList);
	} else if (factorList.size() == 2){
		TNode* plusNode = pkb->createNode("opt", -1, line);
		pkb->makeLeftChild(plusNode, processFactor(factorList[0], useModifyList));
		pkb->makeRightChild(plusNode, processFactor(factorList[1], useModifyList));
		return plusNode;
	} else {
		TNode* plusNode = pkb->createNode("opt", -1, line);
		pkb->makeRightChild(plusNode, processFactor(factorList[factorList.size() - 1], useModifyList));
		factorList.erase(factorList.end() - 1);
		pkb->makeLeftChild(plusNode, expr(useModifyList, factorList));
		return plusNode;
	}*/
	
}

string Parser::checkVariable(string variable) {
	if (variable.compare("while") == 0 || variable.compare("if") == 0 || variable.compare(procName) == 0) {
		error();
	}
	char* variableArray = new char[variable.size()];
	memcpy(variableArray,variable.c_str(), variable.size());
	if (!isalpha(variableArray[0])) {
		error();
	}
	for (unsigned i = 0; i < variable.size(); i++) {
		if (!isdigit(variableArray[i]) && !isalpha(variableArray[i])) {
			error();
		}
	}
	return variable;
}

void Parser::error() {
	cout << "ERROR" << endl;
	exit(1);
}

