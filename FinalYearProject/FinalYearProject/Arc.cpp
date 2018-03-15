#include "Arc.h"

Arc::Arc()
{

}

Arc::~Arc()
{

}

Arc::Arc(Node * destNode, int weight)
{
	m_pDestNode = destNode;
	m_weight = weight;
}

int Arc::getWeight() const
{
	return m_weight;
}

void Arc::setWeight(int weight)
{
	m_weight = weight;
}

Node * Arc::getDestNode() const
{
	return m_pDestNode;
}

void Arc::setDestNode(Node* DestNode)
{
	m_pDestNode = DestNode;
}

