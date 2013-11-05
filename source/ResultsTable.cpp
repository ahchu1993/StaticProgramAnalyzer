//
//  ResultsTable.cpp
//  cs3202
//
//  Created by Zhao Weixiang on 9/10/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//
/*
 create a map to store the relation result
 use a list to implement table
 add deletion
 
 
 */
#include "ResultsTable.h"
ResultsTable::ResultsTable(){
    
};
ResultsTable::ResultsTable(string ref, set<string> value){
    //constructor
    this->columns.push_back(ref);
    
    for (set<string>::iterator g = value.begin(); g != value.end(); g++) {
        vector<string> temp;
        temp.push_back(*g);
        this->tuples.push_back(temp);
    }
    
}
int ResultsTable::findColumn(string ref){
    int counter = 0;
    for (int i =0; i<columns.size(); i++) {
        string element = columns.at(i);
        if(element.compare(ref)==0)
            return counter;
        counter++;
    }
    return -1;
}
void ResultsTable::initTable(pair<string, string> refs,vector<pair<string,string>> results){
    columns.push_back(refs.first);
    columns.push_back(refs.second);//initiate column
    for (int i=0; i<results.size(); i++) {
        vector<string> tuple;
        string first;
        string second;
        first = results.at(i).first;
        second =results.at(i).second;
        tuple.push_back(first);
        tuple.push_back(second);
        tuples.push_back(tuple);
    }
    
}
void ResultsTable::validation(int parent_index, int child_index, vector<pair<string,string>> results){
    vector<vector<string>> output;
    long list_size = tuples.size();
    for (int i =0; i<list_size; i++) {
        //find for each parent child pair in the table with the same synonmy from results
        vector<string> tuple = tuples.front();
        tuples.pop_front();
        
        string parent_cell= tuple.at(parent_index);
        string child_cell = tuple.at(child_index);
        //compare the parent child pair with values from results
        for(int i =0; i<results.size();i++){
            if(parent_cell.compare(results.at(i).first)==0 && child_cell.compare(results.at(i).second)==0){
                
                vector<string> newvector;
				newvector = tuple;
                tuples.push_back(newvector);//save the tuple in a table
            }
        }
    }//for
}
//use nest loop join algorithm
void ResultsTable::equiJoin(int join_index, int position,vector<pair<string,string>> results){
    //vector<vector<string>> output;
    long list_size = tuples.size();
    for (int i =0; i<list_size; i++) {
        //find for each parent child pair in the table with the same synonmy from results
        //bool flag_find = false;
        vector<string> tuple = tuples.front();
        tuples.pop_front();
        string join_cell= tuple.at(join_index);
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
                vector<string> newvector = tuple;//save original tuples in a temp vector
                newvector.push_back(newCell);//add the additional tuple with original tuples in a temp vector
                tuples.push_back(newvector);//save the tuple in a table
            }//if can join
        }//for each result
    }//for each tuple in tuple_set
}
void ResultsTable::merge(ResultsTable table){
    //merge columns
    vector<vector<string>> output;
    if (tuples.size()==0) {
        tuples = table.tuples;
        columns = table.columns;
    }
    else{
        for (int i =0; i<table.columns.size(); i++) {
            columns.push_back(table.columns.at(i));
        }
        //merge tuples
        long list_size = tuples.size();
        for (int t =0; t<list_size; t++) {
              
           
            for (list<vector<string>>::iterator g = table.tuples.begin(); g != table.tuples.end(); g++) {
                //find for each parent child pair in the table with the same synonmy from results
                vector<string> o_tuple = *g;//get each tuple from other table
                vector<string> newvector = tuples.front();//save original tuples in a temp vector
                for (int m=0; m<o_tuple.size(); m++) {//for each cell in other table
                
                    newvector.push_back(o_tuple.at(m));//add all the cells with original tuples in a temp vector
                }
                tuples.push_back(newvector);//save the temp vector in a output table
            }//for other table
			 tuples.pop_front();
        }//for main
    }
}

void ResultsTable::join(pair<string, string> refs, vector<pair<string,string>> results){
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
            validation(column_parent, column_child, results);
        }
        else{// parent or child is not in the result table
            if (flag_parent) {//parent is in the result table
                columns.push_back(refs.second);
                equiJoin(column_parent, 1, results);
            }
            else if(flag_child){
                columns.push_back(refs.first);
                equiJoin(column_child, 2, results);
            }
        }//else parent or child is not in the result table
        if (tuples.size()==0) {
            vector<string> newset;
            columns=newset;
        }
    }
}
string makeKey(vector<string>input){
    string output;
    for (int g =0; g<input.size(); g++) {
        string temp = input.at(g)+" ";
        output+=temp;
    }
    return output;
}
bool findRef(vector<string>refs, string c){
    for (int i =0; i<refs.size(); i++) {
        string element = refs.at(i);
        if(element.compare(c)==0)
            return true;
    }
    return false;
}
void ResultsTable::eliminateColumns(vector<string>refs){
    vector<int> ref_index;
    vector<string> newColumns;
    bool do_eliminate = false;
    //vector<vector<string>> newtuples;
    if (refs.at(0)!="BOOLEAN") {
        
        for (int d=0; d<columns.size(); d++) {
            if(findRef(refs,columns.at(d))==false){//if columns have more ref than refs, do elimination
                do_eliminate = true;
                break;
            }
        }
        if (do_eliminate) {
            for (int a=0; a<refs.size(); a++) {
                int index = findColumn(refs.at(a));
                if (index!=-1) {
                    ref_index.push_back(index);
                }
            }
            map<string,bool> checkmap;
            long list_size = tuples.size();
            for (int t =0; t<list_size; t++) {
                vector<string> tuple = tuples.front();//get each tuple from main
                tuples.pop_front();
                vector<string> newtuple;
                for (int b=0; b<ref_index.size(); b++) {
                    int index = ref_index.at(b);
                    string cell= tuple.at(index);
                    newtuple.push_back(cell);
                }//for one tulples
                string key = makeKey(newtuple);
                if (!checkmap[key]) {
                    checkmap[key]=true;
                    tuples.push_back(newtuple);
                }
            }//for all the tuples
            for (int c=0; c<ref_index.size(); c++) {
                
                string temp = columns.at(ref_index.at(c));
                newColumns.push_back(temp);
            }
            //tuples = newtuples;
            columns = newColumns;
        }
    }
}
void print_vector(vector<string> input){
    for (int i =0; i<input.size(); i++) {
        std::cout << input.at(i)<<" ";
    }
    cout<<"\n";
}
void print_vectors(vector<vector<string>> input){
    for (int i =0; i<input.size(); i++) {
        print_vector(input.at(i));
    }
}

list<string> ResultsTable::toList(vector<string> refs){
    list<string> lists;
    vector<int> ref_index;
    for (int a=0; a<refs.size(); a++) {
        int index = findColumn(refs.at(a));
        if (index!=-1) {
            ref_index.push_back(index);
        }
    }
    for (list<vector<string>>::iterator g = tuples.begin(); g!=tuples.end(); g++) {
        //find for each parent child pair in the table with the same synonmy from results
        string temp;
        vector<string> tuple = *g;//get each tuple
        for (int j=0; j<ref_index.size(); j++) {
            temp += tuple.at(ref_index.at(j));
            if (j!=ref_index.size()-1) {
                temp+=" ";
            }
        }
        
        lists.push_back(temp);
    }
    return lists;
}
