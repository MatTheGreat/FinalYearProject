#include "CoreLoop.h"

const int STARTPOINT = 17;
const int ENDPOINT = 151;



void CoreLoop::RunAStar(Graph graph)
{
	openedNodes = new std::vector<int>();
	// Now traverse the graph.
	graph.aStar(graph.nodes.at(STARTPOINT), graph.nodes.at(ENDPOINT), thepath,openedNodes);

	timeTaken = clock.getElapsedTime().asMilliseconds();
	displayPath = true;

	

	for (int i = thepath.size() - 1; i > -1; i--)
	{
		std::cout << "Location : " << thepath.at(i)->id << " Cost : " << thepath.at(i)->weight << " h(n) : " << thepath.at(i)->m_estDistToDest << std::endl;
		if (thepath.at(i) == graph.nodes.at(STARTPOINT))
		{
			std::cout << std::endl;
		}
	}
	display.ChangeTile(STARTPOINT, "Orange");
	display.ChangeTile(ENDPOINT, "Red");
}

CoreLoop::CoreLoop()
{
	isRunning = true;
	window = new sf::RenderWindow(sf::VideoMode(768, 768), "Final Year Project");
	window->setFramerateLimit(60.0f);

	graph = Graph();
	std::string temp;
	std::ifstream myfile;

	displayPath = false;
	displayTimer = 0;
	openedNodesIndex = 0;
	
	for (int c = 0; c < 16; c++)
	{
		for (int r = 0; r < 16; r++)
		{
			temp = "R :" + std::to_string(r)+" C: " + std::to_string(c);
			graph.addNode(std::numeric_limits<int>::max() - 10000, temp);
		}
	}
	int from, to, weight;

	for (int c = 0; c < 16; c++)
	{
		for (int r = 0; r < 16; r++)
		{
			std::string IDstring;
			IDstring = "R :" + std::to_string(r) + " C: " + std::to_string(c);
			if (r != 0)
			{
				std::string destString;
				destString = "R :" + std::to_string((r - 1))+" C: " + std::to_string(c);
				if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
				{
					graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
				}
			}
			if (c != 0)
			{
				std::string destString;
				destString = "R :" + std::to_string(r)+" C: " + std::to_string((c - 1));
				if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
				{
					graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
				}
			}
			if (r < 15)
			{
				std::string destString;
				destString = "R :" + std::to_string((r + 1))+" C: " + std::to_string(c);
				if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
				{
					graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
				}
			}
			if (c < 15)
			{
				std::string destString;
				destString = "R :" + std::to_string(r) +" C: " + std::to_string((c + 1));
				if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
				{
					graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
				}
			}
		}
	}

	//timer start
	clock.restart();

	RunAStar(graph);

	//timerEnd
	Outputter output;

	output.firstLog();
	output.LogData(STARTPOINT, ENDPOINT, timeTaken , openedNodes->size());
	output.logNewLine();
}

CoreLoop::~CoreLoop()
{

}

void CoreLoop::Loop()
{
	Update();
	Draw();
}

void CoreLoop::Update()
{
	if (displayPath = true)
	{
		if (displayTimer > 20)
		{
			if (openedNodesIndex < openedNodes->size())
			{
				if (STARTPOINT != openedNodes->at(openedNodesIndex))
				{
					display.ChangeTile(openedNodes->at(openedNodesIndex), "Yellow");
				}
				openedNodesIndex++;
			}
			else
			{
				for (int i = thepath.size() - 1; i > -1; i--)
				{
					if (STARTPOINT != graph.GetIndex(thepath.at(i)->id) && ENDPOINT != graph.GetIndex(thepath.at(i)->id))
					{
						display.ChangeTile(graph.GetIndex(thepath.at(i)->id), "Green");
					}
					
				}
				displayPath = false;
			}
			displayTimer = 0;

		}
		else
		{
			displayTimer++;
		}

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		isRunning = false;
	}
}

void CoreLoop::Draw()
{
	window->clear();

	display.Render(window);

	window->display();
}
