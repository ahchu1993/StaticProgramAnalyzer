#include "FollowTable.h"
#include <assert.h>
FollowTable::FollowTable(void)
{
} 

void FollowTable::insertFollow(int stm1, string DE1, int stm2, string DE2)
{
	if(stm1 < 0 || stm2 < 0 ) return; 
	follow_row newRow;
	newRow.col1 = stm1;
	newRow.de1 = DE1;
	newRow.col2 = stm2;
	newRow.de2 = DE2;
	followTable.push_back(newRow);
}

vector<int> FollowTable::getFollowsList(string DE){
	vector<int> result;
	for(unsigned i=0; i<followTable.size(); i++){
		if(followTable.at(i).de1.compare(DE) == 0 || DE.compare("_") == 0){
			result.push_back(followTable.at(i).col1);
		}
	}
	return result;
}

vector<int> FollowTable::getFollowedList(string DE){
	vector<int> result;
	for(unsigned i=0; i<followTable.size(); i++){
		if(followTable.at(i).de2.compare(DE) == 0 || DE.compare("_") == 0){
			result.push_back(followTable.at(i).col2);
		}
	}
	return result;
}


bool FollowTable::checkFollow(vector<int> set1, vector<int> set2){
	for(unsigned i=0; i<set1.size(); i++){
		for(unsigned j=0; j<set2.size(); j++){
			if(isFollowed(set1.at(i), set2.at(j))){
				return true;
			}
		}
	}
	return false;
}

int FollowTable::findFollows(int stm){
	assert(stm >= 0);

	for(unsigned i=0; i<followTable.size(); i++){
		if(followTable.at(i).col2 == stm){
			return followTable.at(i).col1;
		}
	}

	return -1;
}

int FollowTable::findFollowed(int stm){
	assert(stm >= 0);

	for(unsigned i=0; i<followTable.size(); i++){
		if(followTable.at(i).col1 == stm){
			return followTable.at(i).col2;
		}
	}

	return -1;
}

bool FollowTable::isFollowed(int stm1,int stm2){
	assert(stm2 >=0 && stm1 >= 0);

	for(unsigned i=0; i<followTable.size(); i++){
		if(followTable.at(i).col1 == stm1 && followTable.at(i).col2 == stm2){
			return true;
		}
	}
	return false;
}

int FollowTable::getSize(){
	return followTable.size();
}

void FollowTable::print(){
	cout << "Follow Table: Size:" << followTable.size() << "\n";

	for(unsigned i=0; i<followTable.size(); i++){
		cout << followTable.at(i).col1 << followTable.at(i).de1 <<" : " << followTable.at(i).col2 << followTable.at(i).de2 << "\n";
	}
}

vector<int> FollowTable::findFollowedT(int stmt){
	assert (stmt>=0);
	
	vector<int> result;
	int index;

	while((index = this->findFollowed(stmt)) != -1){
		for(unsigned i=0; i<followTable.size(); i++){
			if(followTable.at(i).col2 == index){
					result.push_back(followTable.at(i).col2);	
			}
		}
		stmt = index;
	}

	return result;
}

vector<int> FollowTable::findFollowsT(int stmt){
	assert(stmt>=0);
	vector<int> result;
	int index;

	while((index = this->findFollows(stmt)) != -1){
		for(unsigned i=0; i<followTable.size(); i++){
			if(followTable.at(i).col1 == index){
					result.push_back(followTable.at(i).col1);	
			}
		}
		stmt = index;
	}

	return result;
}
