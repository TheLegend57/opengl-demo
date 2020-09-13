#include "Logger.h"

Logger::Logger(){
	
}

Logger::~Logger(){
	StopLogger();
}

void Logger::SetLogFilePath(std::string path){
	this->LogFilePath = path;
}

void Logger::StartLogger(){
	this->LogFile.open(this->LogFilePath);
	this->LoggerStarted = this->LogFile.is_open();
}

bool Logger::IsLoggerStarted(){
	return this->LoggerStarted;
}

void Logger::StopLogger(){
	if (this->LogFile.is_open())
		this->LogFile.close();
	this->LoggerStarted = false;
}

std::string Logger::GetDateStamp(){
	std::time_t CurrentTime = time(NULL);
	std::tm *TimeStruct = std::localtime(&CurrentTime);
	std::string TimeDateStamp = "[";
	if (TimeStruct->tm_mday < 10)
		TimeDateStamp += "0";
	TimeDateStamp += std::to_string(TimeStruct->tm_mday) + ".";
	if (TimeStruct->tm_mon + 1 < 10)
		TimeDateStamp += "0";
	TimeDateStamp += std::to_string(TimeStruct->tm_mon + 1)
	+ "." + std::to_string(TimeStruct->tm_year + 1900) + " - ";
	if (TimeStruct->tm_hour < 10)
		TimeDateStamp += "0";
	TimeDateStamp += std::to_string(TimeStruct->tm_hour) + ":";
	if (TimeStruct->tm_min < 10)
		TimeDateStamp += "0";
	TimeDateStamp += std::to_string(TimeStruct->tm_min) + ":";
	if (TimeStruct->tm_sec < 10)
		TimeDateStamp += "0";
	TimeDateStamp += std::to_string(TimeStruct->tm_sec) + "]: ";
	return TimeDateStamp;
}

void Logger::AddToLog(std::string message){
	if (this->LoggerStarted){
		std::string Stamp = this->GetDateStamp();
		this->LogFile << Stamp << message << std::endl;
	}
}

void Logger::AddToLog(std::string message1, std::string message2){
	if (this->LoggerStarted){
		std::string Stamp = this->GetDateStamp();
		this->LogFile << Stamp << message1 << std::endl;
		this->LogFile << message2 << std::endl;
	}
}