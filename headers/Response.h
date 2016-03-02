#ifndef HTTP_SERVER_RESPONSE_H
#define HTTP_SERVER_RESPONSE_H

#include <ostream>

class Response {

public:
    std::string httpVersion;
    unsigned int statusCode;
    std::string statusMessage;

    std::map<std::string, std::string> headers;
    std::vector<char> body;
};

inline std::ostream& operator<<(std::ostream& os, const Response& response) {
    os << response.httpVersion << " "
       << response.statusCode << " "
       << response.statusMessage << "\r\n";

    for(const auto& header : response.headers) {
        os << header.first << ": " << header.second << "\r\n";
    }

    os << "\r\n";
    std::copy(response.body.begin(), response.body.end(), std::ostream_iterator<char>(os, ""));

    return os;
}

#endif //HTTP_SERVER_RESPONSE_H
