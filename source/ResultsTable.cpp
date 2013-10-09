//
//  ResultsTable.cpp
//  cs3202
//
//  Created by Zhao Weixiang on 9/10/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//

#include "ResultsTable.h"

ResultsTable::ResultsTable(){
    //constructor
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
vector<vector<string>> ResultsTable::validation(int parent_index, int child_index, vector<pair<string,string>> results){
    vector<vector<string>> output;
    for (int i =0; i<tuples.size(); i++) {
        //find for each parent child pair in the table with the same synonmy from results
        vector<string> tuple = tuples.at(i);
        string parent_cell= tuple.at(parent_index);
        string child_cell = tuple.at(child_index);
        //compare the parent child pair with values from results
        for(int i =0; i<results.size();i++){
            if(parent_cell.compare(results.at(i).first)==0 && child_cell.compare(results.at(i).second)==0){
                string newCell1;
                string newCell2;
                newCell1 = parent_cell;
                newCell2 = child_cell;
                vector<string> newvector;
                newvector.push_back(newCell1);//add the additional tuple with original tuples in a temp vector
                newvector.push_back(newCell2);
                output.push_back(newvector);//save the tuple in a table
            }
        }
    }//for
    return output;
}
//use nest loop join algorithm
vector<vector<string>> ResultsTable::equiJoin(int join_index, int position,vector<pair<string,string>> results){
    vector<vector<string>> output;
    for (int i =0; i<tuples.size(); i++) {
        //find for each parent child pair in the table with the same synonmy from results
        //bool flag_find = false;
        vector<string> tuple = tuples.at(i);//get each tuple
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
                output.push_back(newvector);//save the tuple in a table
            }//if can join
        }//for each result
    }//for each tuple in tuple_set
    return output;
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
        for (int t =0; t<tuples.size(); t++) {
            vector<string> tuple = tuples.at(t);//get each tuple from main
            for (int o =0; o<table.tuples.size(); o++) {
                vector<string> o_tuple = table.tuples.at(o);//get each tuple from other table
                vector<string> newvector = tuple;//save original tuples in a temp vector
                for (int m=0; m<o_tuple.size(); m++) {//for each cell in other table
                    
                    
                    string merge_cell= o_tuple.at(m);
                    string newCell;
                    newCell = merge_cell;
                    newvector.push_back(newCell);//add all the cells with original tuples in a temp vector
                }
                output.push_back(newvector);//save the temp vector in a output table
            }//for other table
        }//for main
        tuples = output;//save output table into original table
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
            tuples = validation(column_parent, column_child, results);
        }
        else{// parent or child is not in the result table
            if (flag_parent) {//parent is in the result table
                columns.push_back(refs.second);
                tuples = equiJoin(column_parent, 1, results);
            }
            else if(flag_child){
                columns.push_back(refs.first);
                tuples = equiJoin(column_child, 2, results);
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
void ResultsTable::eliminateColumns(vector<string>refs){
    vector<int> ref_index;
    vector<string> newColumns;
    vector<vector<string>> newtuples;
    for (int a=0; a<refs.size(); a++) {
        int index = findColumn(refs.at(a));
        if (index!=-1) {
            ref_index.push_back(index);
        }
    }
    map<string,bool> checkmap;
    for (int i =0; i<tuples.size(); i++) {
        vector<string> tuple = tuples.at(i);
        vector<string> newtuple;
        for (int b=0; b<ref_index.size(); b++) {
            int index = ref_index.at(b);
            string cell= tuple.at(index);
            newtuple.push_back(cell);
        }//for one tulples
        string key = makeKey(newtuple);
        if (!checkmap[key]) {
            checkmap[key]=true;
            newtuples.push_back(newtuple);
        }
    }//for all the tuples
    for (int c=0; c<ref_index.size(); c++) {
        
        string temp = columns.at(ref_index.at(c));
        newColumns.push_back(temp);
    }
    tuples = newtuples;
    columns = newColumns;
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
list<string> ResultsTable::toList(){
    //list<list<string>> lists;
    list<string> list;
    for (int i =0; i<tuples.size(); i++) {
        string temp;
        vector<string> tuple= tuples.at(i);
        for (int j=0; j<tuple.size(); j++) {
            temp = tuple.at(j) + " ";
        }
        if (i!=tuples.size()-1) {
            temp +=",";
        }
        list.push_back(temp);
    }
    return list;
}
void ResultsTable::printResults(){
    print_vector(columns);
    print_vectors(tuples);

}

