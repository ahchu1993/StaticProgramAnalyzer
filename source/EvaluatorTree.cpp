//
//  EvaluatorTree.cpp
//  cs3202
//
//  Created by Zhao Weixiang on 21/9/13.
//  Copyright (c) 2013 Zhao Weixiang. All rights reserved.
//

#include "EvaluatorTree.h"
EvaluatorTree::EvaluatorTree(){
	//constuctor
	
}

//EvaluatorTree::e_node createNode(string ref, e_node* front, e_node* back, int value, int weights){
EvaluatorTree::e_node createNode(string ref,int value, int weights){
    EvaluatorTree::e_node node;
    node.ref = ref;
   // node.before = front;
   // node.next = back;
    node.value = value;
    node.weights = weights;
    return node;
}

int findColumn(string ref){
    for(int i = 0; i < EvaluatorTree::columns.size(); i++){
        if(EvaluatorTree::columns.at(i)->ref.compare(ref)==0)
            return i;
    }
    return -1;
}

void findNode(vector<Pair<EvaluatorTree::e_node*, int>>& results, EvaluatorTree::e_node node){
    int i = findColumn(node.ref);
    if(i!=-1){
        vector<EvaluatorTree::e_node*> temp = EvaluatorTree::columns.at(i)->links;
        for(int j =0; j<temp.size();j++){
            if(temp.at(j)->value == node.value){
                Pair<EvaluatorTree::e_node*, int> pair = Pair<EvaluatorTree::e_node*, int>::Pair(temp.at(j), j);
                results.push_back(pair);
            }
                                            
        }
    }
}
void merge(EvaluatorTree::e_node parent, EvaluatorTree::e_node child, bool flag){
    vector<Pair<EvaluatorTree::e_node*, int>> parent_sets;
    int index_parent =findColumn(parent.ref);
    int index_child =findColumn(child.ref);
    if(!flag){//the both columns are not empty, do elimination
        EvaluatorTree::Columns* firstColumn = EvaluatorTree::columns.at(0);
        EvaluatorTree::findNode(parent_sets, parent);
        if(parent_sets.size()==0){//if parent is not in the tree
            //do nothing
        }//if
        else{//else found parent
             vector<EvaluatorTree::e_node*> childrenColumn = EvaluatorTree::columns.at(index_child)->links;
            for(int i=0;i<parent_sets.size();i++){
                int parentRow = parent_sets.at(i).getSecond();
                if (childrenColumn.at(parentRow)->value == child.value){
                    EvaluatorTree::e_node* rowHeader = firstColumn->links.at(parentRow);
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
            if(EvaluatorTree::columns.size()==0){//if result table is empty
                EvaluatorTree::Columns parentColumn;
                parentColumn.ref = parent.ref;
                parentColumn.links.push_back(&parent);
                EvaluatorTree::columns.push_back(&parentColumn);
                EvaluatorTree::Columns childrenColumn;
                childrenColumn.ref = child.ref;
                childrenColumn.links.push_back(&child);
                EvaluatorTree::columns.push_back(&childrenColumn);
            }//if first Colunm is empty
            else{//if result table is not empty
                if (index_child==-1) {//if child is also not in the table
                    duplicateAndAdd(parent,child)
                }
            
            }
        }//if parent is not in the result column
    
    }//else one or two column is empty
}


/*EvaluatorTree::e_node findNodeRef(vector<EvaluatorTree::e_node*>nodes, EvaluatorTree::e_node node){
    if(nodes.size()==0){
        EvaluatorTree::e_node null;
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
void findNode(vector<EvaluatorTree::e_node*>& results, vector<EvaluatorTree::e_node*>nodes, EvaluatorTree::e_node node){
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


static void duplicateAndAdd(EvaluatorTree::e_node parent,EvaluatorTree::e_node child){
    
    for(int i =0; i<EvaluatorTree::columns.size();i++){
        for (int j = 0; j<EvaluatorTree::columns.at(i)->links.size(); j++) {
            //todo
        }//for j
    
    }//for i

}
