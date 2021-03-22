#include "Server.h"
#include "CSVProcessor.h"
#include <iostream>

Server::Server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
{
    doAccept();
}

void Server::doAccept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code errorCode, tcp::socket socket)
        {
            if (!errorCode)
            {
                std::make_shared<CSVProcessor>(std::move(socket))->start();
            }
            else
            {
                std::cout << "receive error" << errorCode.message() << std::endl;
            }

            doAccept();
        });
}