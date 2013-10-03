//
//  Results.cpp
//  cs3202
//
//  Created by Zhao Weixiang on 21/9/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//

#include "Results.h"
Results::Results(){
	//constuctor
	
}

//Results::cell createNode(string ref, cell* front, cell* back, int value, int weights){
Results::cell createNode(string ref,int value, int weights){
    Results::cell node;
    node.ref = ref;
   // node.before = front;
   // node.next = back;
    node.value = value;
    node.weights = weights;
    return node;
}
/*
 
 for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
 int element = *g;
 
 */
Pair<Results::Columns*,int> findColumn(string ref){
    int counter = 0;
    for (set<Results::Columns*>::iterator g = Results::columns.begin(); g != Results::columns.end(); g++) {
        Results::Columns* element = *g;
        if(element->ref.compare(ref)==0)
            return Pair<Results::Columns*,int>(element,counter);
    }
    return Pair<Results::Columns*,int>(NULL,counter);
}

void findNode(vector<Results::cell*>& results, Results::cell node){
    Results::Columns* i = findColumn(node.ref).getFirst();
    if(i!=NULL){
        list<Results::cell*> temp = i->links;
        for (list<Results::cell*>::iterator g = temp.begin(); g != temp.end(); g++) {
            Results::cell* element = *g;
            if (element->value.compare(node.value)==0) {
                results.push_back(element);
            }
        }//for
    }//if !NULL
}
/*
void merge(Results::cell parent, Results::cell child, bool flag){
    vector<Pair<Results::cell*, int>> parent_sets;
    Pair<Results::Columns*,int> column_parent =findColumn(parent.ref);
    Pair<Results::Columns*,int> column_child =findColumn(child.ref);
    if(!flag){//the both columns are not empty, do elimination
        Results::Columns* firstColumn = *Results::columns.begin();
        Results::findNode(parent_sets, parent);
        if(parent_sets.size()==0){//if parent is not in the tree
            //do nothing
        }//if
        else{//else found parent
             vector<Results::cell*> childrenColumn = Results::columns.at(index_child)->links;
            for(int i=0;i<parent_sets.size();i++){
                int parentRow = parent_sets.at(i).getSecond();
                if (childrenColumn.at(parentRow)->value == child.value){
                    Results::cell* rowHeader = firstColumn->links.at(parentRow);
                     rowHeader->weights++;
                }//if
            }//for
            //
             should do deletion
             will done in the future
             //
        }//else found parent
    }//if column is not empty
    else{//else one or two column is empty
        if(index_parent==-1){//if parent is not in the result column
            if(Results::columns.size()==0){//if result table is empty
                Results::Columns parentColumn;
                parentColumn.ref = parent.ref;
                parentColumn.links.push_back(&parent);
                Results::columns.push_back(&parentColumn);
                Results::Columns childrenColumn;
                childrenColumn.ref = child.ref;
                childrenColumn.links.push_back(&child);
                Results::columns.push_back(&childrenColumn);
            }//if first Colunm is empty
            else{//if result table is not empty
                if (index_child==-1) {//if child is also not in the table
                    duplicateAndAdd(parent,child)
                }
            
            }
        }//if parent is not in the result column
    
    }//else one or two column is empty
}
*/
void validation(Pair<Results::Columns*,int> column_parent, Pair<Results::Columns*,int> column_child, vector<Pair<string,string>> results){
    if (column_parent.getSecond()<column_child.getSecond()) {//parent child
        Results::Columns* firstColumn = *Results::columns.begin();
        
    }
}
void merge(Pair<string, string> refs, vector<Pair<string,string>> results){
    bool flag_parent=false;//use flag to check whether parent or child column exits or not
    bool flag_child=false;
    Pair<Results::Columns*,int> column_parent =findColumn(refs.getFirst());
    Pair<Results::Columns*,int> column_child =findColumn(refs.getSecond());
    if (column_parent.getFirst()==NULL) {
        flag_parent=true;
    }
    if (column_child.getFirst()==NULL) {
        flag_child=true;
    }
    if (flag_parent&&flag_child) {
        validation(column_parent, column_child, results);
    }
    else{// parent or child is not in the result table
        if (flag_parent) {
            naturalJoin();
        }
        else if(flag_child){
            naturalJoin();
        }
        else{
            joinTables();
        }
    }//else parent or child is not in the result table
}


static void duplicateAndAdd(Results::cell parent,Results::cell child){
    
    for(int i =0; i<Results::columns.size();i++){
        for (int j = 0; j<Results::columns.at(i)->links.size(); j++) {
            //todo
        }//for j
    
    }//for i

}
