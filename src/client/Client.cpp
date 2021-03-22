#include "Client.h"
#include "boost/asio.hpp"
#include <boost/thread.hpp>
#include <iostream>
#include <server/CSVProcessor.h>
#include <boost/asio/read_until.hpp>


static boost::mutex debug_mutex;

Client::Client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
    socket(io_context)
{
    connect(endpoints);
}

void Client::transfer(std::filesystem::path& path)
{
    input.open(path);
    if (input.is_open()) {        
        int sizef = std::filesystem::file_size(path);
        char header[TransmitSettings::HEDER_SIZE + 1] = "";
        std::sprintf(header, "%4d", sizef);
        std::memcpy(buff.data(), header, TransmitSettings::HEDER_SIZE);

        boost::asio::async_write(socket,
            boost::asio::buffer(buff.data(), TransmitSettings::HEDER_SIZE),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    sendFile();
                }
                else
                {
                    socket.close();
                }
            });
    }
    else {
        std::cout << "cant open " << path << std::endl;
    }
}


void Client::connect(const tcp::resolver::results_type& endpoints)
{
    std::cout << "try connect" << std::endl;
    boost::asio::async_connect(socket, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint)
        {
            if (!ec)
            {
                readAnswer();
            }
            else
            {
                {
                    boost::mutex::scoped_lock lk(debug_mutex);
                }

            }
        });
}

void Client::readAnswer()
{
    boost::asio::async_read_until(socket, streambuf, "\0",
        [this](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::filesystem::path path = std::filesystem::current_path() / "protocol.txt";
                std::ofstream protocol(path, std::ios_base::app);
                int size = streambuf.size();
                std::string res = "client : ";
                streambuf.sgetn(buff.data(), size);
                res.append(buff.data(), size);
                if (protocol.is_open()) 
                {
                   protocol << res;
                   protocol.close();
                }                    
            }
            else
            {
                std::cout << ec.message() << std::endl;
            }
        });
}

void Client::sendFile()
{
    if (!input.eof()) {
        input.read(buff.data(), static_cast<std::streamsize>(buff.size() - 1));
        buff[input.gcount() + 1] = '\0';
        boost::asio::async_write(socket,
            boost::asio::buffer(buff.data(), input.gcount() + 1),
            [this](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    {
                        boost::mutex::scoped_lock lk(debug_mutex);
                        std::cout << "Send " << input.gcount() << "bytes, total: "
                            << input.tellg() << " bytes.\n" << length << std::endl;;
                    }
                    sendFile();
                }
                else
                {
                    {
                        boost::mutex::scoped_lock lk(debug_mutex);
                        std::cout << "Send error "<< ec.message()<<std::endl;
                    }
                    socket.close();
                }
            });
    }
}
