#include <chrono>
#include <boost/tokenizer.hpp>
#include "boost/date_time/posix_time/posix_time.hpp" 
#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>
#include <boost/date_time.hpp>
#include "CSVFile.h"
#include "TransmitSettings.h"
#include <boost/algorithm/string/split.hpp>

namespace {
	const boost::regex linesregx("\\r\\n|\\n\\r|\\n|\\r");
	const boost::regex fieldsregx(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");
}
CSVFile::CSVFile(){
}

void CSVFile::open()
{
	auto now = std::chrono::system_clock::now().time_since_epoch().count();
	path = std::filesystem::current_path();
	path /= (std::string("tmp_") +  std::to_string(now));
	out.open(path);
}

int CSVFile::write(const char* data, int length)
{
	if(out.is_open()){
		out.write(data, length);
		fileSize -= length;
		if (fileSize == 0)
			out.close();
	}
	return fileSize;
}

void CSVFile::setFileSize(int fileSize)
{
	this->fileSize = fileSize;
}

void CSVFile::inspectFile()
{
	std::ifstream input(path);
	if (input.is_open()) 
	{
		boost::posix_time::ptime maxTime;
		double division = 0.0;

		for (std::string line; std::getline(input, line); )
		{
			++recodsCount;
			std::cout << " line" << std::endl;
			auto res = parse(line);
			std::string time = res[0];
			time.erase(std::remove_if(time.begin(), time.end(), [](char c) {
				return c == '"';
				}), time.end());
			Row dateTimeParts;
			Row dateParts;
			Row timeParts;
			boost::algorithm::split(dateTimeParts, time, boost::is_any_of(" "));
			boost::algorithm::split(dateParts, dateTimeParts[0], boost::is_any_of("."));
			boost::algorithm::split(timeParts, dateTimeParts[1], boost::is_any_of(":"));

			boost::posix_time::ptime readedTime(boost::gregorian::date(std::atoi(dateParts[2].c_str()),
				std::atoi(dateParts[1].c_str()), std::atoi(dateParts[0].c_str())),
				boost::posix_time::time_duration(std::atoi(timeParts[0].c_str()), std::atoi(timeParts[1].c_str()),
					std::atoi(timeParts[2].c_str())));
			double firstOperand = std::atof(res[1].c_str());
			double secondOperand = std::atof(res[2].c_str());
			try
			{
				double divisionBuff = firstOperand / secondOperand;
				if (maxTime > readedTime )
				{
					maxTime = readedTime;
					division = divisionBuff;
				}
				error = CSVFile::Errors::SECCESS;
			}
			catch (std::exception& e)
			{
				error = CSVFile::Errors::DIVISION_BY_ZERO;
			}
		}
		writeProtocol(maxTime, division);
	}
	input.close();
	std::filesystem::remove(path);
}

int CSVFile::getError()
{
	return error;
}

int CSVFile::getRecordsCount()
{
	return recodsCount;
}

Row CSVFile::parse(std::string &line)
{
	{
		std::vector<Row> result;


		boost::sregex_token_iterator ti(line.begin(), line.end(), fieldsregx, -1);
		boost::sregex_token_iterator end;

		Row row;
		while (ti != end) {
			std::string token = ti->str();
			++ti;
			row.push_back(token);
		}
		return row;
	}
}

void CSVFile::writeProtocol(boost::posix_time::ptime& maxTime, double divRes)
{
	std::filesystem::path path = std::filesystem::current_path() / "protocol.txt";
	std::ofstream protocol(path, std::ios_base::app);
	std::string res = "server : ";
	if (protocol.is_open())
	{
		if (error == CSVFile::Errors::UNDEFINED)
		{
			boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
			res += (boost::posix_time::to_simple_string(now) + "received file incorrect\n");
			protocol << res;
		}
		if (error == CSVFile::Errors::SECCESS)
		{
			res += (boost::posix_time::to_simple_string(maxTime) + " " + std::to_string(divRes)+ "\n");
			protocol << res;
		}
		if (error == CSVFile::Errors::DIVISION_BY_ZERO)
		{
			boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
			res += (boost::posix_time::to_simple_string(now) + "found division byzero\n");
			protocol << res;
		}
		protocol.close();
	}	
}

