#ifndef __ALGORITHIMPATH_h__  
#define __ALGORITHIMPATH_h__

#include "Node.h"
#include <vector>
#include "Graph.h"
#include "MapDisplay.h"


class Graph;

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
	int eValue;
private:

};

#endif