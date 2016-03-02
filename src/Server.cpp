#include "../headers/Server.h"

#include <boost/bind.hpp>

Server::Server(ba::io_service& io_service, const std::string& host, short port, const std::string& rootDir)
    : rootDir(rootDir),
      io_service(io_service),
      acceptor(io_service, tcp::endpoint(ba::ip::address::from_string(host), port))
{
    accept();
}

void Server::handle_accept(Connection* connection, const bs::error_code& error)
{
    if(!error) {
        connection->start();
    } else {
        delete connection;
    }

    accept();
}

void Server::accept()
{
    Connection* connection = new Connection(io_service, rootDir);
    acceptor.async_accept(connection->getSocket(),
                          boost::bind(&Server::handle_accept, this,
                                      connection, ba::placeholders::error));

}
