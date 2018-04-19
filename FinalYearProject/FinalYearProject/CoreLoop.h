#pragma once
#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Graph.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include "Outputter.h"
#include "AlgorithimPath.h"
#include "MapDisplay.h"


class CoreLoop
{
public:
	CoreLoop();
	~CoreLoop();
	void Update();
	void Draw();
	void Loop();
	void RunAStar(Graph graph);
	void OutputPathToConsole(std::vector<Node *> path);
	void OutputToCSVFile(int startPoint, int endPoint, int timeTaken, int nodesOpened);
	void DisplayPath();
	void GenerateMap();
	void RunFRAStar(Graph graph);
	void RunARAStar(Graph graph);
	bool IsObstacle(int column, int row);
	bool IsObstacle(int index);

	bool isRunning;

private:
	sf::RenderWindow* window;
	Graph graph;
	MapDisplay display;
	sf::Clock clock;
	int timeTaken;
	//std::vector<int> * openedNodes;
	bool displayPath;
	int displayTimer;
	int pathIndex;
	Outputter * output;
	int pathTimer;
	//std::vector<Node *> thepath;
	std::vector<AlgorithimPath> m_Paths;
	std::vector<std::pair<int, int>> obstcles;
	std::vector<int> obstcleIndex;

};

