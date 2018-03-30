#ifndef __Node_h__  // idempotence - keep header from being included multiple times
#define __Node_h__
#include "Arc.h"

#include <list>
#include <string>
#include <iostream>

class Arc;

class Node
{
public:
	Node();
	~Node();

	Arc* GetArc(Node* pNode);
	void AddArc(Node* pNode, int weight);
	void removeArc(Node* pNode);
	void ResetNode();

	std::string id = "NULL";
	int weight;
	std::list<Arc> m_arcList;
	bool m_marked;
	Node *m_previous;
	int m_estDistToDest;
	bool m_removed;
	int generatediteration;
	int previousIndex;

	void SetID(std::string newID)
	{
		id = newID;
	}
	void SetMarked(bool marked)
	{
		m_marked = marked;
	}
	void SetWeight(int newWeight)
	{
		weight = newWeight;
	}
	Node * getPrevious()
	{
		return m_previous;
	}
	void SetPrevious(Node *previous)
	{
		if (previous != nullptr)
		{
			m_previous = previous;
		}
	}

	Node(const Node &obj);
	Node& Node::operator=(const Node& other);

private:

};

#endif _Node_h_