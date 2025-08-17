#pragma once
#include <dirent.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>
#include <fstream>
#include <sstream>

class HttpRequest
{
    int socket=0;
    std::string method;
    std::string path;
    std::string version;
    std::string response;
public:

    void print() const;
    std::string getMethod();
    std::string getPath();
    std::string getVersion();

    void setRequestInfo(int server_fd,struct sockaddr * address,socklen_t * addrlen);

    void writeResponse() const;

    //HTTP METHOD HANDLERS
    void decideHandler();

    void requestHandlerGET();
    void requestHandlerUNKNOWN();
};
