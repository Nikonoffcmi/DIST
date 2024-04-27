#include "HTTPServer.hpp"

int main(int argc, char *argv[])
{
    HttpServer::UPtr server = std::make_unique<HttpServer>();

    server->Start(7777);

    return 0;
}