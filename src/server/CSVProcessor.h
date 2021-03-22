#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include "CSVFile.h"
#include <TransmitSettings.h>
#include <boost/asio/streambuf.hpp>
using boost::asio::ip::tcp;

class CSVProcessor : public std::enable_shared_from_this<CSVProcessor>
{
public:
	CSVProcessor(tcp::socket socket);
	~CSVProcessor();
	void start();
private:
	int decodeHaeder(const char* data);
	void readBody();
	void sendAnswer();
	tcp::socket socket;
	CSVFile csvFile;
	std::array<char, TransmitSettings::BUFFER_SIZE> buff;
	boost::asio::streambuf streambuf;

};

