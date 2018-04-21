#pragma once
#include <string>
#include <iostream>
#include <fstream>

class Outputter
{
public:
	Outputter();
	~Outputter();

	bool isFirstLog;

	void firstLog();
	void LogData(std::string algorithim, int startPoint, int endPoint, float timeTaken, int numVisited, int eValue);
	void logNewLine();

private:
	//name of file to be created by logger when logging
	std::string fileName;

	//variable used to output to csv file
	std::ofstream csvFile;
};

