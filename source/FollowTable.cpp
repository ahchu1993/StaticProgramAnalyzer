#include "FollowTable.h"
#include <assert.h>
FollowTable::FollowTable(void)
{
} 

void FollowTable::insertFollow(int stm1, string DE1, int stm2, string DE2)
{
	if(stm1 < 0 || stm2 < 0 ) return; //assert
	follow_row newRow;
	newRow.col1 = stm1;
	newRow.de1 = DE1;
	newRow.col2 = stm2;
	newRow.de2 = DE2;
	followTable.push_back(newRow);
}
int FollowTable::findFollows(int stm){
	assert(stm >= 0);

	for (it=followTable.begin(); it<followTable.end(); it++){
		if(((follow_row)*it).col2 == stm){
			return ((follow_row)*it).col1;
		}
	}

	return -1;
}

int FollowTable::findFollowed(int stm){
	assert(stm >= 0);

	for (it=followTable.begin(); it<followTable.end(); it++){
		if(((follow_row)*it).col1 == stm){
			return ((follow_row)*it).col2;
		}
	}

	return -1;
}


bool FollowTable::isFollowed(int stm1,int stm2){
	assert(stm2 >=0 && stm1 >= 0);

	for (it=followTable.begin(); it<followTable.end(); it++){
		if((((follow_row)*it).col1 == stm1) && (((follow_row)*it).col2 == stm2)){
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

	for (it=followTable.begin(); it<followTable.end(); it++){
		cout << ((follow_row)*it).col1 << ((follow_row)*it).de1 <<" : " << ((follow_row)*it).col2 << ((follow_row)*it).de2 << "\n";
	}
}

vector<int> FollowTable::findFollowedT(int stmt, string DE){
	assert (stmt>=0);
	
	vector<int> result;
	int index;

	while((index = this->findFollowed(stmt)) != -1){
		for (it=followTable.begin(); it<followTable.end(); it++){
			if(((follow_row)*it).col2 == index){
				if(((follow_row)*it).de2.compare(DE) == 0 || DE.compare("stmt") == 0){
					result.push_back(((follow_row)*it).col2);	
				}
			}
		}
		stmt = index;
	}

	return result;
}

vector<int> FollowTable::findFollowsT(int stmt, string DE){
	assert(stmt>=0);
	vector<int> result;
	int index;

	while((index = this->findFollows(stmt)) != -1){
		for (it=followTable.begin(); it<followTable.end(); it++){
			if(((follow_row)*it).col1 == index){
				if(((follow_row)*it).de1.compare(DE) == 0 || DE.compare("stmt")==0){
					result.push_back(((follow_row)*it).col1);	
				}
			}
		}
		stmt = index;
	}

	return result;
}
