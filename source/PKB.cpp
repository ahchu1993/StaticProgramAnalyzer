#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
#include "PKB.h"

//PKB* PKB::pInstance = NULL;
PKB::PKB(){

}

/*PKB* PKB::instance(){
	if(!pInstance){
		pInstance = new PKB();
	}
	return pInstance;
} */

//AST
TNode* PKB::createRootNode(string type, int content, int lineNo){
	return ast.createRootNode(type, content, lineNo);
}
TNode* PKB::createNode(string type, int content, int lineNo){
	return ast.createNode(type,content,lineNo);
}
void PKB::makeLeftChild(TNode* parent, TNode* leftChild){
	ast.makeLeftChild(parent, leftChild);
}
void PKB::makeRightChild(TNode* parent, TNode* rightChild){
	ast.makeRightChild(parent, rightChild);
}
TNode* PKB::getLeftChild(TNode* parent){
	return ast.getLeftChild(parent);
}
TNode* PKB::getRightChild(TNode* parent){
	return ast.getRightChild(parent);
}
int PKB::getContent(TNode* node)
{
	return ast.getContent(node);
}
int PKB::getLineNo(TNode* node)
{
	return ast.getLineNo(node);
}
string PKB::getType(TNode* node)
{
	return ast.getType(node);
}
vector<TNode*> PKB::getAST()
{
	return ast.getAST();
}
void PKB::printAST()
{
	ast.printAST();
}

//CallTable
void PKB::insert(string proc1, string proc2){
	callTable.insert(proc1,proc2);
}
bool PKB::isCalled(string proc1, string proc2){
	return callTable.isCalled(proc1,proc2);
}
vector<string> PKB::getCallsList(string procName){
	return callTable.getCallsList(procName);
}
vector<string> PKB::getCalledList(string procName){
	return callTable.getCalledList(procName);
}
void PKB::printCallTable(){
	callTable.print();
}

//ParentTable
void PKB::insert(int stm1, string DE1, int stm2, string DE2){
	parentTable.insert(stm1, DE1, stm2, DE2);
}
int PKB::getParent (int stm){
	return parentTable.getParent(stm);
}
vector<int> PKB::getParentT(int stmt){
	return parentTable.getParentT(stmt);
}
vector<int> PKB::getChildren(int stm, string DE){
	return parentTable.getChildren(stm, DE);
}
vector<int> PKB::getChildrenT(int stm, string DE){
	return parentTable.getChildrenT(stm, DE);
}
void PKB::printParentTable(){
	parentTable.print();
}

//FollowTable
void PKB::insertFollow(int stm1, string DE1, int stm2, string DE2){
	followTable.insertFollow(stm1, DE1, stm2, DE2);
}
int PKB::findFollowed(int stm){
	return followTable.findFollowed(stm);
}
vector<int> PKB::findFollowedT(int stmt, string DE){
	return followTable.findFollowedT(stmt, DE);
}
int PKB::findFollows(int stm){
	return followTable.findFollows(stm);
}
vector<int> PKB::findFollowsT(int stmt, string DE){
	return followTable.findFollowsT(stmt, DE);
}
bool PKB::isFollowed(int stm1,int stm2){
	return followTable.isFollowed(stm1,stm2);
}
void PKB::printFollowTable(){
	followTable.print();
}

//ModifyTable
int PKB::insertModifyStmt(int stmtNo, int varIndex, string DE){
	return modifyTable.insertModifyStmt(stmtNo, varIndex, DE);
}
int PKB::insertModifyProc(int procIndex, int varIndex){
	return modifyTable.insertModifyProc(procIndex, varIndex);
}
vector<int> PKB::getModifiedStmt(int stmtNo){
	return modifyTable.getModifiedStmt(stmtNo);
}
vector<int> PKB::getModifiedProc(int procIndex){
	return modifyTable.getModifiedProc(procIndex);
}
vector<int> PKB::getModifiedList(int varIndex, string DE){
	return modifyTable.getModifiedList(varIndex, DE);
}
void PKB::printModifyTable()
{
	modifyTable.printModifyTable();
}

//UseTable
int PKB::insertUseStmt(int stmtNo, int varIndex, string DE){
	return useTable.insertUseStmt(stmtNo, varIndex, DE);
}
int PKB::insertUseProc(int procIndex, int varIndex){
	return useTable.insertUseProc(procIndex, varIndex);
}
vector<int> PKB::getUsedStmt(int stmtNo){
	return useTable.getUsedStmt(stmtNo);
}
vector<int> PKB::getUsedProc(int procIndex){
	return useTable.getUsedProc(procIndex);
}
vector<int> PKB::getUsedList(int varIndex, string DE){
	return useTable.getUsedList(varIndex, DE);
}
void PKB::printUseTable()
{
	useTable.printUseTable();
}

//VarTable
int PKB::insertVar(string varName){
	return varTable.insertVar(varName);
}
string PKB::getVarName(int index){
	return varTable.getVarName(index);
}
int PKB::getVarIndex(string varName){
	return varTable.getVarIndex(varName);
}
int PKB::getSizeVarTable(){
	return varTable.getSize();
}
void PKB::printVarTable()
{
	varTable.printVarTable();
}

//ProcTable
int PKB::insertProc(string procName){
	return procTable.insertProc(procName);
}
string PKB::getProcName(int index){
	return procTable.getProcName(index);
}
int PKB::getProcIndex(string procName){
	return procTable.getProcIndex(procName);
}
int PKB::getSizeProcTable(){
	return procTable.getSize();
}
void PKB::printProcTable()
{
	procTable.printProcTable();
}

//StmtTable
int PKB::insertStmt(int stmtNo, string type)
{
	return stmtTable.insertStmt(stmtNo, type);
}
vector<int> PKB::getStmtNo(string type)
{
	return stmtTable.getStmtNo(type);
}
int PKB::getSizeStmtTable()
{
	return stmtTable.getSize();
}
string PKB::getStmtType(int stmtNo)
{
	return stmtTable.getStmtType(stmtNo);
}
void PKB::printStmtTable()
{
	stmtTable.printStmtTable();
}

// ConstantTable
void PKB::insertConst(int stmtNo, int number)
{
	constTable.insertConst(stmtNo, number);
}

vector<int> PKB::getConst(int stmtNo)
{
	return constTable.getConst(stmtNo);
}

vector<int> PKB::getStmtForConst(int number)
{
	return constTable.getStmt(number);
}

vector<int> PKB::getAllStmtForConst()
{
	return constTable.getAllStmt();
}

void PKB::printConstTable()
{
	constTable.printConstTable();
}

/*
void makeLeft(TNode left);
void makeRight(TNode right);
void makeChild(TNode child);
TNode getParent(TNode child);
vector<TNode> getChildren(TNode parent);
TNode getLeftSibling(TNode node);
TNode getRightSibling(TNode node);
*/
