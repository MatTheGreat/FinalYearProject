#pragma once
#include <list>
#include <queue>

#include "Node.h"
#include "Arc.h"
#include "AlgorithimPath.h"

#include "MapDisplay.h"


class Node;
class Arc;
class AlgorithimPath;

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

class Graph
{
private:
	sf::Clock* m_timer;

public:

	std::vector<Node*> nodes;

	Graph() {};
	Graph(sf::Clock* timer);
	~Graph();

	bool addNode(int data, std::string id, int displayIndex, std::pair<int, int> position);
	void removeNode(int index);
	bool addArc(int from, int to, int weight);
	void removeArc(int from, int to);
	Arc* getArc(int from, int to);
	void clearMarks();
	// Gets Distance between two Nodes
	int GetManhattanDistance(int xPosOne, int yPosOne, int xPosTwo, int yPosTwo);
	void aStar(Node* pStart, Node* pDest, std::vector<Node *>& path, std::vector<int> * openedNodes);
	bool fraStar(Node* pStart, Node* pDest, std::vector<AlgorithimPath>& paths);
	void fraStarInitializeState(Node * currentNode, int currentIteration);
	bool fraComputeCostMinimalPath(std::vector<Node*> open, int currentIteration ,Node* start, Node* goal, std::vector<Node *>& path, std::vector<int> * openedNodes);
	void fraStep2(Node * start, Node * prevStart, std::vector<Node*> open);
	void fraStep4(std::vector<Node*> open, Node * start, int currentIteration);
	bool TestClosedList(Node *current, Node * start);
	void setHeuristic(Node * pStart, Node * pDest);
	void ResetGraph();
	Node* GetLowestFValue(std::vector<Node*> nodes);
	int GetLowestFValueIndex(std::vector<Node*> nodes);
	Node* GetLowestFValue(std::vector<Node*> nodes, float e);
	int GetLowestFValueIndex(std::vector<Node*> nodes, float e);
	bool NodeInVector(Node* node ,std::vector<Node*> nodeVector);
	int NodeInVectorIndex(Node* node, std::vector<Node*> nodeVector);

	bool gfraStar(Node* pStart, Node* pDest,  std::vector<AlgorithimPath>& paths);
	void gfraStep2(Node * start, Node * prevStart, std::vector<Node*> open, std::vector<Node*>& deleted);
	void gfraStep4(std::vector<Node*>& open, Node * start, int currentIteration, std::vector<Node*>& deleted);

	void araStar(Node* pStart, Node* pDest, int startPoint, int endPoint, std::vector<AlgorithimPath>& paths);
	float fValue(Node* current, float e);
	void araImprovePath(Node* pStart, Node* pDest, std::vector<Node *>& path, std::vector<Node*>& open, std::vector<Node*>& incons, float e, std::vector<int> * openedNodes);

	bool iaraStarImprovePath(Node * pStart, Node * pDest, std::vector<Node*>& path, std::vector<Node*>& open, std::vector<Node*>& incons, int e, std::vector<int> * openedNodes);
	bool iaraStarComputePath(Node * pStart, Node * pDest, std::vector<AlgorithimPath>& paths, std::vector<Node*>& open, std::vector<Node*>& incons, int e);
	void iaraStarStep1(Node* start, std::vector<Node*> open, std::vector<Node*> incons);
	void iaraStarStep2(Node* start, Node* prevStart, std::vector<Node*> open, std::vector<Node*> incons, std::vector<Node*> deleted);
	void iaraStarStep3(std::vector<Node*> deleted, std::vector<Node*> open);
	void iaraStarStep4(Node* goal, std::vector<Node*> open, int e, int emax);
	bool iaraStar(Node * pStart, Node * pDest, std::vector<AlgorithimPath>& paths);


	bool NodeExists(std::string id);
	bool NodeExists(int index);

	int GetIndex(std::string id);



};

