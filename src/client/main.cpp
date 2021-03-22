#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>
#include <boost/thread.hpp>
#include <cstring>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <filesystem>
#include "Client.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: chat_client <host> <port> <file name>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        std::filesystem::path path = std::filesystem::current_path() / argv[3];
        Client client(io_context, endpoints);
        boost::this_thread::sleep_for(boost::chrono::milliseconds(300));

        std::thread t([&io_context]() { io_context.run(); });
        client.transfer(path);

        t.join();

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}