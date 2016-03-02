#include "../headers/Connection.h"

#include <boost/bind.hpp>
#include <fstream>

Connection::Connection(ba::io_service& io_service, const std::string &rootDir)
    : rootDir(rootDir), socket(io_service) {}

void Connection::start()
{
    ba::async_read_until(socket, data, "\r\n", boost::bind(&Connection::handleReadRequestLine, this,
                                                           ba::placeholders::error));
}

void Connection::handleReadRequestLine(const bs::error_code& error)
{
    if(error) {
        delete this;
        return;
    }

    std::istream is(&data);
    is >> request.method;
    is >> request.uri;
    std::getline(is, request.httpVersion);

    ba::async_read_until(socket, data, "\r\n\r\n", boost::bind(&Connection::handleReadHeaders, this,
                                                               ba::placeholders::error));
}

void Connection::handleReadHeaders(const bs::error_code& error)
{
    if(error) {
        delete this;
        return;
    }

    std::istream is(&data);
    std::string header;
    std::string::size_type idx;
    while(std::getline(is, header) && header != "\r") {
        idx = header.find(":");
        if(idx == std::string::npos) {
            continue;
        }

        request.headers[header.substr(0, idx)] = header.substr(idx + 1);
    }

    std::ostream os(&data);
    Response response = processRequest(request);
    os << response;

    ba::async_write(socket, data, boost::bind(&Connection::handleResponseSent, this,
                                              ba::placeholders::error()));
}

void Connection::handleResponseSent(const bs::error_code& error)
{
    delete this;
}

Response Connection::processRequest(const Request& request)
{
    Response response;
    response.httpVersion = "HTTP/1.0";

    std::string fullPath = rootDir + uriToPath(request.uri);
    std::ifstream is(fullPath, std::ios::binary);
    if(is) {
        response.statusCode = 200;
        response.statusMessage = "OK";
        //TODO: reserve space
        response.body.assign(std::istreambuf_iterator<char>(is),
                             std::istreambuf_iterator<char>());
    } else {
        response.statusCode = 404;
        response.statusMessage = "Not Found";
        std::string body = "Not Found";
        response.body.assign(body.begin(), body.end());
    }

    response.headers["Content-Type"] = "text/html";
    response.headers["Content-Length"] = std::to_string(response.body.size());
    response.headers["Connection"] = "close";

    return response;
}

std::string Connection::uriToPath(const std::string& uri)
{
    std::string::size_type begin = (uri[0] == '/' ? 1 : 0);
    std::string::size_type end = uri.find_last_of('?');
    if(end != std::string::npos) {
        end -= 1;
    }

    return uri.substr(begin, end);
}

tcp::socket& Connection::getSocket()
{
    return socket;
}
