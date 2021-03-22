#pragma once
#include <filesystem>
#include <fstream>
#include <boost/regex.hpp>

typedef std::vector<std::string> Row;

class CSVFile {	
public:
	enum Errors {
		UNDEFINED,
		SECCESS ,
		DIVISION_BY_ZERO
	};
	CSVFile();
	/*!
	* create received file
	!*/
	void open();
	/*!
	* write some data to file
	* @param data - poinre to file data
	* @param length - data length
	!*/
	int write(const char* data, int length);
	/*!
	* set received file sixe
	* @param fileSize - size of file in bytes
	!*/
	void setFileSize(int fileSize);
	/*!
	* scan received file for recodcs and dividing
	!*/
	void inspectFile();
	/*!
	* return scan file erros
	!*/ 
	int getError();
	/*!
	* return recors in file
	!*/
	int getRecordsCount();

private:
	Row parse(std::string& line);
	void writeProtocol(boost::posix_time::ptime& maxTime, double divRes);
	int fileSize{ 0 };
	int recodsCount{ 0 };
	std::filesystem::path path;
	std::ofstream out;
	int error{ CSVFile::Errors::UNDEFINED };
};