#include "Routers.hpp"
#include "Utility.hpp"
#include <mutex> 

struct User{
    std::string name;
    std::string password;
    std::string info;
    std::string role;
};

std::unordered_map<std::string, User> users;
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

            request = nlohmann::json::parse(req->body);
            if (request.contains("name") && request.contains("Role") && request.contains("info")
            && request.contains("login") && request.contains("password"))
            {
                user.password = request["password"].get<std::string>();
                user.name = request["name"].get<std::string>();
                user.info = request["info"].get<std::string>();
                user.role = request["Role"].get<std::string>();
                users[request["login"].get<std::string>()] = user;
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
            std::string userId = req->GetParam("userId");
            
            if (users.find(userId) != users.end())
            {
                response["name"] = users[userId].name;
                response["info"] = users[userId].info;
                response["role"] = users[userId].role;
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

            std::string login;
            auto basic_auth = req->GetHeader("Authorization");

            if (!basic_auth.empty())
            {
                auto splitted_header = utils::Split(basic_auth, " ");

                if (splitted_header.size() == 2 && splitted_header.front() == "Basic")
                {
                    auto decode = utils::DecodeBase64(splitted_header.back());
                    auto splitted_auth = utils::Split(decode, ":");

                    if (splitted_auth.size() == 2)
                    {
                        login = splitted_auth.front();
                        if (users[login].password != splitted_auth.back())
                            throw std::exception();
                    }
                    else
                        throw std::exception();
                }
                else
                    throw std::exception();
            }

            if (users.empty())
            {
                response["msg"] = "Users not found";
            }
            else
            {    
                for (auto &user : users)
                {
                    nlohmann::json userJson;
                    if (!login.empty() && users[login].role == "admin")
                        userJson["login"] = user.first;
                    userJson["name"] = user.second.name;
                    userJson["info"] = user.second.info;
                    userJson["role"] = user.second.role;
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
            std::string login;
            auto basic_auth = req->GetHeader("Authorization");

            if (!basic_auth.empty())
            {
                auto splitted_header = utils::Split(basic_auth, " ");

                if (splitted_header.size() == 2 && splitted_header.front() == "Basic")
                {
                    auto decode = utils::DecodeBase64(splitted_header.back());
                    auto splitted_auth = utils::Split(decode, ":");

                    if (splitted_auth.size() == 2)
                    {
                        login = splitted_auth.front();
                        if (users[login].password != splitted_auth.back())
                            throw std::exception();
                    }
                    else
                        throw std::exception();
                }
                else
                    throw std::exception();
            }
            else
            {
                response["error"] = "Authorization is required";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 401;
            }

            std::string userId = req->GetParam("userId");
            
            if (users.find(userId) == users.end()){
                response["error"] = "User not found";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 404;
            } 
            if (login == userId || users[login].role == "admin")
            {
                users.erase(userId);
                response["msg"] = "User deleted successfully";
            }
            else{
                response["error"] = "Access denied";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 403;
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

    router.PUT("/user/{userId}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;

        std::unique_lock<std::mutex> my_lock(my_mutex);

        try
        {
            std::string userId = req->GetParam("userId");

            std::string login;
            std::string pass;

            auto basic_auth = req->GetHeader("Authorization");

            if (!basic_auth.empty())
            {
                auto splitted_header = utils::Split(basic_auth, " ");

                if (splitted_header.size() == 2 && splitted_header.front() == "Basic")
                {
                    auto decode = utils::DecodeBase64(splitted_header.back());
                    auto splitted_auth = utils::Split(decode, ":");

                    if (splitted_auth.size() == 2)
                    {
                        login = splitted_auth.front();
                        if (users[login].password != splitted_auth.back())
                            throw std::exception();
                    }
                    else
                        throw std::exception();
                }
                else
                    throw std::exception();
            }
            else
            {
                response["error"] = "Authorization is required";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 401;
            }

            request = nlohmann::json::parse(req->body);

            if (users.find(userId) == users.end()){
                response["error"] = "User not found";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 404;
            }

            if (!request.contains("name") || !request.contains("Role") || !request.contains("info") || !request.contains("password")){
                throw std::exception();
            }

            if (login == userId || users[login].role == "admin")
            {
                users[userId].name = request["name"].get<std::string>();
                users[userId].password = utils::DecodeBase64(request["password"].get<std::string>());
                users[userId].info = request["info"].get<std::string>();
                users[userId].role = request["Role"].get<std::string>();
            }
            else{
                response["error"] = "Access denied";
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            
            response["msg"] = "User change successfully";
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