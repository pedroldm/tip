#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <memory>
#include <cmath> 
#include <mutex>
#include <atomic>
#include <algorithm>
#include <iostream>
#include "Problem.h"

using namespace std;

class Node{
	//private:
		
	protected:
		atomic<int>* execMax;
		mutex mtxNode;
		int execAtual = 0; 
		bool endN = false; 
		atomic<int>* indexPT;
		vector<std::pair<Node*,bool>> edgeFrom;
		vector<std::pair<Node*,bool>> edgeto;
	public:
		Node();
		~Node();
		virtual void run() = 0;
		virtual bool ready() = 0;
		virtual bool notify() = 0;
		virtual bool observer(Node* from) = 0;
		bool addEdge(Node* from, Node* to);
		bool theEnd();	
		bool finish();
		void reset();
		void printEdgeto();
		void printEdgeFrom();
};

Node::Node(){
}

Node::~Node(){
}

bool Node::addEdge(Node* from, Node* to){
 if (from == this){
	edgeFrom.push_back(std::make_pair(to,true));
	return true;
 } else if(to == this){
	edgeto.push_back(std::make_pair(from,false));
	return true; 
 }else{
	std::cout<<"This edge isn't related with this node!!!!"<<std::endl;
	return false;
 }
}

void Node::reset(){
	for(vector<std::pair<Node*,bool>>::iterator it = edgeto.begin(); it != edgeto.end(); it++){
		it->second = false;		
	}
}

bool Node::theEnd(){
	execAtual++;
	
	return (*execMax <= execAtual);
}

bool Node::finish(){
	return (*execMax <= execAtual);
}

void Node::printEdgeto(){
	cout<<"Node:"<<this<<endl;
	
	for(vector<std::pair<Node*,bool>>::iterator it = edgeto.begin(); it != edgeto.end(); it++){
		cout<<"Pointer:"<<it->first<<" status:"<<it->second<<endl;
	}
}

void Node::printEdgeFrom(){
	cout<<"Node:"<<this<<endl;
	
	for(vector<std::pair<Node*,bool>>::iterator it = edgeFrom.begin(); it != edgeFrom.end(); it++){
		cout<<"Pointer:"<<it->first<<" status:"<<it->second<<endl;
	}
}

#endif
