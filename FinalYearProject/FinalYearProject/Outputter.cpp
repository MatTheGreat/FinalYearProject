#include "Outputter.h"

Outputter::Outputter()
{
	isFirstLog = true;
}

Outputter::~Outputter()
{

}

void Outputter::firstLog()
{
	fileName = "Output.csv";
	csvFile.open(fileName);
	//Sets up first row
	csvFile << "Start Point,End Point,Time Taken(ms),Number of Nodes Visited";
	csvFile.close();

	logNewLine();
}

void Outputter::LogData(int startPoint, int endPoint, float timeTaken, int numVisited)
{
	//opens the file at the end of the file
	csvFile.open(fileName, std::ios::app);
	std::string output = std::to_string(startPoint);
	csvFile << output;
	csvFile << ",";
	output = std::to_string(endPoint);
	csvFile << output;
	csvFile << ",";
	output = std::to_string(timeTaken);
	csvFile << output;
	csvFile << ",";
	output = std::to_string(numVisited);
	csvFile << output;
	csvFile.close();
	logNewLine();
}



void Outputter::logNewLine()
{
	//opens the file at the end of the file
	csvFile.open(fileName, std::ios::app);
	csvFile << "\n";
	csvFile.close();
}



