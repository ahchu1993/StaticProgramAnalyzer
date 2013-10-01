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

int findColumn(string ref){
    for(int i = 0; i < Results::columns.size(); i++){
        if(Results::columns.at(i)->ref.compare(ref)==0)
            return i;
    }
    return -1;
}

void findNode(vector<Pair<Results::cell*, int>>& results, Results::cell node){
    int i = findColumn(node.ref);
    if(i!=-1){
        vector<Results::cell*> temp = Results::columns.at(i)->links;
        for(int j =0; j<temp.size();j++){
            if(temp.at(j)->value == node.value){
                Pair<Results::cell*, int> pair = Pair<Results::cell*, int>::Pair(temp.at(j), j);
                results.push_back(pair);
            }
                                            
        }
    }
}
void merge(Results::cell parent, Results::cell child, bool flag){
    vector<Pair<Results::cell*, int>> parent_sets;
    int index_parent =findColumn(parent.ref);
    int index_child =findColumn(child.ref);
    if(!flag){//the both columns are not empty, do elimination
        Results::Columns* firstColumn = Results::columns.at(0);
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
            /*
             should do deletion
             will done in the future
             */
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


/*Results::cell findNodeRef(vector<Results::cell*>nodes, Results::cell node){
    if(nodes.size()==0){
        Results::cell null;
        null.ref = "";
 return null;
    }
    for (int i=0;i<nodes.size();i++)
    {
        if (nodes.at(i)->ref == node.ref)
            return *nodes.at(i);

        else return findNodeRef(nodes.at(i)->links, node);
    }
}
void findNode(vector<Results::cell*>& results, vector<Results::cell*>nodes, Results::cell node){
    unsigned long linksize = nodes.size();
    if(linksize>0){
        for (int i=0;i<linksize;i++){
            if (nodes.at(i)->ref == node.ref && nodes.at(i)->value == node.value){
                results.push_back(nodes.at(i));
                break;
            }
            else{
                findNode(results, nodes.at(i)->links, node);
            }
        }
    }
}
*/


static void duplicateAndAdd(Results::cell parent,Results::cell child){
    
    for(int i =0; i<Results::columns.size();i++){
        for (int j = 0; j<Results::columns.at(i)->links.size(); j++) {
            //todo
        }//for j
    
    }//for i

}
