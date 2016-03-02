#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <unistd.h>
#include <cstdlib>
#include <boost/bind.hpp>

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
    daemonize();

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

    boost::asio::io_service service;
    Server server(service, host, port, rootDir);

    std::vector<boost::shared_ptr<boost::thread>> threads;
    for (std::size_t i = 0; i < 4; ++i)
    {
      boost::shared_ptr<boost::thread> thread(new boost::thread(
              boost::bind(&boost::asio::io_service::run, &service)));
      threads.push_back(thread);
    }

    for (std::size_t i = 0; i < threads.size(); ++i) {
      threads[i]->join();
    }

    return EXIT_SUCCESS;
}
