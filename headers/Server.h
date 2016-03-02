#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H

#include "../headers/common.h"
#include "../headers/Connection.h"

class Server {

public:
    Server(ba::io_service& io_service, const std::string& host, short port, const std::string& rootDir);

    void handle_accept(Connection* connection, const bs::error_code& error);
private:
    void accept();

    std::string rootDir;
    ba::io_service& io_service;
    tcp::acceptor acceptor;
};

#endif //HTTP_SERVER_SERVER_H
