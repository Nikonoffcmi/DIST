#include "Routers.hpp"
#include <mutex> 

std::map<int64_t, nlohmann::json> users;
int64_t count = 0;
std::mutex my_mutex;

void route::RegisterResources(hv::HttpService &router)
{
    router.POST("/user", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;
        std::unique_lock<std::mutex> my_lock(my_mutex);

        try
        {
            request = nlohmann::json::parse(req->body);
            request["id"] = count;
            users[count++] = request;
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
        my_lock.unlock();
        return 200;
    });

    router.GET("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        std::unique_lock<std::mutex> my_lock(my_mutex);

        try
        {
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
        my_lock.unlock();

        return 200;
    });

    router.GET("/users", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        std::unique_lock<std::mutex> my_lock(my_mutex);

        try{
            if (users.empty())
            {
                nlohmann::json userJson;
                userJson["data"] = "";
                response.push_back(userJson);
            }
            else
            {    
                for (auto &user : users)
                {
                    nlohmann::json userJson;
                    userJson["data"] = user.second;
                    response.push_back(userJson);
                }
            }
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
        my_lock.unlock();

        return 200;
    });

    router.Delete("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        std::unique_lock<std::mutex> my_lock(my_mutex);

        try
        {
            int userId = std::stoi(req->GetParam("userId"));
            
            if (users.find(userId) != users.end())
            {
                users.erase(userId);
                response["msg"] = "User deleted successfully";
            }
            else{
                response["error"] = "User not found";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 404;
            }
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
        my_lock.unlock();
        
        return 200;
    });
}