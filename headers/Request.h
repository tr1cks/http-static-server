#ifndef HTTP_SERVER_REQUEST_H
#define HTTP_SERVER_REQUEST_H

class Request {

public:
    std::string method;
    std::string uri;
    std::string httpVersion;
    std::map<std::string, std::string> headers;
};

#endif //HTTP_SERVER_REQUEST_H
