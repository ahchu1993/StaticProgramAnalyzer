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
void Results::initTable(pair<string, string> refs,vector<pair<string,string>> results){
    columns.insert(refs.first);
    columns.insert(refs.second);//initiate column
    for (int i=0; i<results.size(); i++) {
        list<string> tuple;
        string first;
        string second;
        first = results.at(i).first;
        second =results.at(i).second;
        tuple.push_back(first);
        tuple.push_back(second);
        tuple_list.push_back(tuple);
    }
    
}
list<list<string>> Results::validation(int parent_index, int child_index, vector<pair<string,string>> results){
    list<list<string>> output;
    for (list<list<string>>::iterator g = tuple_list.begin(); g != tuple_list.end(); g++) {
        //find for each parent child pair in the table with the same synonmy from results
        list<string> tuple = *g;
        list<string>::iterator pit = next(tuple.begin(),parent_index);
        string parent_cell= *pit;
        list<string>::iterator cit = next(tuple.begin(),child_index);
        string child_cell = *cit;
        //compare the parent child pair with values from results
        for(int i =0; i<results.size();i++){
            if(parent_cell.compare(results.at(i).first)==0 && child_cell.compare(results.at(i).second)==0){
                string newCell1;
                string newCell2;
                newCell1 = parent_cell;
                newCell2 = child_cell;
                list<string> newlist;
                newlist.push_back(newCell1);//add the additional tuple with original tuples in a temp list
                newlist.push_back(newCell2);
                output.push_back(newlist);//save the tuple in a table
            }
        }
    }//for
    return output;
}
//use nest loop join algorithm
list<list<string>> Results::equiJoin(int join_index, int position,vector<pair<string,string>> results){
    list<list<string>> output;
    for (list<list<string>>::iterator g = tuple_list.begin(); g != tuple_list.end(); g++) {
        //find for each parent child pair in the table with the same synonmy from results
        list<string> tuple = *g;//get each tuple
        list<string>::iterator j = next(tuple.begin(),join_index);
        //bool flag_find = false;
        string join_cell= *j;
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
            if(join_cell.compare(compare)==0){
                string newCell;
                newCell = add;
                list<string> newlist = tuple;//save original tuples in a temp list
                newlist.push_back(newCell);//add the additional tuple with original tuples in a temp list
                output.push_back(newlist);//save the tuple in a table
            }//if can join
        }//for each result
    }//for each tuple in tuple_set
    return output;
}
void Results::merge(Results table){
    //merge columns
    list<list<string>> output;
    if (tuple_list.size()==0) {
        tuple_list = table.tuple_list;
        columns = table.columns;
    }
    else{
        for (set<string>::iterator g = table.columns.begin(); g!= table.columns.end(); g++) {
            string element = *g;
            columns.insert(element);
        }
        //merge tuples
        for (list<list<string>>::iterator t = tuple_list.begin(); t!= tuple_list.end(); t++) {
            list<string> tuple = *t;//get each tuple from main
            for (list<list<string>>::iterator o = table.tuple_list.begin(); o!= table.tuple_list.end(); o++) {
                list<string> o_tuple = *o;//get each tuple from other table
                list<string> newlist = tuple;//save original tuples in a temp list
                for (list<string>::iterator m=o_tuple.begin();m!=o_tuple.end(); m++) {//for each cell in other table
                    string merge_cell= *m;
                    string newCell;
                    newCell = merge_cell;
                    newlist.push_back(newCell);//add all the cells with original tuples in a temp list
                }
                output.push_back(newlist);//save the temp list in a output table
            }//for other table
        }//for main
        tuple_list = output;//save output table into original table
    }
}
void Results::join(pair<string, string> refs, vector<pair<string,string>> results){
    bool flag_parent=false;//use flag to check whether parent or child column exits or not
    bool flag_child=false;
    if (columns.size()==0){
        initTable(refs, results);
    }
    else {
        int column_parent =findColumn(refs.first);
        int column_child =findColumn(refs.second);
        if (column_parent!=-1) {
            flag_parent=true;
        }
        if (column_child!=-1) {
            flag_child=true;
        }
        if (flag_parent&&flag_child) {
            tuple_list = validation(column_parent, column_child, results);
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
        if (tuple_list.size()==0) {
            set<string> newset;
            columns=newset;
        }
    }
}
string makeKey(list<string>input){
    string output;
    for (list<string>::iterator g = input.begin(); g != input.end(); g++) {
        string temp = *g;
        output+=temp;
    }
    return output;
}
void Results::eliminateColumns(vector<string>refs){
    vector<int> ref_index;
    set<string> newColumns;
    list<list<string>> newtuple_list;
    for (int a=0; a<refs.size(); a++) {
        int index = findColumn(refs.at(a));
        if (index!=-1) {
            ref_index.push_back(index);
        }
    }
    map<string,bool> checkmap;
    for (list<list<string>>::iterator g = tuple_list.begin(); g != tuple_list.end(); g++) {
        list<string> tuple = *g;
        list<string> newtuple;
        for (int b=0; b<ref_index.size(); b++) {
            int index = ref_index.at(b);
            list<string>::iterator pit = next(tuple.begin(),index);
            string cell= *pit;
            newtuple.push_back(cell);
        }//for one tulples
        string key = makeKey(newtuple);
        if (!checkmap[key]) {
            checkmap[key]=true;
            newtuple_list.push_back(newtuple);
        }
    }//for all the tuples
    for (int c=0; c<ref_index.size(); c++) {
        set<string>::iterator pits = next(columns.begin(),ref_index.at(c));
        string temp = *pits;
        newColumns.insert(temp);
    }
    tuple_list = newtuple_list;
    columns = newColumns;
}


