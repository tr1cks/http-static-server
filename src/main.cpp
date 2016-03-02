#include <iostream>
#include <boost/asio.hpp>
#include <unistd.h>
#include <cstdlib>

#include "../headers/Server.h"

namespace ba = boost::asio;
using boost::asio::ip::tcp;

void daemonize() {
    if(fork()) {
        exit(EXIT_SUCCESS);
    }

    setsid();
    chdir("/");
    umask(0);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char** argv)
{
//    daemonize();

    std::string host;
    short port;
    std::string rootDir;

    int opt = getopt(argc, argv, "h:p:d:");
    while(opt != -1) {
        switch(opt) {
            case 'h':
                host = optarg;
                break;
            case 'p':
                port = std::atoi(optarg);
                break;
            case 'd':
                rootDir = optarg;
                break;
            default:
                break;
        }
        opt = getopt(argc, argv, "h:p:d:");
    }

    if(rootDir.back() != '/') {
        rootDir += '/';
    }

    boost::asio::io_service io_service;
    Server server(io_service, host, port, rootDir);
    io_service.run();

    return EXIT_SUCCESS;
}
