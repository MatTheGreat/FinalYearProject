﻿#include "Graph.h"
#include <iostream>


Graph::Graph(sf::Clock* timer)
{
	m_timer = timer;
}

Graph::~Graph()
{
	//for (int i = 0; i < nodes.size(); i++)
	//{
	//	//delete nodes.at(i);
	//}
}

bool Graph::addNode(int data, std::string id,int displayIndex, std::pair<int, int> position)
{
	if (NodeExists(id) == false)
	{
		Node * node = new Node();
		node->SetID(id);
		node->SetWeight(data);
		node->SetMarked(false);
		node->m_displayIndex = displayIndex;
		node->m_position = position;
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

// Gets Distance between two Nodes
int Graph::GetManhattanDistance(int xPosOne, int yPosOne, int xPosTwo, int yPosTwo)
{
	int length;
	length = sqrt((xPosTwo - xPosOne)*(xPosTwo - xPosOne));
	length += sqrt((yPosTwo - yPosOne)*(yPosTwo - yPosOne));
	return length;
}

void Graph::setHeuristic(Node * pStart, Node * pDest)
{

	for (int i = 0; i < nodes.size(); i++)
	{
		
		nodes.at(i)->weight = GetManhattanDistance(pDest->m_position.first, pDest->m_position.second, nodes.at(i)->m_position.first, nodes.at(i)->m_position.second);
		//Set Heuritic based on Manhattan Distance 
		nodes.at(i)->m_estDistToDest = nodes.at(i)->weight*0.9f;
		nodes.at(i)->weight = std::numeric_limits<int>::max() - 10000;
		std::cout << nodes.at(i)->id << " " << nodes.at(i)->m_estDistToDest << std::endl;

		nodes.at(i)->m_marked = false;
		nodes.at(i)->m_removed = false;
	}

}

void Graph::aStar(Node * pStart, Node * pDest, std::vector<Node*>& path , std::vector<int> * openedNodes)
{
	ResetGraph();
	//g(n) = nodeQueue.top()->weight
	//h(n) = estimated cost
	//f(n) = g(n)+h(n)

	if (pStart != 0)
	{
		//Calulate Heuristic
		setHeuristic(pStart,pDest);
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
			//Go through all of neighbouring nodes
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

bool Graph::fraStar(Node * pStart, Node * pDest, std::vector<AlgorithimPath>& paths)
{
	ResetGraph();
	setHeuristic(pStart, pDest);
	std::vector<Node*> open;
	//std::vector<Node*> closed;

	
	//std::vector<Node*> open;
	Node * start = pStart;
	Node * prevStart = pStart;
	Node * goal = pDest;

	clearMarks();
	for (int index = 0; index < nodes.size(); index++)
	{
		nodes.at(index)->generatediteration = 0;
		nodes.at(index)->m_previous = NULL;
	}
	int iteration = 1;
	fraStarInitializeState(pStart, iteration);
	start->weight = 0;
	start->m_marked = true;

	open.push_back(start);
	while (start != goal)
	{
		paths.push_back(AlgorithimPath(NodeInVectorIndex(start, nodes), NodeInVectorIndex(goal, nodes),"FRA*"));
		m_timer->restart();
		if (fraComputeCostMinimalPath(open,iteration,start,goal, paths.at(paths.size() - 1).path, paths.at(paths.size() - 1).opendedNodes) == false)
		{
			//No path found
			return false;
		}
		else
		{
			//for (int i = path.size() - 1; i > -1; i--)
			//{
			//	std::cout << "Location : " << path.at(i)->id << " Cost : " << path.at(i)->weight << " h(n) : " << path.at(i)->m_estDistToDest << std::endl;
			//	if (path.at(i) == nodes.at(NodeInVectorIndex(start,nodes)))
			//	{
			//		std::cout << std::endl;
			//	}
			//}
		}
		paths.at(paths.size() - 1).timeTaken = m_timer->getElapsedTime().asMilliseconds();
		bool openListInComplete = false;
		int pathIndex = paths.at(paths.size() - 1).path.size() - 2;
		while (TestClosedList(goal,start))
		{
			//follow path from start to goal
			if (start == goal)
			{
				//reached destination
				return true;
			}
			prevStart = start;

			//set new start
			pathIndex--;
			start = paths.at(paths.size() - 1).path.at(pathIndex);

			//set new goal
			goal;

			if (start != prevStart)
			{
				//Step2
				fraStep2(start, prevStart, open);
				openListInComplete = true;
			}
		}
		if (openListInComplete == true)
		{
			iteration++;
		}
		//Step 4
		fraStep4(open, start, iteration);

	}
	return true;
		/*
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
			if (start != previous start)
				Step2();
				openlist incomplete = true;
		if (openlist incomplete)
			iteration = iteration + 1;
		Step4();
		*/
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

bool Graph::TestClosedList(Node *current, Node * start)
{
	
	if (current == start || current->m_marked == true && current->m_previous == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Graph::fraComputeCostMinimalPath(std::vector<Node*> open, int currentIteration,Node* start, Node* goal, std::vector<Node *>& path,  std::vector<int> * openedNodes)
{
	
	Node* current;
	while (open.size() != 0)
	{

		current = GetLowestFValue(open);
		int index = GetLowestFValueIndex(open);

		std::swap(open.at(index), open.back());
		open.pop_back();

		int indexInNodes = NodeInVectorIndex(current,nodes);

		current->m_marked = true;

		openedNodes->push_back(GetIndex(current->id));
		std::list<Arc>::const_iterator iter = current->m_arcList.begin();
		std::list<Arc>::const_iterator endIter = current->m_arcList.end();

		for (; iter != endIter; iter++)
		{
			if (false == TestClosedList(iter->getDestNode(), start))
			{
				fraStarInitializeState(iter->getDestNode(), currentIteration);
				if (nullptr != current->GetArc(iter->getDestNode()))
				{
					int dist = current->weight + current->GetArc(iter->getDestNode())->getWeight();
					if (iter->getDestNode()->weight > dist)
					{
						iter->getDestNode()->weight = dist;
						iter->getDestNode()->previousIndex = indexInNodes;
						if (NodeInVector(iter->getDestNode(), open) == false)
						{
							open.push_back(iter->getDestNode());
						}
					}
					if (current == goal)
					{
							path.clear();
							Node* temp = current;
							path.push_back(current);
							while (temp != start)
							{
								temp = nodes.at(temp->previousIndex);
								path.push_back(temp);
							}
							return true;
					}
				}
				else
				{
					std::cout << "ERROR ARC NOT FOUND" << std::endl;
					//arc not found
				}
			}

		}
	}
	return false;
	/*
	while (OPEN not empty)
 		current = node with smallest f(n)
		OPEN.remove(current)
 		expanded(current) = true;
 		for each node in Succ(s)
 			if (NOT TestClosedList(node))
 			InitializeState(node);
 			if (g(node) >g(current) + distanceFrom(current, node))
 				g(node) = g(current) + distanceFrom(current, node);
 				parent(node) = current;
 				if (node not in OPEN)
 					OPEN.add(node)
			if (current == goal)
				return true;
	return false
	*/
}

void Graph::fraStep2(Node * start , Node * prevStart, std::vector<Node*> open)
{
	start->m_previous = nullptr;
	std::list<Arc>::const_iterator iter = prevStart->m_arcList.begin();
	std::list<Arc>::const_iterator endIter = prevStart->m_arcList.end();
	for (; iter != endIter; iter++)
	{
		if (iter->getDestNode()->m_previous != start)
		{
			iter->getDestNode()->m_previous = nullptr;
			if (NodeInVector(iter->getDestNode(), open) == true)
			{
				std::swap(open.at(NodeInVectorIndex(iter->getDestNode(), open)), open.back());
				open.pop_back();
			}
		}
	}
	/*
procedure Step2()
   parent(start) = NULL;
for each in node in the search tree rooted at previous start but not the subtree rooted at start
 	parent(node) = NULL;
 	if (node in OPEN)
 		OPEN.remove(node)
	*/
}

void Graph::fraStep4( std::vector<Node*> open, Node * start, int currentIteration)
{
	//go through every node neighbouring nodes to open
	for (int i = 0; i < open.size(); i++)
	{
		std::list<Arc>::const_iterator iter = open.at(i)->m_arcList.begin();
		std::list<Arc>::const_iterator endIter = open.at(i)->m_arcList.end();
		for (; iter != endIter; iter++)
		{
			if (TestClosedList(iter->getDestNode(),start) == true && NodeInVector(iter->getDestNode(),open)== false && iter->getDestNode()->m_marked == true)
			{
				open.push_back(iter->getDestNode());
			}
		}
	}
	for (int i = 0; i < open.size(); i++)
	{
		fraStarInitializeState(open.at(i),currentIteration);
		std::list<Arc>::const_iterator iter = open.at(i)->m_arcList.begin();
		std::list<Arc>::const_iterator endIter = open.at(i)->m_arcList.end();
		for (; iter != endIter; iter++)
		{
			if (iter->getDestNode()->m_marked == true)
			{
				int dist = iter->getDestNode()->weight + open.at(i)->GetArc(iter->getDestNode())->getWeight();
				if (TestClosedList(iter->getDestNode(), start) == true && open.at(i)->weight > dist)
				{
					open.at(i)->weight = dist;
					open.at(i)->m_previous = iter->getDestNode();
				}
			}
		}
	}

	/*
procedure Step4()
  for each node in graph on relevant part of outer perimeter of CLOSED list
 	if (node not in OPEN AND neighbour in Pred(s) : TestClosedList(neighbour))
 		OPEN.add(node)
  for each node in OPEN
 	InitializeState(node);
  for each node in OPEN
 	for each n in Pred(s)
 		if (TestClosedList(n) AND g(node) >g(n) + distanceFrom(n, node))
 			g(node) = g(n) + distanceFrom(n, node);
 			parent(node) = n;
	*/
}

void Graph::ResetGraph()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes.at(i)->ResetNode();
	}
}

Node * Graph::GetLowestFValue(std::vector<Node*> nodes)
{
	Node * lowestF = nodes.at(0);
	for (int i = 0; i < nodes.size(); i++)
	{
			//f(n) = h(n) + g(n)
			int p1 = lowestF->m_estDistToDest + lowestF->weight;
			int p2 = nodes.at(i)->m_estDistToDest + nodes.at(i)->weight;
			if (p1 > p2)
			{
				lowestF = nodes.at(i);
			}
	}
	return lowestF;
}

int Graph::GetLowestFValueIndex(std::vector<Node*> nodes)
{
	Node * lowestF = nodes.at(0);
	int lowestIndex = 0;
	for (int i = 0; i < nodes.size(); i++)
	{
		//f(n) = h(n) + g(n)
		int p1 = lowestF->m_estDistToDest + lowestF->weight;
		int p2 = nodes.at(i)->m_estDistToDest + nodes.at(i)->weight;
		if (p1 > p2)
		{
			lowestF = nodes.at(i);
			lowestIndex = i;
		}
	}
	return lowestIndex;
}

Node * Graph::GetLowestFValue(std::vector<Node*> nodes, float e)
{
	Node * lowestF = nodes.at(0);
	for (int i = 0; i < nodes.size(); i++)
	{
		//f(n) = h(n) + g(n)
		int p1 = e*lowestF->m_estDistToDest + lowestF->weight;
		int p2 = e*nodes.at(i)->m_estDistToDest + nodes.at(i)->weight;
		if (p1 > p2)
		{
			lowestF = nodes.at(i);
		}
	}
	return lowestF;
}

int Graph::GetLowestFValueIndex(std::vector<Node*> nodes, float e)
{
	Node * lowestF = nodes.at(0);
	int lowestIndex = 0;
	for (int i = 0; i < nodes.size(); i++)
	{
		//f(n) = h(n) + g(n)
		int p1 = e*lowestF->m_estDistToDest + lowestF->weight;
		int p2 = e*nodes.at(i)->m_estDistToDest + nodes.at(i)->weight;
		if (p1 > p2)
		{
			lowestF = nodes.at(i);
			lowestIndex = i;
		}
	}
	return lowestIndex;
}

bool Graph::NodeInVector(Node* node ,std::vector<Node*> nodeVector)
{
	for (int n = 0; n < nodeVector.size(); n++)
	{
		if (node == nodeVector.at(n))
		{
			return true;
		}
	}
	return false;
}

int Graph::NodeInVectorIndex(Node * node, std::vector<Node*> nodeVector)
{
	for (int n = 0; n < nodeVector.size(); n++)
	{
		if (node == nodeVector.at(n))
		{
			return n;
		}
	}
	return -1;
}

bool Graph::gfraStar(Node * pStart, Node * pDest, std::vector<AlgorithimPath>& paths)
{
	ResetGraph();
	setHeuristic(pStart, pDest);
	std::vector<Node*> open;
	std::vector<Node*> deleted;


	//std::vector<Node*> open;
	Node * start = pStart;
	Node * prevStart = pStart;
	Node * goal = pDest;

	clearMarks();
	for (int index = 0; index < nodes.size(); index++)
	{
		nodes.at(index)->generatediteration = 0;
		nodes.at(index)->m_previous = NULL;
	}
	int iteration = 1;
	fraStarInitializeState(pStart, iteration);
	start->weight = 0;
	start->m_marked = true;

	open.push_back(start);
	while (start != goal)
	{
		paths.push_back(AlgorithimPath(NodeInVectorIndex(start,nodes), NodeInVectorIndex(goal,nodes),"G-FRA*"));
		m_timer->restart();
		if (fraComputeCostMinimalPath(open, iteration, start, goal, paths.at(paths.size() - 1).path, paths.at(paths.size() - 1).opendedNodes) == false)
		{
			//No path found
			return false;
		}
		else
		{
			//for (int i = path.size() - 1; i > -1; i--)
			//{
			//	std::cout << "Location : " << path.at(i)->id << " Cost : " << path.at(i)->weight << " h(n) : " << path.at(i)->m_estDistToDest << std::endl;
			//	if (path.at(i) == nodes.at(NodeInVectorIndex(start,nodes)))
			//	{
			//		std::cout << std::endl;
			//	}
			//}
		}
		paths.at(paths.size() - 1).timeTaken = m_timer->getElapsedTime().asMilliseconds();
		bool openListInComplete = false;
		int pathIndex = paths.at(paths.size() - 1).path.size() - 2;
		while (TestClosedList(goal, start))
		{
			//follow path from start to goal
			if (start == goal)
			{
				//reached destination
				return true;
			}
			prevStart = start;

			//set new start
			pathIndex--;
			start = paths.at(paths.size() - 1).path.at(pathIndex);

			//set new goal
			goal;

			if (start != prevStart)
			{
				//Step2
				gfraStep2(start, prevStart, open, deleted);
				openListInComplete = true;
			}
		}
		if (openListInComplete == true)
		{
			iteration++;
		}
		//Step 4
		gfraStep4(open, start, iteration, deleted);

	}
	return true;
}

void Graph::gfraStep2(Node * start, Node * prevStart, std::vector<Node*> open, std::vector<Node*>& deleted)
{
	start->m_previous = nullptr;
	std::list<Arc>::const_iterator iter = prevStart->m_arcList.begin();
	std::list<Arc>::const_iterator endIter = prevStart->m_arcList.end();
	for (; iter != endIter; iter++)
	{
		if (iter->getDestNode()->m_previous != start)
		{
			iter->getDestNode()->m_previous = nullptr;
			deleted.push_back(iter->getDestNode());
			if (NodeInVector(iter->getDestNode(), open) == true)
			{
				std::swap(open.at(NodeInVectorIndex(iter->getDestNode(), open)), open.back());
				open.pop_back();
			}
		}
	}
}

void Graph::gfraStep4(std::vector<Node*>& open, Node * start, int currentIteration, std::vector<Node*> & deleted)
{
	for (int i = 0; i < open.size(); i++)
	{
		open.at(i)->generatediteration = currentIteration;
	}
	for (int i = 0; i < deleted.size(); i++)
	{
		fraStarInitializeState(deleted.at(i), currentIteration);
		std::list<Arc>::const_iterator iter = deleted.at(i)->m_arcList.begin();
		std::list<Arc>::const_iterator endIter = deleted.at(i)->m_arcList.end();
		for (; iter != endIter; iter++)
		{
			if (iter->getDestNode()->m_marked == true)
			{
				int dist = iter->getDestNode()->weight + deleted.at(i)->GetArc(iter->getDestNode())->getWeight();
				if (TestClosedList(iter->getDestNode(), start) == true && deleted.at(i)->weight > dist)
				{
					deleted.at(i)->weight = dist;
					deleted.at(i)->m_previous = iter->getDestNode();
				}
			}
		}
		open.push_back(deleted.at(i));
	}
	deleted.clear();
}

void Graph::araStar(Node * pStart, Node * pDest,int startPoint, int endPoint, std::vector<AlgorithimPath>& paths)
{
	ResetGraph();
	//Set Intial Heuristic
	setHeuristic(pStart,pDest);

	std::vector<Node*> open;
	std::vector<Node*> incons;

	//Set Inflated Heuristic Value
	float e = 3.0f;

	//Intial first node
	pStart->weight = 0;
	open.push_back(pStart);

	//Set up path variables
	paths.push_back(AlgorithimPath(startPoint, endPoint,"ARA*"));
	paths.at(paths.size() - 1).eValue = e;

	m_timer->restart();
	//Run Improve Path
	araImprovePath(pStart, pDest, paths.at(paths.size()-1).path, open, incons, e, paths.at(paths.size() - 1).opendedNodes);
	paths.at(paths.size() - 1).timeTaken = m_timer->getElapsedTime().asMilliseconds();

	while (e > 1)
	{
		//Reduce value of inflation on heuristic
		e--;
		for (int i = 0; i < incons.size(); i++)
		{
			//add any inconsistant nodes to open
			open.push_back(incons.at(i));
		}
		for (int n = 0; n < nodes.size(); n++)
		{
			//set all nodes to having not been visited
			nodes.at(n)->m_marked = false;
		}
		//Set up path variables
		paths.push_back(AlgorithimPath(startPoint, endPoint,"ARA*"));
		paths.at(paths.size() - 1).eValue = e;

		m_timer->restart();
		//Run Improve Path
		araImprovePath(pStart, pDest, paths.at(paths.size() - 1).path, open, incons, e, paths.at(paths.size() - 1).opendedNodes);
		paths.at(paths.size() - 1).timeTaken = m_timer->getElapsedTime().asMilliseconds();
	}
}

float Graph::fValue(Node * current, float e)
{
	return (current->weight + e*(current->m_estDistToDest));
}

void Graph::araImprovePath(Node * pStart, Node * pDest, std::vector<Node*>& path, std::vector<Node*>& open, std::vector<Node*>& incons, float e, std::vector<int> * openedNodes)
{
	if (open.size() != 0)
	{
		Node* current;
		current = GetLowestFValue(open, e);
		int index = GetLowestFValueIndex(open, e);

		while (fValue(pDest, e) > fValue(current, e))
		{
			current = GetLowestFValue(open, e);
			int index = GetLowestFValueIndex(open, e);
			std::swap(open.at(index), open.back());
			open.pop_back();
			int indexInNodes = NodeInVectorIndex(current, nodes);
			current->m_marked = true;

			openedNodes->push_back(GetIndex(current->id));
			std::list<Arc>::const_iterator iter = current->m_arcList.begin();
			std::list<Arc>::const_iterator endIter = current->m_arcList.end();
			
			for (; iter != endIter; iter++)
			{
				
				int dist = current->weight + current->GetArc(iter->getDestNode())->getWeight();
				if (iter->getDestNode()->weight > dist)
				{
					iter->getDestNode()->weight = dist;
					iter->getDestNode()->previousIndex = indexInNodes;
					if (iter->getDestNode()->m_marked == false)
					{
						open.push_back(iter->getDestNode());
					}
					else
					{
						incons.push_back(iter->getDestNode());
					}
				}
				if (iter->getDestNode() == pDest)
				{
					path.clear();
					Node* temp = current;
					path.push_back(current);
					while (temp != pStart)
					{
						temp = nodes.at(temp->previousIndex);
						path.push_back(temp);
					}
				}

			}
		}
	}

	/*
	02 while(fvalue(goal) > node with smallest fvalue in open list)
		03 remove s with the smallest fvalue(s) from OPEN;
		04 Add s to closed list
		05 for each successor s0 of s
			06 if s0 was not visited before then
				07 g(s0) =∞;
			08 if g(s0) > g(s)+ c(s,s0)
				09 g(s0) = g(s)+ c(s,s0);
				10 if s0 6∈ CLOSED
					11 insert s0 into OPEN with fvalue(s0);
				12 else
					13 insert s0 into INCONS;
	*/
}



//The v-value v(s) is the gvalue at the time of the last expansion of cell s. Initially, it is infinity.

bool Graph::iaraStarImprovePath(Node * pStart, Node * pDest, std::vector<Node*>& path, std::vector<Node*>& open, std::vector<Node*>& incons, int e, std::vector<int> * openedNodes)
{
	// function ImprovePath()
	if (open.size() != 0)
	{
		Node* current;
		current = GetLowestFValue(open, e);
		int index = GetLowestFValueIndex(open, e);

		while (fValue(pDest, e) > fValue(current, e))
		{
			current = GetLowestFValue(open, e);
			int index = GetLowestFValueIndex(open, e);
			std::swap(open.at(index), open.back());
			open.pop_back();
			int indexInNodes = NodeInVectorIndex(current, nodes);
			nodes.at(indexInNodes)->vValue = nodes.at(indexInNodes)->weight;
			current->m_marked = true;

			openedNodes->push_back(GetIndex(current->id));
			std::list<Arc>::const_iterator iter = current->m_arcList.begin();
			std::list<Arc>::const_iterator endIter = current->m_arcList.end();

			for (; iter != endIter; iter++)
			{
				if (iter->getDestNode()->weight > (current->vValue + iter->getWeight()))
				{
					iter->getDestNode()->weight = current->vValue + iter->getWeight();
					iter->getDestNode()->m_previous = current;
					iter->getDestNode()->previousIndex = indexInNodes;
					if (iter->getDestNode()->m_marked == false)
					{
						if (NodeInVector(iter->getDestNode(), open) == false && NodeInVector(iter->getDestNode(), incons)==false)
						{
							open.push_back(iter->getDestNode());
						}
					}
					else
					{
						iter->getDestNode()->m_marked = false;
						incons.push_back(iter->getDestNode());
					}
				}
			}
		}
		if (pDest->weight == std::numeric_limits<int>::max() - 10000)
		{
			return false;
		}
		else
		{
			path.clear();
			Node* temp = pDest;
			path.push_back(pDest);
			while (temp != pStart)
			{
				temp = nodes.at(temp->previousIndex);
				path.push_back(temp);
			}
			return true;
		}
	}
		// while g(sgoal) + e × h(sgoal, sgoal) > mins∈OPEN(g(s) + e × h(s, sgoal))
			// move s in OPEN with the smallest g(s) + e × h(s, sgoal) from OPEN to CLOSED;
			// v(s) = g(s);
			// forall s′ int Neighbor(s)
				// if g(s′) > v(s) + c(s, s′)
					// g(s′) = v(s) + c(s, s′);
					// parent(s′) := s;
					// if s′ not in CLOSED
						// if s′ not in OPEN AND s′ not in INCONS
							// insert s′into OPEN;
					// else
						// move s′from CLOSED to INCONS;
	// if g(sgoal) = ∞
		// return false;
	// else
		// return true;
}

bool Graph::iaraStarComputePath(Node * pStart, Node * pDest, std::vector<AlgorithimPath>& paths, std::vector<Node*>& open, std::vector<Node*>& incons, int e)
{
	while (true)
	{
		
		paths.push_back(AlgorithimPath(NodeInVectorIndex(pStart,nodes), NodeInVectorIndex(pDest, nodes),"I-ARA*"));
		paths.at(paths.size() - 1).eValue = e;
		m_timer->restart();
		if (iaraStarImprovePath(pStart, pDest, paths.at(paths.size() - 1).path, open, incons, e, paths.at(paths.size() - 1).opendedNodes) == false)
		{
			return false;
		}
		if (e == 1)
		{
			return true;
		}
		for (int i = 0; i < incons.size(); i++)
		{
			open.push_back(incons.at(i));
		}
		clearMarks();
		incons.clear();
		e = std::max(1, e - 1);
		paths.at(paths.size() - 1).timeTaken = m_timer->getElapsedTime().asMilliseconds();
	}
}

void Graph::iaraStarStep1(Node* start, std::vector<Node*> open, std::vector<Node*> incons)
{
	if (start->weight != start->vValue)
	{
		start->weight = start->vValue;
		if (NodeInVector(start, incons) == true)
		{
			int index = NodeInVectorIndex(start, incons);
			std::swap(incons.at(index), incons.back());
			incons.pop_back();
		}
		if (NodeInVector(start, open) == true)
		{
			int index = NodeInVectorIndex(start, open);
			std::swap(open.at(index), open.back());
			open.pop_back();
		}
	}
	//if weight(startNode) != v(startNode)
		//weight(startNode) = v(startNode);
		// delete startNode from INCONS if startNode in INCONS;
		// delete startNode from OPEN if startNode in OPEN;
}

void Graph::iaraStarStep2(Node* start, Node* prevStart, std::vector<Node*> open, std::vector<Node*> incons, std::vector<Node*> deleted)
{
	if (start != prevStart)
	{
		start->m_previous = nullptr;
		std::list<Arc>::const_iterator iterPrev = prevStart->m_arcList.begin();
		std::list<Arc>::const_iterator endIterPrev = prevStart->m_arcList.end();
		for (; iterPrev != endIterPrev; iterPrev++)
		{
			std::list<Arc>::const_iterator iter = start->m_arcList.begin();
			std::list<Arc>::const_iterator endIter = start->m_arcList.end();
			bool nodeInSubTree = false;
			for (; iter != endIter; iter++)
			{
				if (iterPrev->getDestNode() == iter->getDestNode())
				{
					nodeInSubTree = true;
				}
			}
			if (nodeInSubTree == false)
			{
				iterPrev->getDestNode()->vValue = iterPrev->getDestNode()->weight;
				iterPrev->getDestNode()->m_previous = nullptr;
				if (NodeInVector(iterPrev->getDestNode(), incons) == true)
				{
					int index = NodeInVectorIndex(iterPrev->getDestNode(), incons);
					std::swap(incons.at(index), incons.back());
					incons.pop_back();
				}
				if (NodeInVector(iterPrev->getDestNode(), open) == true)
				{
					int index = NodeInVectorIndex(iterPrev->getDestNode(), open);
					std::swap(open.at(index), open.back());
					open.pop_back();
				}
				deleted.push_back(iterPrev->getDestNode());
			}

		}

	}
	// procedure Step2()
		// if start != previous start
			// parent(start) := NULL;
			// forall s in the search tree rooted at previous sstart but not in the subtree rooted at sstart
				// v(s) = g(s) = ∞;
				// parent(s) = NULL;
				// delete s from INCONS if s in INCONS;
				// delete s from OPEN if s in OPEN;
				// insert s into DELETED;

}

void Graph::iaraStarStep3(std::vector<Node*> deleted, std::vector<Node*> open)
{
	
	for (int i = 0; i < deleted.size(); i++)
	{
		std::list<Arc>::const_iterator iter = deleted.at(i)->m_arcList.begin();
		std::list<Arc>::const_iterator endIter = deleted.at(i)->m_arcList.end();
		for (; iter != endIter; iter++)
		{
			if (deleted.at(i)->weight > iter->getDestNode()->vValue + iter->getDestNode()->GetArc(deleted.at(i))->getWeight())
			{
				deleted.at(i)->weight = iter->getDestNode()->vValue + iter->getDestNode()->GetArc(deleted.at(i))->getWeight();
				deleted.at(i)->m_previous = iter->getDestNode();
			}
		}
		if (deleted.at(i)->weight != std::numeric_limits<int>::max() - 10000)
		{
			open.push_back(deleted.at(i));
		}
	}

	open.clear();
	deleted.clear();
	clearMarks();

	// procedure Step3()
		// forall s in DELETED
			// forall s′ in Neighbor(s)
				// if g(s) > v(s′) + c(s′, s)
					// g(s) := v(s′) + c(s′, s);
					// parent(s) := s′;
			// if g(s) 6 = ∞
				// insert s into OPEN;
	// OPEN : = OPEN and INCONS;
	// CLOSED : = INCONS : = DELETED : = NULL;

}

void Graph::iaraStarStep4(Node* goal, std::vector<Node*> open, int e, int emax)
{
	// procedure Step4()
	Node* lowestfValueNode;
	lowestfValueNode = GetLowestFValue(open, e);
	int index = GetLowestFValueIndex(open, e);
	if (fValue(goal,e) < fValue(open.at(index),e))
	{
		e = emax;
	}
	else
	{
		e = std::max(1, e - 1);
	}
		// if g(sgoal) + e × h(sgoal, sgoal) > mins∈OPEN(g(s) + e × h(s, sgoal))
		// e = emax;
	// else
		// e = max(1, e − δe);
}

bool Graph::iaraStar(Node * pStart, Node * pDest, std::vector<AlgorithimPath>& paths)
{
	ResetGraph();
	setHeuristic(pStart,pDest);
	float e = 3.0f;
	std::vector<Node*> open;
	std::vector<Node*> incons;
	std::vector<Node*> deleted;
	Node * start = pStart;
	Node * prevStart = pStart;
	Node * goal = pDest;
	start->weight = 0;
	open.push_back(start);

	while (start != goal)
	{

		if (iaraStarComputePath(start,goal, paths,open,incons,e) == false)
		{
			return false;
		}
		prevStart = start;

		if (start == goal)
		{
			//reached destination
			return true;
		}
		else
		{
			int pathIndex = paths.at(paths.size() - 1).path.size() - 1;
			//set new start
			pathIndex--;
			//std::cout << "New Start : " << paths.at(paths.size() - 1).path.at(pathIndex)->id << std::endl;
			start = paths.at(paths.size() - 1).path.at(pathIndex);
		}
		//set new goal
		goal;

		iaraStarStep1(start,open,incons);
		iaraStarStep2(start,prevStart,open,incons,deleted);
		iaraStarStep3(deleted,open);
		iaraStarStep4(goal,open,e,5);
	}
	return true;
}
