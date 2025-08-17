#include "HttpRequest.h"



void HttpRequest::print() const {
    std::cout << "Method: " << method << std::endl;
    std::cout << "Path: " << path << std::endl;
    std::cout << "HTTP Version: " << version << std::endl;
}

std::string HttpRequest::getMethod()
{
    return method;
}
std::string HttpRequest::getPath()
{
    return path;
}
std::string HttpRequest::getVersion()
{
    return version;
}

void HttpRequest::setRequestInfo(const int server_fd,struct sockaddr * address,socklen_t * addrlen)
{
    char buffer[40000];
    //std::cout << "Waiting for a connection…\n";
    socket = accept(server_fd, address,addrlen);

    if (socket < 0) {
        perror("Accept failed");
        return;
    }

    memset(buffer, 0, sizeof(buffer));
    read(socket, buffer, 40000);


    std::string request(buffer); // Convert the char array to a std::string
    const size_t lineEnd = request.find("\r\n"); // The request line ends with \r\n
    std::string requestLine = request.substr(0, lineEnd);

    const size_t firstSpace = requestLine.find(' ');
    const size_t secondSpace = requestLine.find(' ', firstSpace + 1);


    //Retrieve Method, Path, Version
    method = requestLine.substr(0, firstSpace);
    path = requestLine.substr(firstSpace + 1, secondSpace - firstSpace - 1);
    version = requestLine.substr(secondSpace + 1);
}

void HttpRequest::writeResponse() const {
    write(socket,response.c_str(),strlen(response.c_str()));
    close(socket);
}


void HttpRequest::decideHandler() {
    if( method == "GET"){
        requestHandlerGET();
    }
    else if (method == "POST")
    {
        // Parse body and handle submission
        std::cout << "POST request" << std::endl;
    }
    else if (method == "PUT")
    {
        // Simulate update
        std::cout << "PUT request" << std::endl;
    }
    else if (method == "DELETE")
    {
        // Simulate deletion
        std::cout << "DELETE request" << std::endl;
    }
    else
    {
        requestHandlerUNKNOWN();
    }
}

bool endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

void HttpRequest::requestHandlerGET()
{

    std::ifstream file("../html/index.html");
    if (!file.is_open())
    {
        std::cerr << "Failed to open index.html\n";
        response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "404 Not Found";
    }
    else
    {
        // Read file
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string body = ss.str();
        file.close();

        if (path=="/") {
            path = "../html/index.html";
        }
        else {
            path = "../html" + path;
        }

        // Determine Content-Type
        std::string contentType = "text/plain";
        if (endsWith(path, ".html")) contentType = "text/html";
        else if (endsWith(path, ".css")) contentType = "text/css";
        else if (endsWith(path, ".js")) contentType = "application/javascript";
        else if (endsWith(path, ".json")) contentType = "application/json";
        else if (endsWith(path, ".png")) contentType = "image/png";
        else if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) contentType = "image/jpeg";

        // Build and send response
         response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: " + contentType + "\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "\r\n" + body;
    }
}
void HttpRequest::requestHandlerUNKNOWN()
{
    std::cout << "Unsupported method: " << method << std::endl;

    response = "Unsupported request type"
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 18\r\n"
        "\r\n"
        "Method Not Allowed";
}
