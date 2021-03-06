
#include "Node.h"

Node::Node()
{

}

Node::~Node()
{

}

Arc* Node::GetArc(Node* pNode)
{
	//Get Iterator for the beginning of the ArcList
	std::list<Arc>::iterator iter = m_arcList.begin();

	//Get Iterator Position for the end of the ArcList
	std::list<Arc>::iterator endIter = m_arcList.end();

	Arc* pArc = nullptr;
	m_previous = nullptr;

	// find the arc that matches the node
	for (; iter != endIter && pArc == 0; ++iter) 
	{
		if ((*iter).getDestNode() == pNode) 
		{
			pArc = &((*iter));
		}
	}

	// returns null if not found
	return pArc;
}

void Node::AddArc(Node* pNode, int weight)
{
	// Create a new arc.
	Arc a;
	a.setDestNode(pNode);
	a.setWeight(weight);
	// Add it to the arc list.
	m_arcList.push_back(a);
}

void Node::removeArc(Node * pNode)
{
	std::list<Arc>::iterator iter = m_arcList.begin();
	std::list<Arc>::iterator endIter = m_arcList.end();

	int size = m_arcList.size();
	// find the arc that matches the node
	for (; iter != endIter && m_arcList.size() == size;++iter) 
	{
		if ((*iter).getDestNode() == pNode)
		{
			m_arcList.remove((*iter));
		}
	}
}

void Node::ResetNode()
{
	m_previous = nullptr;
	m_marked = false;
	weight = std::numeric_limits<int>::max() - 10000;
	m_estDistToDest = std::numeric_limits<int>::max() - 10000;
	m_removed = false;
	vValue = std::numeric_limits<int>::max() - 10000;
}

Node::Node(const Node &obj)
{
	std::cout << "Copy constructor allocating ptr." << std::endl;
	*m_previous = *obj.m_previous;
	id = obj.id;
	weight = obj.weight;
	m_arcList = obj.m_arcList;
	m_marked = obj.m_marked;
	m_estDistToDest = obj.m_estDistToDest;
	m_removed = obj.m_removed;
	generatediteration = obj.generatediteration;
}

Node& Node::operator=(const Node& other) 
{
	*m_previous = *other.m_previous;
	id = other.id;
	weight = other.weight;
	m_arcList = other.m_arcList;
	m_marked = other.m_marked;
	m_estDistToDest = other.m_estDistToDest;
	m_removed = other.m_removed;
	generatediteration = other.generatediteration;
	return *this;
}