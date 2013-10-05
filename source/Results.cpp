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
/*
Results::cell createNode(string ref,int value, int weights){
    Results::cell node;
    node.ref = ref;
    // node.before = front;
    // node.next = back;
    node.value = value;
    node.weights = weights;
    return node;
}
 */
/*
 
 for (set<int>::iterator g = pattern_result.cell.begin(); g != pattern_result.cell.end(); g++) {
 int element = *g;
 
 */
/*
 Pair<Results::Columns*,int> findColumn(string ref){
 int counter = 0;
 for (set<Results::Columns*>::iterator g = Results::columns.begin(); g != Results::columns.end(); g++) {
 Results::Columns* element = *g;
 if(element->ref.compare(ref)==0)
 return Pair<Results::Columns*,int>(element,counter);
 }
 return Pair<Results::Columns*,int>(NULL,counter);
 }
 */
int Results::findColumn(string ref){
    int counter = 0;
    for (set<string>::iterator g = columns.begin(); g != columns.end(); g++) {
        string element = *g;
        if(element.compare(ref)==0)
            return counter;
        counter++;
    }
    return -1;
}
/*
 void Results::findNode(vector<Results::cell*>& results, Results::cell node){
 int index = this.findColumn(node.ref);
 if(index!=-1){
 list<list<cell*>*>::iterator it=tuple_list.begin();
 list<Results::cell*> temp = next(it,index);
 for (list<list<cell*>*>::iterator g = temp.begin(); g != temp.end(); g++) {
 Results::cell* element = *g;
 if (element->value.compare(node.value)==0) {
 results.push_back(element);
 }
 }//for
 }//if !NULL
 }
 */
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
 int parentRow = parent_sets.at(i).second;
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
/*
 void Results::validation(Pair<Results::Columns*,int> column_parent, Pair<Results::Columns*,int> column_child, vector<Pair<string,string>> results){
 if (column_parent.second<column_child.second) {//parent child
 Results::Columns* firstColumn = columns.begin();
 
 }
 }
 */
void Results::initTable(pair<string, string> refs,vector<pair<string,string>> results){
    columns.insert(refs.first);
    columns.insert(refs.second);//initiate column
    for (int i=0; i<results.size(); i++) {
        list<cell*> tuple;
        Results::cell first;
        Results::cell second;
        first.value = results.at(i).first;
        second.value =results.at(i).second;
        tuple.push_back(&first);
        tuple.push_back(&second);
        tuple_list.push_back(&tuple);
    }

}
void Results::validation(int parent_index, int child_index, vector<pair<string,string>> results){
    for (list<list<cell*>*>::iterator g = tuple_list.begin(); g != tuple_list.end(); g++) {
        //find for each parent child pair in the table with the same synonmy from results
        bool flag_find = false;
        list<cell*>* tuple = *g;
        list<cell*>::iterator pit = next(tuple->begin(),parent_index);
        cell* parent_cell= *pit;
        list<cell*>::iterator cit = next(tuple->begin(),child_index);
        cell* child_cell = *cit;
        //compare the parent child pair with values from results
        for(int i =0; i<results.size();i++){
            if(parent_cell->value.compare(results.at(i).first)==0 && child_cell->value.compare(results.at(i).second)==0){
                flag_find = true;
                break;
            }
        }
        if (!flag_find) {//if not match at all, delete this tuple from list
            tuple_list.erase(g);
        }
    }
}
//use nest loop join algorithm
list<list<Results::cell*>*> Results::equiJoin(int join_index, int position,vector<pair<string,string>> results){
    list<list<Results::cell*>*> output;
    for (list<list<cell*>*>::iterator g = tuple_list.begin(); g != tuple_list.end(); g++) {
        //find for each parent child pair in the table with the same synonmy from results
        list<cell*>* tuple = *g;//get each tuple
        list<cell*>::iterator j = next(tuple->begin(),join_index);
        //bool flag_find = false;
        cell* join_cell= *j;
        for(int i =0; i<results.size();i++){
            string compare;
            string add;
            if (position==1) {
                compare = results.at(i).first;
                add = results.at(i).second;
            }
            else if (position==2){
                compare = results.at(i).second;
                add = results.at(i).first;
            }
            if(join_cell->value.compare(compare)==0){
                Results::cell newCell;
                newCell.value = add;
                list<cell*> newlist = *tuple;//save original tuples in a temp list
                newlist.push_back(&newCell);//add the additional tuple with original tuples in a temp list
                output.push_back(&newlist);//save the tuple in a table
            }//if can join
        }//for each result
    }//for each tuple in tuple_set
    return output;
}
void Results::merge(Results table){
    //merge columns
    list<list<Results::cell*>*> output;
    for (set<string>::iterator g = table.columns.begin(); g!= table.columns.end(); g++) {
        string element = *g;
        columns.insert(element);
    }
    //merge tuples
    for (list<list<cell*>*>::iterator t = tuple_list.begin(); t!= tuple_list.end(); t++) {
        list<cell*>* tuple = *t;//get each tuple from main
        for (list<list<cell*>*>::iterator o = table.tuple_list.begin(); o!= table.tuple_list.end(); o++) {
            list<cell*>* o_tuple = *o;//get each tuple from other table
            list<cell*> newlist = *tuple;//save original tuples in a temp list
            for (list<cell*>::iterator m=o_tuple->begin();m!=o_tuple->end(); m++) {//for each cell in other table
                cell* merge_cell= *m;
                Results::cell newCell;
                newCell.value = merge_cell->value;
                newlist.push_back(&newCell);//add all the cells with original tuples in a temp list
            }
            output.push_back(&newlist);//save the temp list in a output table
        }//for other table
    }//for main
    tuple_list = output;//save output table into original table
}
void Results::join(pair<string, string> refs, vector<pair<string,string>> results){
    bool flag_parent=false;//use flag to check whether parent or child column exits or not
    bool flag_child=false;
    if (columns.size()==0){
        initTable(refs, results);
    }
    int column_parent =findColumn(refs.first);
    int column_child =findColumn(refs.second);
    if (column_parent!=-1) {
        flag_parent=true;
    }
    if (column_child!=-1) {
        flag_child=true;
    }
    if (flag_parent&&flag_child) {
        validation(column_parent, column_child, results);
    }
    else{// parent or child is not in the result table
        if (flag_parent) {//parent is in the result table
            columns.insert(refs.second);
            tuple_list = equiJoin(column_parent, 1, results);
        }
        else if(flag_child){
            columns.insert(refs.first);
            tuple_list = equiJoin(column_child, 2, results);
        }
    }//else parent or child is not in the result table
}


