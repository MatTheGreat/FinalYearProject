#include "Graph.h"
#include <iostream>


Graph::Graph()
{

}

Graph::~Graph()
{
	//for (int i = 0; i < nodes.size(); i++)
	//{
	//	//delete nodes.at(i);
	//}
}

bool Graph::addNode(int data, std::string id)
{
	if (NodeExists(id) == false)
	{
		Node * node = new Node();
		node->SetID(id);
		node->SetWeight(data);
		node->SetMarked(false);
		nodes.push_back(node);

		return true;
	}
	else
	{
		return false;
	}
}

void Graph::removeNode(int index)
{
	Arc* arc;
	for (int i = 0; i < nodes.size(); i++)
	{
		arc = nodes.at(i)->GetArc(nodes.at(index));
		if (arc != nullptr)
		{
			removeArc(i, index);
		}
	}

}

bool Graph::addArc(int from, int to, int weight)
{
	bool proceed = true;

	if (NodeExists(from) == false || NodeExists(to) == false)
	{
		proceed = false;
	}

	 //if an arc already exists we should not proceed
	if (nodes.at(from)->GetArc(nodes.at(to)) != 0) 
	{
		proceed = false;
	}

	if (proceed == true) 
	{
		// add the arc to the "from" node.
		nodes.at(from)->AddArc(nodes.at(to), weight);
	}

	return proceed;
}

void Graph::removeArc(int from, int to)
{
	bool nodeExists = true;
	if (NodeExists(from) == false || NodeExists(to) == false)
	{
		nodeExists = false;
	}

	if (nodeExists == true) 
	{
		nodes.at(from)->removeArc(nodes.at(to));
	}
}

Arc * Graph::getArc(int from, int to)
{
	Arc* pArc = 0;
	// make sure the to and from nodes exist
	if (NodeExists(from) == true && NodeExists(to) == true)
	{
		pArc = nodes.at(from)->GetArc(nodes.at(to));
	}

	return pArc;
}

void Graph::clearMarks()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes.at(i)->m_marked = false;
	}
}

bool Graph::NodeExists(std::string id)
{
	bool nodePresent = false;
	for (int i = 0; i < nodes.size(); i++)
	{
		if (id == nodes.at(i)->id)
		{
			nodePresent = true;
			break;
		}
	}
	if (nodePresent == true)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool Graph::NodeExists(int index)
{
	bool nodePresent = false;
	if (index <= nodes.size())
	{
		nodePresent = true;
	}
	if (nodePresent == true)
	{
		return true;
	}
	else
	{
		return false;
	}

}

int Graph::GetIndex(std::string id)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (id == nodes.at(i)->id)
		{
			return i;
		}
	}
	return -1;
}

void Graph::ucs(Node * pStart, Node * pDest, std::vector<Node*>& path)
{
	if (pStart != 0)
	{
		for (int index = 0; index < nodes.size(); index++) 
		{
			if (nodes.at(index) != nullptr) 
			{
				nodes.at(index)->weight = std::numeric_limits<int>::max() - 10000;
			}
		}
		bool foundPath = false;
		std::priority_queue<Node*, std::vector<Node*>, NodeSearchCostComparer> nodeQueue;
		nodeQueue.push(pStart);
		pStart->m_marked = true;
		pStart->weight = 0;
		while (nodeQueue.size() != 0 && nodeQueue.top() != pDest)
		{
			std::list<Arc>::const_iterator iter = nodeQueue.top()->m_arcList.begin();
			std::list<Arc>::const_iterator endIter = nodeQueue.top()->m_arcList.end();
			for (; iter != endIter; iter++)
			{
				if ((*iter).getDestNode() != nodeQueue.top())
				{
					int dist = nodeQueue.top()->weight + iter->getWeight();

					if ((*iter).getDestNode()->m_marked == false)
					{
						// mark the node and add it to the queue.
						(*iter).getDestNode()->m_previous = nodeQueue.top();
						if (dist < (*iter).getDestNode()->weight)
						{
							(*iter).getDestNode()->weight = dist;
						}
						(*iter).getDestNode()->m_marked = true;
						nodeQueue.push((*iter).getDestNode());
					}
					if (dist < (*iter).getDestNode()->weight)
					{
						(*iter).getDestNode()->weight = dist;
						(*iter).getDestNode()->m_previous = nodeQueue.top();
					}
					if ((*iter).getDestNode() == pDest)
					{
						if (dist <= (*iter).getDestNode()->weight)
						{
							(*iter).getDestNode()->weight = dist;
							(*iter).getDestNode()->m_previous = nodeQueue.top();
							if (foundPath == true)
							{
								path.clear();
							}
							Node* temp = (*iter).getDestNode();
							path.push_back((*iter).getDestNode());
							while (temp != pStart)
							{
								temp = temp->m_previous;
								path.push_back(temp);
							}
							foundPath = true;
						}
					}

				}
			}
			nodeQueue.pop();
		}
	}
}

void Graph::aStar(Node * pStart, Node * pDest, std::vector<Node*>& path , std::vector<int> * openedNodes)
{
	//g(n) = nodeQueue.top()->data().second
	//h(n) = estimated cost
	//f(n) = g(n)+h(n)

	if (pStart != 0)
	{
		//run UCS in opposite direction
		std::vector<Node*> ucsPath;
		//ucs(pDest, pStart, ucsPath);
		ucs(pDest, pStart, ucsPath);


		for (int index = 0; index < nodes.size(); index++)
		{
			if (nodes.at(index) != nullptr)
			{
				nodes.at(index)->m_estDistToDest = nodes.at(index)->weight*0.9f;
				nodes.at(index)->weight = std::numeric_limits<int>::max() - 10000;
				std::cout << nodes.at(index)->id << " " << nodes.at(index)->m_estDistToDest << std::endl;

				nodes.at(index)->m_marked = false;
				nodes.at(index)->m_removed = false;
			}
		}
		std::priority_queue<Node*, std::vector<Node*>, NodeSearchEstimateComparer> nodeQueue;

		//Add starting node to queue and set its distance and that it has been visited
		nodeQueue.push(pStart);
		pStart->m_marked = true;
		pStart->weight = 0;

		while (nodeQueue.size() != 0 && nodeQueue.top() != pDest)
		{
			openedNodes->push_back(GetIndex(nodeQueue.top()->id));
			std::list<Arc>::const_iterator iter = nodeQueue.top()->m_arcList.begin();
			std::list<Arc>::const_iterator endIter = nodeQueue.top()->m_arcList.end();
			for (; iter != endIter; iter++)
			{
				if ((*iter).getDestNode()->m_previous != nodeQueue.top())
				{
					int dist = nodeQueue.top()->weight + iter->getWeight();

					//Checks if distance is shorter than current shortest distance to this node
					if (dist < (*iter).getDestNode()->weight)
					{
						//sets new distance
						(*iter).getDestNode()->weight = dist;
						//sets pointer for previous node to the node at the top of the queue
						(*iter).getDestNode()->m_previous = nodeQueue.top();
					}

					//Checks to see if the node has been visited
					if ((*iter).getDestNode()->m_marked == false)
					{
						//sets pointer for previous node to the node at the top of the queue
						(*iter).getDestNode()->m_previous = nodeQueue.top();
						// mark the node and add it to the queue.
						(*iter).getDestNode()->m_marked = true;
						nodeQueue.push((*iter).getDestNode());
					}

					if ((*iter).getDestNode() == pDest)
					{
						if (dist <= (*iter).getDestNode()->weight)
						{
							//sets new distance
							(*iter).getDestNode()->weight = dist;
							//sets pointer for previous node to the node at the top of the queue
							(*iter).getDestNode()->m_previous = nodeQueue.top();
							//Clears contents of vector
							path.clear();
							Node* temp = (*iter).getDestNode();
							path.push_back((*iter).getDestNode());
							//Gets pointer to the previous node and adds it to the path vector
							while (temp != pStart)
							{
								temp = temp->m_previous;
								path.push_back(temp);
							}
						}
					}
				}
			}
			nodeQueue.pop();
		}
	}
}




void Graph::fraStar(Node * pStart, Node * pDest, std::vector<Node*>& path)
{
	int iteration = 1;
	std::vector<Node*> open;
	Node * start = pStart;

	clearMarks();
	for (int index = 0; index < nodes.size(); index++)
	{
		nodes.at(index)->generatediteration = 0;
		nodes.at(index)->m_previous = NULL;
	}
	fraStarInitializeState(pStart, iteration);
	start->weight = 0;
	start->m_marked = true;

	open.push_back(start);
	while (start != pDest)
	{

	}
}


void Graph::fraStarInitializeState(Node * currentNode, int currentIteration)
{
	if (currentNode->generatediteration != currentIteration)
	{
		currentNode->weight = std::numeric_limits<int>::max() - 10000;
		currentNode->generatediteration = currentIteration;
		currentNode->m_marked = false;
	}
}

/*


Current = node currently being evaluated
OPEN = list with all nodes to be evaulated
Pred(s) = states from previous searches
Succ(s) = states to be passed to the next searches

procedure InitializeState(current)
	if generatediteration(current) not equal to iteration
		g(current) = infinity
		generatediteration(current) = iteration;
		expanded(current) = false;

function TestClosedList(current)
	 return (s = startNode OR(expanded(s) AND parent(s) = NULL));

function ComputeCostMinimalPath()
 while (OPEN not empty)
 	current = node with smallest f(n)
	OPEN.remove(current)
 	expanded(current) = true;
 	for each node in Succ(s)
 		if (NOT TestClosedList(node))
 		InitializeState(node);
 		if (g(node) >g(current) + distanceFrom(current, node))
 			g(current) = g(current) + distanceFrom(current, node);
 			parent(node) = current;
 			if (node not in OPEN)
 				OPEN.add(node)
	 if (current == goal)
		return true;
 return false;

procedure Step2()
   parent(start) = NULL;
for each in node in the search tree rooted at previous start but not the subtree rooted at start
 	parent(node) = NULL;
 	if (node in OPEN)
 		OPEN.remove(node)


procedure Step4()
  for each node in graph on relevant part of outer perimeter of CLOSED list
 	if (node not in OPEN AND neighbour in Pred(s) : TestClosedList(neighbour))
 		OPEN.add(node)
  for each node in OPEN
 	InitializeState(node);
  for each node in OPEN
 	for each n in Pred(s)
 		if (TestClosedList(n) AND g(node) >g(node) + distanceFrom(n, node))
 			g(node) = g(n) + distanceFrom(n, node);
 			parent(node) = n;



function Main()
  for each node in graph
	 generatediteration(node) = 0;
 	 expanded(node) = false;
 	 parent(node) = NULL;
   iteration = 1;
   InitializeState(start);
   g(start) = 0;
   OPEN = { start };
   while (start not equal goal)
  	if (NOT ComputeCostMinimalPath())
 		return false;
 	openlist incomplete : = false;
 	while (TestClosedList(goal))
 		while (target is on path from start to goal and not caught)
 			follow cost - minimal path from start to goal;
 		if (target is caught)
 			return true;
 		previous start = start;
 		start = the current state of the hunter;
 		goal = the current state of the target;
 		if (start = previous start)
 			Step2();
 			openlist incomplete = true;
 	if (openlist incomplete)
 		iteration = iteration + 1;
 		Step4();
return true;


*/