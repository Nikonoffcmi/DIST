#include "Routers.hpp"
#include <map>

std::map<int, nlohmann::json> users;

void route::RegisterResources(hv::HttpService &router)
{
    // router.GET("/hello", [](HttpRequest *req, HttpResponse *resp)
    // {
    //     nlohmann::json response
    //     {
    //         {"куе", "Hello world"}
    //     };

    //     resp->SetBody(response.dump());
    //     resp->content_type = APPLICATION_JSON;

    //     return 200;
    // });

    // router.POST("/hello", [](HttpRequest *req, HttpResponse *resp)
    // {
    //     nlohmann::json request;
    //     nlohmann::json response;

    //     try
    //     {
    //         request = nlohmann::json::parse(req->body);
    //     }
    //     catch(const std::exception& e)
    //     {
    //         response["error"] = "Invalid JSON";
    //         resp->SetBody(response.dump());
    //         resp->content_type = APPLICATION_JSON;
    //         return 400;
    //     }
        
    //     resp->SetBody(request.dump());
    //     resp->content_type = APPLICATION_JSON;

    //     return 200;
    // });

    router.POST("/user", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;

        try
        {
            request = nlohmann::json::parse(req->body);
            int id = users.size();
            users[id] = request; 
            response["id"] = id;
            response["msg"] = "User added successfully";
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid data";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;

        return 200;
    });

    router.GET("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        int userId = std::stoi(req->GetParam("userId"));

        if (users.find(userId) != users.end())
        {
            response = users[userId];
        }
        else
        {
            response["error"] = "User not found";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 404;
        }

        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;

        return 200;
    });

    router.GET("/users", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response = nlohmann::json::array();

        for (auto &user : users)
        {
            nlohmann::json userJson;
            userJson["id"] = user.first;
            userJson["data"] = user.second;
            response.push_back(userJson);
        }

        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;

        return 200;
    });

    router.Delete("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        int userId = std::stoi(req->GetParam("userId"));

        if (users.find(userId) != users.end())
        {
            users.erase(userId);
            response["msg"] = "User deleted successfully";
        }
        else
        {
            response["error"] = "User not found";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 404;
        }

        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;

        return 200;
    });
}