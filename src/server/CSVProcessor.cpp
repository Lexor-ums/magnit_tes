#include <boost/asio/read.hpp>
#include "CSVProcessor.h"
#include "TransmitSettings.h"
#include <iostream>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>


CSVProcessor::CSVProcessor(tcp::socket socket)
    : socket(std::move(socket))
{
}

CSVProcessor::~CSVProcessor()
{
}

void CSVProcessor::start()
{
    auto self(shared_from_this());
    boost::asio::async_read(socket,
        boost::asio::buffer(buff.data(), TransmitSettings::HEDER_SIZE),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec )
            {
                csvFile.open();
                csvFile.setFileSize(decodeHaeder(buff.data()));
                readBody();
            }
        });
}

 int CSVProcessor::decodeHaeder(const char* data)
{
    char header[TransmitSettings::HEDER_SIZE + 1] = "";
    std::strncat(header, data, TransmitSettings::HEDER_SIZE);
    return static_cast<int>(std::atoi(header));
}

void CSVProcessor::readBody()
{
    auto self(shared_from_this());
    boost::asio::async_read_until(socket, streambuf, "\0",
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                int size = streambuf.size();
                streambuf.sgetn(buff.data(), size);
                if (csvFile.write(buff.data(), size) > 0)
                {
                    readBody();
                }
                else
                {
                    csvFile.inspectFile();
                    if (csvFile.getError() == CSVFile::Errors::SECCESS)
                        sendAnswer();
                }
            }
            else
            {
                std::cout << ec.message() << std::endl;
            }
        });
}

void CSVProcessor::sendAnswer()
{    
    std::cout << "records " << csvFile.getRecordsCount()<< " "<< std::to_string(csvFile.getRecordsCount())<<std::endl;
    std::string res = "recods processed " + std::to_string(csvFile.getRecordsCount()) + "\0";
    boost::asio::async_write(socket,
        boost::asio::buffer(res.c_str(), res.length()),
        [this](boost::system::error_code ec, std::size_t length)
        {

        });
}
