#include "Outputter.h"

Outputter::Outputter()
{
	isFirstLog = true;
	fileName = "";
	csvFile = std::ofstream();
}

Outputter::~Outputter()
{

}

void Outputter::firstLog()
{
	fileName = "Output.csv";
	csvFile.open(fileName);
	//Sets up first row
	csvFile << "Alogrithim, Start Point,End Point,Time Taken(ms),Number of Nodes Visited , E Value";
	csvFile.close();

	logNewLine();
}

void Outputter::LogData(std::string algorithim,int startPoint, int endPoint, float timeTaken, int numVisited , int eValue)
{
	//opens the file at the end of the file
	csvFile.open(fileName, std::ios::app);
	std::string output = algorithim;
	csvFile << output;
	csvFile << ",";
	output = std::to_string(startPoint);
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
	csvFile << ",";
	if (eValue == 0)
	{
		output = "N/A";
	}
	else
	{
		output = std::to_string(eValue);
	}
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



