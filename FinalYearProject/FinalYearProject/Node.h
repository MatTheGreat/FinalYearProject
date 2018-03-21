#ifndef __Node_h__  // idempotence - keep header from being included multiple times
#define __Node_h__
#include "Arc.h"

#include <list>
#include <string>

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

private:

};

#endif _Node_h_