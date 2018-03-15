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

	bool isRunning;

private:
	sf::RenderWindow* window;
	Graph graph;
	MapDisplay display;
	sf::Clock clock;
	int timeTaken = 0;
	std::vector<int> * openedNodes;
	bool displayPath;
	int displayTimer;
	int openedNodesIndex;
	std::vector<Node *> thepath;
};

