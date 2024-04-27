#include "HTTPServer.hpp"
#include "requests.h"

int main(int argc, char *argv[])
{
    if ( (argc == 2) && stricmp( argv[1], "2" ) == 0 )
    {
        auto resp = requests::get("http://www.example.com");
        http_headers head;
        head["Authorization"] =  "";
        http_body body = "{\"client\":\"us1\",\"pass\":\"123\",\"who\":\"admin\",\"data\":\"hi\"}";
        resp = requests::post("0.0.0.0:7777/user", body);
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