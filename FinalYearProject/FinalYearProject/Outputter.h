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
	void LogData(int startPoint, int endPoint, float timeTaken, int numVisited);
	void logNewLine();

private:
	//name of file to be created by logger when logging
	std::string fileName;

	//variable used to output to csv file
	std::ofstream csvFile;
};

