#ifndef __Arc_h__  // idempotence - keep header from being included multiple times
#define __Arc_h__
#include "Node.h"

class Node;

class Arc
{
public:
	Arc();
	~Arc();
	Arc(Node* destNode, int weight);

	

	int getWeight() const;
	void setWeight(int weight);
	Node* getDestNode() const;
	void setDestNode(Node* DestNode);

	friend bool operator== (const Arc& lhs, const Arc& rhs)
	{
		if (lhs.m_weight == rhs.m_weight && lhs.getDestNode() == rhs.getDestNode())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	friend bool operator!=(const Arc& lhs, const Arc& rhs)
	{
		return !(lhs == rhs);
	}

private:

	Node* m_pDestNode;
	int m_weight;

};

#endif _Arc_h_