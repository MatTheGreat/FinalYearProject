#pragma once
#include "Node.h"
#include <vector>
#include "Graph.h"

class AlgorithimPath
{
public:
	AlgorithimPath();
	AlgorithimPath(int startIndex, int endIndex);
	~AlgorithimPath();
	bool DisplayPath(int & pathIndex, Graph & graph, MapDisplay & display);

	int startPointIndex;
	int endPointIndex;
	int openedNodesIndex;
	std::vector<int>* opendedNodes;
	std::vector<Node *> path;
	bool displayStarted;
private:

};

