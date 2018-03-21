#include "AlgorithimPath.h"

AlgorithimPath::AlgorithimPath()
{
	startPointIndex = -1;
	endPointIndex = -1;
	openedNodesIndex = 0;
	opendedNodes = new std::vector<int>();
	path = std::vector<Node*>();
	displayStarted = false;
}

AlgorithimPath::AlgorithimPath(int startIndex, int endIndex)
{
	startPointIndex = startIndex;
	endPointIndex = endIndex;
	openedNodesIndex = 0;
	opendedNodes = new std::vector<int>();
	path = std::vector<Node*>();
	displayStarted = false;
}

AlgorithimPath::~AlgorithimPath()
{

}

bool AlgorithimPath::DisplayPath(int & pathIndex , Graph & graph , MapDisplay & display)
{
	if (openedNodesIndex < opendedNodes->size())
	{
		if (startPointIndex != opendedNodes->at(openedNodesIndex))
		{
			display.ChangeTile(opendedNodes->at(openedNodesIndex), "Yellow");
		}
		openedNodesIndex++;
		return false;
	}
	else
	{
		for (int i = path.size() - 1; i > -1; i--)
		{
			if (startPointIndex != graph.GetIndex(path.at(i)->id) && endPointIndex != graph.GetIndex(path.at(i)->id))
			{
				display.ChangeTile(graph.GetIndex(path.at(i)->id), "Green");
			}
		}
		
		display.ChangeTile(startPointIndex, "Orange");
		display.ChangeTile(endPointIndex, "Red");
		return true;
	}
}
