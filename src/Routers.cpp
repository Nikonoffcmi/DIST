#include "Routers.hpp"
#include "Utility.hpp"
#include <mutex> 

struct User{
    uint64_t id;
    std::string name;
    std::string info;
    std::string password;
    Role role;
};

std::unordered_map<uint64_t, User> users;
uint64_t count = 0;
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
            User user;
            auto basic_auth = req->GetHeader("Authorization");

            if (!basic_auth.empty())
            {
                auto splited_header = utils::Split(basic_auth, " ");

                if (splited_header.size() == 2 && splited_header.front() == "Basic")
                {
                    auto decode = utils::DecodeBase64(splited_header.back());
                    auto splited_auth = utils::Split(decode, ":");

                    if (splited_auth.size() == 2)
                    {
                        std::cout << splited_auth.front() << " : " << splited_auth.back() << std::endl;
                        user.name = splited_auth.front();
                        user.password = splited_auth.back();
                    }
                }
            }

            request = nlohmann::json::parse(req->body);
            if (request.contains("Role") && request.contains("info"))
            {
                user.info = request["info"].get<std::string>();
                user.role = request["Role"].get<Role>();
                user.id = count;
                users[count++] = user;
            }
            else
                throw std::exception();
            
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
        std::unique_lock<std::mutex> my_lock(my_mutex);

        try
        {
            int userId = std::stoi(req->GetParam("userId"));
            
            if (users.find(userId) != users.end())
            {
                response["id"] = users[userId].id;
                response["name"] = users[userId].name;
                response["info"] = users[userId].info;
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
                    userJson["id"] = user.second.id;
                    userJson["name"] = user.second.name;
                    userJson["info"] = user.second.info;
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
        
        return 200;
    });
}