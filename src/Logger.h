#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
class Logger{
public:
	Logger();
	~Logger();
	
	void SetLogFilePath(std::string);
		
	void StartLogger();
	bool IsLoggerStarted();
	void AddToLog(std::string);
	void AddToLog(std::string, std::string);
	void StopLogger();
private:
	std::string LogFilePath = "";
	std::ofstream LogFile;
	std::string GetDateStamp();
	bool LoggerStarted = false;
};
#endif /* LOGGER_H */