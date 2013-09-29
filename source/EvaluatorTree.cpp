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

EvaluatorTree::e_node createNode(string ref, e_node* front, e_node* back, int value, int weights){
    EvaluatorTree::e_node node;
    node.ref = ref;
    node.before = front;
    node.next = back;
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
int findRow(EvaluatorTree::e_node* node){
    


}
void findNode(vector<EvaluatorTree::e_node*>& results, EvaluatorTree::e_node node){
    int i = findColumn(node.ref);
    if(i!=-1){
        vector<EvaluatorTree::e_node*> temp = EvaluatorTree::columns.at(i)->links;
        for(int j =0; j<temp.size();j++){
            if(temp.at(j)->value == node.value)
                results.push_back(temp.at(j));
        }
    }
}
void merge(EvaluatorTree::e_node parent, EvaluatorTree::e_node child, bool flag){
    if(!flag){//the both columns are not empty, do elimination
        vector<EvaluatorTree::e_node*> parent_sets;
        int index_parent =findColumn(parent.ref);
        int index_child =findColumn(child.ref);
        EvaluatorTree::findNode(parent_sets, parent);
        if(parent_sets.size()==0){//if parent is not in the tree
            //do nothing
        }//if
        else{//else found parent
            for(int i=0;i<parent_sets.size();i++){
                std::distance(EvaluatorTree::columns.at(i)->links, parent);
                
            }
            
        }//
    }//if column is not empty
    
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


void addBackNode(EvaluatorTree::e_node* start, EvaluatorTree::e_node node){



}
