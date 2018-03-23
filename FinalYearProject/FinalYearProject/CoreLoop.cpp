#include "CoreLoop.h"

const int STARTPOINT = 17;
const int ENDPOINT = 155;



void CoreLoop::RunAStar(Graph graph)
{
	int start = STARTPOINT;
	int end = ENDPOINT;
	output->firstLog();

	m_Paths.push_back(AlgorithimPath(start, end));

	// Now traverse the graph.
	clock.restart();

	graph.aStar(graph.nodes.at(start), graph.nodes.at(end), m_Paths.at(m_Paths.size() - 1).path, m_Paths.at(m_Paths.size() - 1).opendedNodes);

	timeTaken = clock.getElapsedTime().asMilliseconds();

	OutputPathToConsole(m_Paths.at(m_Paths.size() - 1).path);
	OutputToCSVFile(m_Paths.at(m_Paths.size() - 1).startPointIndex, m_Paths.at(m_Paths.size() - 1).endPointIndex, timeTaken, m_Paths.at(m_Paths.size() - 1).opendedNodes->size());

	Node * newStart = m_Paths.at(m_Paths.size() - 1).path.at(m_Paths.at(m_Paths.size() - 1).path.size() - 2);
	start = graph.GetIndex(newStart->id);

	while (start != end)
	{
		m_Paths.push_back(AlgorithimPath(start, end));

		clock.restart();

		graph.aStar(graph.nodes.at(start), graph.nodes.at(end), m_Paths.at(m_Paths.size() - 1).path, m_Paths.at(m_Paths.size() - 1).opendedNodes);
		timeTaken = clock.getElapsedTime().asMilliseconds();

		std::cout << "START : " << start << std::endl;
		std::cout << "END : " << end << std::endl;

		OutputPathToConsole(m_Paths.at(m_Paths.size() - 1).path);
		OutputToCSVFile(m_Paths.at(m_Paths.size() - 1).startPointIndex, m_Paths.at(m_Paths.size() - 1).endPointIndex, timeTaken, m_Paths.at(m_Paths.size() - 1).opendedNodes->size());

		Node * newStart = m_Paths.at(m_Paths.size() - 1).path.at(m_Paths.at(m_Paths.size() - 1).path.size() - 2);
		start = graph.GetIndex(newStart->id);
	}

	displayPath = true;
}

void CoreLoop::OutputPathToConsole(std::vector<Node *> path)
{
	for (int i = path.size() - 1; i > -1; i--)
	{
		std::cout << "Location : " << path.at(i)->id << " Cost : " << path.at(i)->weight << " h(n) : " << path.at(i)->m_estDistToDest << std::endl;
		if (path.at(i) == graph.nodes.at(STARTPOINT))
		{
			std::cout << std::endl;
		}
	}
}

void CoreLoop::OutputToCSVFile(int startPoint, int endPoint, int timeTaken, int nodesOpened)
{
	output->LogData(startPoint, endPoint, timeTaken, nodesOpened);
}

void CoreLoop::DisplayPath()
{
	if (pathIndex < m_Paths.size())
	{
		if (m_Paths.at(pathIndex).displayStarted == false)
		{
			for (int i = 0; i < display.getTiles()->size(); i++)
			{
				display.ChangeTile(i, "White");
			}
			display.ChangeTile(m_Paths.at(pathIndex).startPointIndex, "Orange");
			display.ChangeTile(m_Paths.at(pathIndex).endPointIndex, "Red");

			m_Paths.at(pathIndex).displayStarted = true;
		}
		if (displayTimer > 5)
		{
			if (m_Paths.at(pathIndex).DisplayPath(pathIndex, graph, display) == true)
			{
				pathTimer = 0;
				pathIndex++;
				displayPath = false;
			}
			if (pathIndex == m_Paths.size())
			{
				display.ChangeTile(m_Paths.at(pathIndex-1).startPointIndex, "White");
				display.ChangeTile(m_Paths.at(pathIndex-1).endPointIndex, "Orange");
			}
			displayTimer = 0;
		}
		else
		{
			displayTimer++;
		}
	}
	else
	{
		displayPath = false;
	}
}

void CoreLoop::GenerateMap()
{
	std::string temp;
	std::ifstream myfile;
	for (int c = 0; c < 16; c++)
	{
		for (int r = 0; r < 16; r++)
		{
			temp = "R :" + std::to_string(r) + " C: " + std::to_string(c);
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
				destString = "R :" + std::to_string((r - 1)) + " C: " + std::to_string(c);
				if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
				{
					graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
				}
			}
			if (c != 0)
			{
				std::string destString;
				destString = "R :" + std::to_string(r) + " C: " + std::to_string((c - 1));
				if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
				{
					graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
				}
			}
			if (r < 15)
			{
				std::string destString;
				destString = "R :" + std::to_string((r + 1)) + " C: " + std::to_string(c);
				if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
				{
					graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
				}
			}
			if (c < 15)
			{
				std::string destString;
				destString = "R :" + std::to_string(r) + " C: " + std::to_string((c + 1));
				if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
				{
					graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
				}
			}
		}
	}
}

CoreLoop::CoreLoop()
{
	isRunning = true;
	window = new sf::RenderWindow(sf::VideoMode(768, 768), "Final Year Project");
	window->setFramerateLimit(60.0f);

	graph = Graph();

	displayPath = false;
	displayTimer = 0;
	pathIndex = 0;
	pathTimer = 0;

	display = MapDisplay();
	timeTaken = 0;

	output = new Outputter();
	
	GenerateMap();


	RunAStar(graph);

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
	if (pathTimer > 120)
	{
		displayPath = true;
	}
	else
	{
		pathTimer++;
	}

	if (displayPath == true)
	{
		DisplayPath();
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
