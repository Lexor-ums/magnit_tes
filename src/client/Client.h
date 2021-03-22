#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <filesystem>
#include <fstream>
#include "TransmitSettings.h"

using boost::asio::ip::tcp;

class Client {
public:
	Client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints);
	/*!
	*	init file transfer to server
	*  @param path - path to file
	!*/
	void transfer(std::filesystem::path &path);
private:
	/*!
	*	connecting to server
	*  @param endpionts - connection configere
	!*/
	void connect(const tcp::resolver::results_type& endpoints);
	/*!
	* waiting from server file scan results
	!*/
    void readAnswer();
	/*!
	*	starts drasfer data
	!*/
	void sendFile();
	boost::asio::io_context& io_context_;
	tcp::socket socket;
	std::ifstream input;
	std::array<char, TransmitSettings::BUFFER_SIZE> buff;
	boost::asio::streambuf streambuf;	
};
