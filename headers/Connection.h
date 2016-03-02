#ifndef HTTP_SERVER_CONNECTION_H
#define HTTP_SERVER_CONNECTION_H

#include "../headers/common.h"
#include "../headers/Request.h"
#include "../headers/Response.h"

class Connection {

public:
    Connection(ba::io_service& io_service, const std::string& rootDir);

    void start();

    tcp::socket& getSocket();

    void handleReadRequestLine(const bs::error_code& error);

    void handleReadHeaders(const bs::error_code &error);

    void handleResponseSent(const bs::error_code &error);

private:
    Response processRequest(const Request &request);

    std::string uriToPath(const std::string &uri);

    const std::string rootDir;
    tcp::socket socket;
    ba::streambuf data;
    Request request;
};

#endif //HTTP_SERVER_CONNECTION_H
