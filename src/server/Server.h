#pragma once
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

class Server
{
public:
    Server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint);        

private:
    void doAccept();
    tcp::acceptor acceptor_;
};
