#pragma once
#include <list>
#include <queue>

#include "Node.h"
#include "Arc.h"

#include "MapDisplay.h"

class Node;
class Arc;

class Graph
{
private:
	

public:

	std::vector<Node*> nodes;

	Graph();
	~Graph();

	bool addNode(int data, std::string id);
	void removeNode(int index);
	bool addArc(int from, int to, int weight);
	void removeArc(int from, int to);
	Arc* getArc(int from, int to);
	void clearMarks();
	void ucs(Node* pStart, Node* pDest, std::vector<Node *>& path);
	void aStar(Node* pStart, Node* pDest, std::vector<Node *>& path, std::vector<int> * openedNodes);
	void fraStar(Node* pStart, Node* pDest, std::vector<Node *>& path);
	void fraStarInitializeState(Node * currentNode, int currentIteration);

	bool NodeExists(std::string id);
	bool NodeExists(int index);

	int GetIndex(std::string id);

	class NodeSearchEstimateComparer
	{
	public:
		//Compares nodes f(n) values
		bool operator()(Node * n1, Node * n2)
		{
			//f(n) = h(n) + g(n)
			int p1 = n1->m_estDistToDest + n1->weight;
			int p2 = n2->m_estDistToDest + n2->weight;
			return p1 > p2;
		}
	};


	class NodeSearchCostComparer
	{
	public:
		//Compares nodes weight/distance values
		bool operator()(Node * n1, Node * n2)
		{
			int p1 = n1->weight;
			int p2 = n2->weight;
			return p1 > p2;
		}
	};

};

