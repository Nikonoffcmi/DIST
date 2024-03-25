#include "Routers.hpp"

nlohmann::json users;

void route::RegisterResources(hv::HttpService &router)
{
    router.POST("/user", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;

        try
        {
            request = nlohmann::json::parse(req->body);

            for (auto &user : users)
            {
                if (request["id"].get<int>() == user["id"].get<int>())
                    throw std::exception();
            }

            users.push_back(request); 
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

        try
        {
            int userId = std::stoi(req->GetParam("userId"));
            
            for (auto &user : users)
            {
                if (user["id"].get<int>() == userId){
                    response["data"] = user;
                    break;
                }
            }
            
            if (response.empty())
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

        return 200;
    });

    router.GET("/users", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        try{
            if (!users.empty())
                response["data"] = users;
            else
                response["msg"] = "There are no users";
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

    router.Delete("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;

        try
        {
            int userId = std::stoi(req->GetParam("userId"));
            
            auto it = users.begin();
            for(; it != users.end(); ++it) {
                if(it->at("id") == userId)
                    break;
            }

            if(it != users.end()){
                users.erase(it);
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

        return 200;
    });
}