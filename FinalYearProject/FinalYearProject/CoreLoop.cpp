#include "CoreLoop.h"

const int STARTPOINT = 17;
const int ENDPOINT = 149;



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
				if (IsObstacle(i) == true)
				{
					display.ChangeTile(i, "Grey");
				}
				else
				{
					display.ChangeTile(i, "White");
				}
			}
			display.ChangeTile(graph.nodes.at(m_Paths.at(pathIndex).startPointIndex)->m_displayIndex, "Orange");
			display.ChangeTile(graph.nodes.at(m_Paths.at(pathIndex).endPointIndex)->m_displayIndex, "Red");

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
				display.ChangeTile(graph.nodes.at(m_Paths.at(pathIndex-1).startPointIndex)->m_displayIndex, "White");
				display.ChangeTile(graph.nodes.at(m_Paths.at(pathIndex-1).endPointIndex)->m_displayIndex, "Orange");
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

	obstcles.push_back(std::pair<int, int>(0, 0));
	obstcles.push_back(std::pair<int, int>(1, 5));
	obstcles.push_back(std::pair<int, int>(1, 6));
	obstcles.push_back(std::pair<int, int>(1, 7));
	obstcles.push_back(std::pair<int, int>(2, 5));
	obstcles.push_back(std::pair<int, int>(2, 6));
	obstcles.push_back(std::pair<int, int>(2, 7));
	obstcles.push_back(std::pair<int, int>(3, 5));
	obstcles.push_back(std::pair<int, int>(3, 6));
	obstcles.push_back(std::pair<int, int>(3, 7));
	obstcles.push_back(std::pair<int, int>(4, 5));
	obstcles.push_back(std::pair<int, int>(4, 6));
	obstcles.push_back(std::pair<int, int>(4, 7));
	std::string temp;
	std::ifstream myfile;
	int index = 0;
	for (int r = 0; r < 16; r++)
	{
		for (int c = 0; c < 16; c++)
		{
			temp = "R :" + std::to_string(r) + " C: " + std::to_string(c);
			if (IsObstacle(c, r) == false)
			{
				graph.addNode(std::numeric_limits<int>::max() - 10000, temp,index);
			}
			else
			{
				obstcleIndex.push_back(index);
				display.ChangeTile(index, "Grey");
			}
			index++;
		}
	}
	int from, to, weight;

	for (int r = 0; r < 16; r++)
	{
		for (int c = 0; c < 16; c++)
		{
			std::string IDstring;
			IDstring = "R :" + std::to_string(r) + " C: " + std::to_string(c);
			if (IsObstacle(c, r) == true)
			{

			}
			else
			{
				if (r != 0)
				{
					std::string destString;
					destString = "R :" + std::to_string((r - 1)) + " C: " + std::to_string(c);
					if (IsObstacle(c, r-1) == false)
					{
						if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
						{
							graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
						}
					}
				}
				if (c != 0)
				{
					std::string destString;
					destString = "R :" + std::to_string(r) + " C: " + std::to_string((c - 1));
					if (IsObstacle(c - 1, r) == false)
					{
						if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
						{
							graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
						}
					}
				}
				if (r < 15)
				{
					std::string destString;
					destString = "R :" + std::to_string((r + 1)) + " C: " + std::to_string(c);
					if (IsObstacle(c, r +1) == false)
					{
						if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
						{
							graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
						}
					}
				}
				if (c < 15)
				{
					std::string destString;
					destString = "R :" + std::to_string(r) + " C: " + std::to_string((c + 1));
					if (IsObstacle(c + 1, r) == false)
					{
						if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
						{
							graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
						}
					}
				}
			}

		}
	}
}

void CoreLoop::RunFRAStar(Graph graph)
{
	int start = STARTPOINT;
	int end = ENDPOINT;

	m_Paths.push_back(AlgorithimPath(start, end));

	graph.gfraStar(graph.nodes.at(start), graph.nodes.at(end), m_Paths.at(m_Paths.size() - 1).path);

	OutputPathToConsole(m_Paths.at(m_Paths.size() - 1).path);
}

void CoreLoop::RunARAStar(Graph graph)
{
	int start = STARTPOINT;
	int end = ENDPOINT;

	//m_Paths.push_back(AlgorithimPath(start, end));
	int index = m_Paths.size();
	graph.araStar(graph.nodes.at(start), graph.nodes.at(end),start,end, m_Paths);
	for (int i = index; i < m_Paths.size(); i++)
	{
		OutputPathToConsole(m_Paths.at(i).path);
	}
	
}

bool CoreLoop::IsObstacle(int column, int row)
{
	for (int i = 0; i < obstcles.size(); i++)
	{
		if (obstcles.at(i).first == column && obstcles.at(i).second == row)
		{
			return true;
		}
	}
	return false;
}

bool CoreLoop::IsObstacle(int index)
{
	for (int i = 0; i < obstcleIndex.size(); i++)
	{
		if (obstcleIndex.at(i) == index)
		{
			return true;
		}
	}
	return false;
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
	//RunFRAStar(graph);
	//RunARAStar(graph);

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
