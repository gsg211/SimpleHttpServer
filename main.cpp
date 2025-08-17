#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>
#include "HttpRequest.h"


[[noreturn]] int main()
{
    int server_fd=0;

    struct sockaddr_in address{}; //holds server address info

    int opt=1;

    int addrlen = sizeof(address);

    server_fd=socket(AF_INET, SOCK_STREAM,0);

    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;

    // Bind to port 8080
    address.sin_port = htons(8080);

    if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }



    listen(server_fd, 3);

    while (true)
    {
        HttpRequest request;
        request.setRequestInfo(server_fd, reinterpret_cast<struct sockaddr *>(&address),reinterpret_cast<socklen_t *>(&addrlen));
        request.print();
        request.decideHandler();
        request.writeResponse();

    }
}