#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>
#include <cstring>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include "Server.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
       if (argc < 2)
       {
           std::cerr << "Usage: chat_server <port> [<port> ...]\n";
           return 1;
       }

        boost::asio::io_context io_context;
        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));

        Server server(io_context, endpoint);

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}