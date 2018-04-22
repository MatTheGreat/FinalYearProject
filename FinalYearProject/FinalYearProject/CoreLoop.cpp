#include "CoreLoop.h"



void CoreLoop::RunAStar(Graph graph)
{
	int start = STARTPOINT;
	int end = ENDPOINT;
	

	m_Paths.push_back(AlgorithimPath(start, end,"A*"));

	// Now traverse the graph.
	clock.restart();

	graph.aStar(graph.nodes.at(start), graph.nodes.at(end), m_Paths.at(m_Paths.size() - 1).path, m_Paths.at(m_Paths.size() - 1).opendedNodes);

	timeTaken = clock.getElapsedTime().asMilliseconds();

	OutputPathToConsole(m_Paths.at(m_Paths.size() - 1).path);
	OutputToCSVFile("A*",graph.nodes.at(m_Paths.at(m_Paths.size() - 1).startPointIndex)->id, graph.nodes.at(m_Paths.at(m_Paths.size() - 1).endPointIndex)->id, timeTaken, m_Paths.at(m_Paths.size() - 1).opendedNodes->size(),0, m_Paths.at(m_Paths.size() - 1).path.size());

	Node * newStart = m_Paths.at(m_Paths.size() - 1).path.at(m_Paths.at(m_Paths.size() - 1).path.size() - 2);
	start = graph.GetIndex(newStart->id);

	while (start != end)
	{
		m_Paths.push_back(AlgorithimPath(start, end,"A*"));

		clock.restart();

		graph.aStar(graph.nodes.at(start), graph.nodes.at(end), m_Paths.at(m_Paths.size() - 1).path, m_Paths.at(m_Paths.size() - 1).opendedNodes);
		timeTaken = clock.getElapsedTime().asMilliseconds();

		std::cout << "START : " << start << std::endl;
		std::cout << "END : " << end << std::endl;

		OutputPathToConsole(m_Paths.at(m_Paths.size() - 1).path);
		OutputToCSVFile("A*", graph.nodes.at(m_Paths.at(m_Paths.size() - 1).startPointIndex)->id, graph.nodes.at(m_Paths.at(m_Paths.size() - 1).endPointIndex)->id, timeTaken, m_Paths.at(m_Paths.size() - 1).opendedNodes->size(),0, m_Paths.at(m_Paths.size() - 1).path.size());

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

void CoreLoop::OutputToCSVFile(std::string algorithim,std::string startPoint, std::string endPoint, int timeTaken, int nodesOpened, int eValue, int pathLength)
{
	output->LogData(algorithim, startPoint, endPoint, timeTaken, nodesOpened, eValue,pathLength);
}

void CoreLoop::DisplayPath()
{
	if (pathIndex < m_Paths.size())
	{
		if (m_Paths.at(pathIndex).displayStarted == false)
		{
			std::string eTxt;
			if (m_Paths.at(pathIndex).eValue != 0)
			{
				eTxt = "E:" + std::to_string(m_Paths.at(pathIndex).eValue);
			}
			else
			{
				eTxt = "";
			}
			display.UpdateText(m_Paths.at(pathIndex).m_type, eTxt);
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
		if (displayTimer > 0)
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
	obstcles.push_back(std::pair<int, int>(1, 5));
	obstcles.push_back(std::pair<int, int>(1, 6));
	obstcles.push_back(std::pair<int, int>(1, 7));
	obstcles.push_back(std::pair<int, int>(2, 5));
	obstcles.push_back(std::pair<int, int>(2, 6));
	obstcles.push_back(std::pair<int, int>(2, 7));
	obstcles.push_back(std::pair<int, int>(4, 4));
	obstcles.push_back(std::pair<int, int>(4, 5));
	obstcles.push_back(std::pair<int, int>(4, 6));
	obstcles.push_back(std::pair<int, int>(4, 7));
	obstcles.push_back(std::pair<int, int>(4, 8));
	obstcles.push_back(std::pair<int, int>(8, 6));
	obstcles.push_back(std::pair<int, int>(8, 7));
	obstcles.push_back(std::pair<int, int>(6, 2));
	obstcles.push_back(std::pair<int, int>(6, 5));
	obstcles.push_back(std::pair<int, int>(9, 6));

	obstcles.push_back(std::pair<int, int>(14, 14));
	obstcles.push_back(std::pair<int, int>(14, 15));
	obstcles.push_back(std::pair<int, int>(15, 16));
	obstcles.push_back(std::pair<int, int>(15, 17));

	obstcles.push_back(std::pair<int, int>(21, 25));
	obstcles.push_back(std::pair<int, int>(22, 25));
	obstcles.push_back(std::pair<int, int>(21, 24));

	obstcles.push_back(std::pair<int, int>(31, 16));
	obstcles.push_back(std::pair<int, int>(31, 17));

	obstcles.push_back(std::pair<int, int>(32, 16));
	obstcles.push_back(std::pair<int, int>(32, 17));
	obstcles.push_back(std::pair<int, int>(33, 16));

	obstcles.push_back(std::pair<int, int>(15, 37));
	obstcles.push_back(std::pair<int, int>(16, 37));
	obstcles.push_back(std::pair<int, int>(17, 37));
	obstcles.push_back(std::pair<int, int>(15, 38));
	obstcles.push_back(std::pair<int, int>(16, 38));
	obstcles.push_back(std::pair<int, int>(17, 38));

	obstcles.push_back(std::pair<int, int>(28, 45));
	obstcles.push_back(std::pair<int, int>(27, 45));
	obstcles.push_back(std::pair<int, int>(28, 46));

	obstcles.push_back(std::pair<int, int>(21, 26));
	obstcles.push_back(std::pair<int, int>(22, 25));
	obstcles.push_back(std::pair<int, int>(21, 25));
	obstcles.push_back(std::pair<int, int>(23, 26));
	obstcles.push_back(std::pair<int, int>(24, 25));
	obstcles.push_back(std::pair<int, int>(25, 26));
	obstcles.push_back(std::pair<int, int>(25, 27));

	obstcles.push_back(std::pair<int, int>(42, 34));
	obstcles.push_back(std::pair<int, int>(45, 25));
	obstcles.push_back(std::pair<int, int>(49, 16));
	obstcles.push_back(std::pair<int, int>(41, 47));
	obstcles.push_back(std::pair<int, int>(43, 8));
	obstcles.push_back(std::pair<int, int>(38, 6));
	obstcles.push_back(std::pair<int, int>(18, 37));

	obstcles.push_back(std::pair<int, int>(36, 12));
	obstcles.push_back(std::pair<int, int>(36, 15));

	obstcles.push_back(std::pair<int, int>(39, 16));


	std::string temp;
	std::ifstream myfile;
	int index = 0;
	for (int r = 0; r < 50; r++)
	{
		for (int c = 0; c < 50; c++)
		{
			temp = "C :" + std::to_string(r) + " R: " + std::to_string(c);
			if (IsObstacle(c, r) == false)
			{
				graph.addNode(std::numeric_limits<int>::max() - 10000, temp,index,std::pair<int,int>(c*100,r*100));
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

	for (int r = 0; r < 50; r++)
	{
		for (int c = 0; c < 50; c++)
		{
			std::string IDstring;
			IDstring = "C :" + std::to_string(r) + " R: " + std::to_string(c);
			if (IsObstacle(c, r) == true)
			{

			}
			else
			{
				if (r != 0)
				{
					std::string destString;
					destString = "C :" + std::to_string((r - 1)) + " R: " + std::to_string(c);
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
					destString = "C :" + std::to_string(r) + " R: " + std::to_string((c - 1));
					if (IsObstacle(c - 1, r) == false)
					{
						if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
						{
							graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
						}
					}
				}
				if (r < 49)
				{
					std::string destString;
					destString = "C :" + std::to_string((r + 1)) + " R: " + std::to_string(c);
					if (IsObstacle(c, r +1) == false)
					{
						if (graph.getArc(graph.GetIndex(IDstring), graph.GetIndex(destString)) == 0)
						{
							graph.addArc(graph.GetIndex(IDstring), graph.GetIndex(destString), 100);
						}
					}
				}
				if (c < 49)
				{
					std::string destString;
					destString = "C :" + std::to_string(r) + " R: " + std::to_string((c + 1));
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

	int index = m_Paths.size();
	graph.fraStar(graph.nodes.at(start), graph.nodes.at(end), m_Paths);
	for (int i = index; i < m_Paths.size(); i++)
	{
		OutputPathToConsole(m_Paths.at(i).path);
		OutputToCSVFile("FRA*", graph.nodes.at(m_Paths.at(i).startPointIndex)->id, graph.nodes.at(m_Paths.at(i).endPointIndex)->id, m_Paths.at(i).timeTaken, m_Paths.at(i).opendedNodes->size(), 0, m_Paths.at(i).path.size());
	}


	displayPath = true;
}

void CoreLoop::RunGFRAStar(Graph graph)
{
	int start = STARTPOINT;
	int end = ENDPOINT;

	//m_Paths.push_back(AlgorithimPath(start, end));
	int index = m_Paths.size();
	graph.gfraStar(graph.nodes.at(start), graph.nodes.at(end), m_Paths);
	for (int i = index; i < m_Paths.size(); i++)
	{
		OutputPathToConsole(m_Paths.at(i).path);
		OutputToCSVFile("G-FRA*", graph.nodes.at(m_Paths.at(i).startPointIndex)->id, graph.nodes.at(m_Paths.at(i).endPointIndex)->id, m_Paths.at(i).timeTaken, m_Paths.at(i).opendedNodes->size(), 0, m_Paths.at(i).path.size());
	}
	

	displayPath = true;

}

void CoreLoop::RunIARAStar(Graph graph)
{
	int start = STARTPOINT;
	int end = ENDPOINT;

	m_Paths.push_back(AlgorithimPath(start, end,"I-ARA*"));
	int index = m_Paths.size();
	graph.iaraStar(graph.nodes.at(start), graph.nodes.at(end), m_Paths);
	for (int i = index; i < m_Paths.size(); i++)
	{
		std::cout << "New Path " << " e:" << m_Paths.at(i).eValue << std::endl;
		std::cout << "" << std::endl;
		OutputPathToConsole(m_Paths.at(i).path);
		OutputToCSVFile("I-ARA*", graph.nodes.at(m_Paths.at(i).startPointIndex)->id, graph.nodes.at(m_Paths.at(i).endPointIndex)->id, m_Paths.at(i).timeTaken, m_Paths.at(i).opendedNodes->size(), m_Paths.at(i).eValue, m_Paths.at(i).path.size());
		if (m_Paths.at(i).eValue == 1)
		{
			output->logNewLine();
		}
	}
	displayPath = true;
}

void CoreLoop::RunARAStar(Graph graph)
{
	int start = STARTPOINT;
	int end = ENDPOINT;

	clock.restart();

	graph.araStar(graph.nodes.at(start), graph.nodes.at(end), start, end, m_Paths);
	

	OutputPathToConsole(m_Paths.at(m_Paths.size() - 3).path);
	OutputToCSVFile("ARA*", graph.nodes.at(m_Paths.at(m_Paths.size() - 3).startPointIndex)->id, graph.nodes.at(m_Paths.at(m_Paths.size() - 3).endPointIndex)->id, m_Paths.at(m_Paths.size() - 3).timeTaken, m_Paths.at(m_Paths.size() - 3).opendedNodes->size(), m_Paths.at(m_Paths.size() - 3).eValue, m_Paths.at(m_Paths.size() - 3).path.size());

	OutputPathToConsole(m_Paths.at(m_Paths.size() - 2).path);
	OutputToCSVFile("ARA*", graph.nodes.at(m_Paths.at(m_Paths.size() - 2).startPointIndex)->id, graph.nodes.at(m_Paths.at(m_Paths.size() - 2).endPointIndex)->id, m_Paths.at(m_Paths.size() - 2).timeTaken, m_Paths.at(m_Paths.size() - 2).opendedNodes->size(), m_Paths.at(m_Paths.size() - 2).eValue, m_Paths.at(m_Paths.size() - 2).path.size());

	OutputPathToConsole(m_Paths.at(m_Paths.size() - 1).path);
	OutputToCSVFile("ARA*", graph.nodes.at(m_Paths.at(m_Paths.size() - 1).startPointIndex)->id, graph.nodes.at(m_Paths.at(m_Paths.size() - 1).endPointIndex)->id, m_Paths.at(m_Paths.size() - 1).timeTaken, m_Paths.at(m_Paths.size() - 1).opendedNodes->size(), m_Paths.at(m_Paths.size() - 1).eValue,m_Paths.at(m_Paths.size() - 1).path.size());

	while (start != end)
	{
		if (m_Paths.at(m_Paths.size() - 1).path.size() != 0)
		{
			Node * newStart = m_Paths.at(m_Paths.size() - 1).path.at(m_Paths.at(m_Paths.size() - 1).path.size() - 2);
			start = graph.GetIndex(newStart->id);
		}
		else
		{
			break;
		}

		clock.restart();

		graph.araStar(graph.nodes.at(start), graph.nodes.at(end), start, end, m_Paths);

		timeTaken = clock.getElapsedTime().asMilliseconds();

		std::cout << "START : " << start << std::endl;
		std::cout << "END : " << end << std::endl;

		OutputPathToConsole(m_Paths.at(m_Paths.size() - 3).path);
		OutputToCSVFile("ARA*", graph.nodes.at(m_Paths.at(m_Paths.size() - 3).startPointIndex)->id, graph.nodes.at(m_Paths.at(m_Paths.size() - 3).endPointIndex)->id, m_Paths.at(m_Paths.size() - 3).timeTaken, m_Paths.at(m_Paths.size() - 3).opendedNodes->size(), m_Paths.at(m_Paths.size() - 3).eValue, m_Paths.at(m_Paths.size() - 3).path.size());

		OutputPathToConsole(m_Paths.at(m_Paths.size() - 2).path);
		OutputToCSVFile("ARA*", graph.nodes.at(m_Paths.at(m_Paths.size() - 2).startPointIndex)->id, graph.nodes.at(m_Paths.at(m_Paths.size() - 2).endPointIndex)->id, m_Paths.at(m_Paths.size() - 2).timeTaken, m_Paths.at(m_Paths.size() - 2).opendedNodes->size(), m_Paths.at(m_Paths.size() - 2).eValue, m_Paths.at(m_Paths.size() - 2).path.size());

		OutputPathToConsole(m_Paths.at(m_Paths.size() - 1).path);
		OutputToCSVFile("ARA*", graph.nodes.at(m_Paths.at(m_Paths.size() - 1).startPointIndex)->id, graph.nodes.at(m_Paths.at(m_Paths.size() - 1).endPointIndex)->id, m_Paths.at(m_Paths.size() - 1).timeTaken, m_Paths.at(m_Paths.size() - 1).opendedNodes->size(), m_Paths.at(m_Paths.size() - 1).eValue, m_Paths.at(m_Paths.size() - 1).path.size());

		output->logNewLine();
	}

	displayPath = true;
	
}

bool CoreLoop::IsObstacle(int column, int row)
{
	for (int i = 0; i < obstcles.size(); i++)
	{
		if (obstcles.at(i).first == row && obstcles.at(i).second == column)
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
	window = new sf::RenderWindow(sf::VideoMode(800, 800), "Final Year Project");
	window->setFramerateLimit(60.0f);



	graph = Graph(&clock);


	displayPath = false;
	displayTimer = 0;
	pathIndex = 0;
	pathTimer = 0;

	display = MapDisplay();
	timeTaken = 0;

	output = new Outputter();
	
	GenerateMap();

	STARTPOINT = graph.GetIndex("C :1 R: 4");
	ENDPOINT = graph.GetIndex("C :35 R: 35");

	output->firstLog();

	RunAStar(graph);
	RunFRAStar(graph);
	RunGFRAStar(graph);
	RunARAStar(graph);
	RunIARAStar(graph);
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
