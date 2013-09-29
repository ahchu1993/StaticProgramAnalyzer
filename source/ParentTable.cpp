#include "ParentTable.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
ParentTable::ParentTable() {}

void ParentTable::insert(int stm1, string DE1, int stm2, string DE2){
	assert(stm1>=0 && stm2>=0);
	unsigned i,j;
	vector<int>::iterator ite;
	children_stm child;
	child.stm = stm2;
	child.DE = DE2;

	for(j=0;j<parentTable.size();j++){
		if(parentTable.at(j).parent == stm1){
			for(i=0;i<parentTable.at(j).children.size();i++){
				if(parentTable.at(j).children.at(i).stm == stm2){
					break;
				}
			}
			if(i==parentTable.at(j).children.size()){
				parentTable.at(j).children.push_back(child);
				return;
			}
		}
	}

	parent_row row;
	row.parentDE = DE1;
	row.parent = stm1;
	row.children.push_back(child);
	parentTable.push_back(row);
}
int ParentTable::getParent (int stm){
	assert(stm>=0);
	for(unsigned i=0; i<parentTable.size(); i++){
		for(unsigned j=0; j<parentTable.at(i).children.size(); j++){
			if(parentTable.at(i).children.at(j).stm == stm)
				return parentTable.at(i).parent;
		}
	}
	return -1;
}

vector<int> ParentTable::getParentList (string DE){
	vector<int> result;
	for(unsigned i=0; i<parentTable.size(); i++){
		if(parentTable.at(i).parentDE.compare(DE) == 0 || DE.compare("_") == 0){
			result.push_back(parentTable.at(i).parent);
		}
	}
	return result;
}

	
vector<int> ParentTable::getChildren (int stm, string DE){
	assert(stm>=0);
	vector<int> result;
	vector<children_stm>::iterator ite;

	for(unsigned i=0;i<parentTable.size();i++){
		if(parentTable.at(i).parent == stm){
			for(unsigned j=0;j<parentTable.at(i).children.size();j++){
				if(parentTable.at(i).children.at(j).DE.compare(DE) == 0 || DE.compare("stmt") == 0){
					result.push_back(parentTable.at(i).children.at(j).stm);
				}
			}
			break;
		}
	}

	return result;
}

vector<int> ParentTable::getChildrenList (string DE){
	vector<int> result;
	for(unsigned i=0; i<parentTable.size(); i++){
		for(unsigned j=0;j<parentTable.at(i).children.size();j++){
			if(parentTable.at(i).children.at(j).DE.compare(DE) == 0 || DE.compare("_") == 0){
				result.push_back(parentTable.at(i).children.at(j).stm);
			}
		}
	}
	//remove duplicate
	sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
	return result;
}
vector<int> ParentTable::getParentT(int stmt){
	assert(stmt>=0);
	
	vector<int> result;
	int index;

	while((index = this->getParent(stmt)) != -1){
		result.push_back(index);
		stmt = index;
	}
	return result;
}
vector<int> ParentTable::getChildrenT(int stmt,string DE){
	assert(stmt>=0);
	
	vector<int> result;

	for(unsigned i=0;i<parentTable.size();i++){
		if(parentTable.at(i).parent == stmt){
			for(unsigned j=0;j<parentTable.at(i).children.size();j++){
				if(parentTable.at(i).children.at(j).DE.compare("while") == 0){
					int stmNo = parentTable.at(i).children.at(j).stm;
					result.push_back(stmNo);
					vector<int> children = this->getChildrenT(stmNo,DE);
					
					for(unsigned i=0;i<children.size();i++){
						cout << children.at(i) <<" ";
					}
					cout <<endl;
					result.insert(result.begin(),children.begin(),children.end());

				}else if(parentTable.at(i).children.at(j).DE.compare(DE) == 0 || DE.compare("stmt") == 0) {
					result.push_back(parentTable.at(i).children.at(j).stm);
				}
				
			}
		}

	}
	return result;
}

int ParentTable::getSize(){
	return parentTable.size();
}

void ParentTable::print(){
	unsigned i;
	cout << "Parent Table: Size:" << parentTable.size() << "\n";

	for (it=parentTable.begin(); it<parentTable.end(); it++){
		cout << ((parent_row)*it).parent <<" : ";
		for(i=0;i<((parent_row)*it).children.size();i++){
			cout << ((parent_row)*it).children.at(i).stm << " ";
		}
		cout << "\n";
	}
}
bool ParentTable::isParent(int stm1, int stm2){
	return getParent(stm2) == stm1;
}
vector<Pair<string>> ParentTable::getParentPairList(vector<int> set1, vector<int> set2){
	vector<Pair<string>> result;
	for(unsigned i=0; i<set1.size(); i++){
		for(unsigned j=0; j<set2.size(); j++){
			if(isParent(set1.at(i), set2.at(j))){
				ostringstream convert1;
				convert1 << set1.at(i);
				string no1 = convert1.str();
				ostringstream convert2;
				convert2 << set2.at(j);
				string no2 = convert2.str();
				result.push_back(Pair<string>(no1, no2));
			}
		}
	}

	return result;
}