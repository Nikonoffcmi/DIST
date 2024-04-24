#include "HTTPServer.hpp"
#include "requests.h"

int main(int argc, char *argv[])
{
    if ( (argc == 2) && stricmp( argv[1], "2" ) == 0 )
    {
        auto resp = requests::get("http://www.example.com");
        if (resp == NULL) {
            printf("request failed!\n");
        } else {
            printf("%s\n", resp->body.c_str());
        }
        http_headers head;
        head["Authorization"] =  "";
        resp = requests::post("127.0.0.1:8080/echo", "hello,world!", head);
        if (resp == NULL) {
            printf("request failed!\n");
        } else {
            printf("%s\n", resp->body.c_str());
        }

        return 0;
    }
    else
    {
        HttpServer::UPtr server = std::make_unique<HttpServer>();

        server->Start(7777);

        return 0;
    }
}