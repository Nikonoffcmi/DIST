#include "Routers.hpp"
#include "Utility.hpp"
#include <mutex> 
#include <postgresql/libpq-fe.h>


struct User{
    std::string name;
    std::string password;
    std::string info;
    std::string role;
};

// std::unordered_map<std::string, User> users;
std::mutex my_mutex;
PGconn *conn;
int ConnectPostgres();
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
            && request.contains("password"))
            {
                user.password = request["password"].get<std::string>();
                user.name = request["name"].get<std::string>();
                user.info = request["info"].get<std::string>();
                user.role = request["Role"].get<std::string>();

                PGresult *res = NULL;
                if (ConnectPostgres() == 1) 
                    throw std::exception();

                
                std::string bdReq = "insert into users values(DEFAULT, '"+user.name + "', '" + user.password + "', '" + user.role + "', '" + user.info + "');";
                res = PQexec(conn, bdReq.c_str());
                if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                    std::cout << "Insert into table failed: " << PQresultErrorMessage(res)
                            << std::endl;
                    throw std::exception();
                }
                PQclear(res);
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

            PGresult *res = NULL;
                if (ConnectPostgres() == 1) 
                    throw std::exception();
            
            std::string dbReq = "select name, role, info from users where name = '" + userId + "';";
            res = PQexec(conn, dbReq.c_str());
            if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
                throw std::exception();
            } else {
                
                if (PQntuples(res) < 1){
                    response["error"] = "User not found";
                    resp->SetBody(response.dump());
                    resp->content_type = APPLICATION_JSON;
                    return 404;
                }
                else {
                    for (int i = 0; i < PQntuples(res); i++) {
                        response["name"] = PQgetvalue(res, i, 0);
                        response["role"] = PQgetvalue(res, i, 1);
                        response["info"] = PQgetvalue(res, i, 2);
                    }
                }
            }
            PQclear(res);

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
            PGresult *res = NULL;
                if (ConnectPostgres() == 1) 
                    throw std::exception();
            
            res = PQexec(conn, "select name, role, info from users;");
            if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
                throw std::exception();
            } else {
                
                nlohmann::json userJson;
                
                if (PQntuples(res) < 1)
                    response["msg"] = "Users not found";
                else {
                    for (int i = 0; i < PQntuples(res); i++) {
                        userJson["name"] = PQgetvalue(res, i, 0);
                        userJson["role"] = PQgetvalue(res, i, 1);
                        userJson["info"] = PQgetvalue(res, i, 2);
                        response.push_back(userJson);
                    }
                }
            }
            PQclear(res);
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

            PGresult *res = NULL;
                if (ConnectPostgres() == 1) 
                    throw std::exception();
            
            std::string role;
            std::string name;

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
                        std::string dbReq = "select password, role, name from users where name = '" + login + "';";
                        res = PQexec(conn, dbReq.c_str());
                        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                            std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
                            throw std::exception();
                        } else {
                            if (PQgetvalue(res, 0, 0) != splitted_auth.back())
                                throw std::exception();

                            role = PQgetvalue(res, 0, 1);
                            name = PQgetvalue(res, 0, 2);
                        }
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

            PQclear(res);
            std::string userId = req->GetParam("userId");
            
            std::string dbReq = "select id from users where name = '" + userId + "';";
            res = PQexec(conn, dbReq.c_str());
            if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
                throw std::exception();
            } else {
                
                nlohmann::json userJson;
                
                if (PQntuples(res) < 1){
                    response["error"] = "User not found";
                    resp->SetBody(response.dump());
                    resp->content_type = APPLICATION_JSON;
                    return 404;
                }
                else {
                    if (name == userId || role == "admin")
                    {
                        std::string dbReq = "delete from users where name = '" + userId + "';";
                        PQclear(res);
                        res = PQexec(conn, dbReq.c_str());
                        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                            std::cout << "Delete failed: " << PQresultErrorMessage(res) << std::endl;
                            throw std::exception();
                        } else {
                            response["msg"] = "User deleted successfully";
                        }
                    }
                    else{
                        response["error"] = "Access denied";
                        resp->SetBody(response.dump());
                        resp->content_type = APPLICATION_JSON;
                        return 403;
                    }
                }
            }
            PQclear(res);
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

            PGresult *res = NULL;
            if (ConnectPostgres() == 1) 
                throw std::exception();
            
            std::string role;
            std::string name;

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
                        std::string dbReq = "select password, role, name from users where name = '" + login + "';";
                        res = PQexec(conn, dbReq.c_str());
                        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                            std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
                            throw std::exception();
                        } else {
                            if (PQgetvalue(res, 0, 0) != splitted_auth.back())
                                throw std::exception();

                            role = PQgetvalue(res, 0, 1);
                            name = PQgetvalue(res, 0, 2);
                        }
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

            PQclear(res);
            request = nlohmann::json::parse(req->body);
            
            std::string dbReq = "select id from users where name = '" + userId + "';";
            res = PQexec(conn, dbReq.c_str());
            if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                std::cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
                throw std::exception();
            } else {
                nlohmann::json userJson;
                
                if (PQntuples(res) < 1){
                    response["error"] = "User not found";
                    resp->SetBody(response.dump());
                    resp->content_type = APPLICATION_JSON;
                    return 404;
                }
                
                PQclear(res);
                if (!request.contains("name") || !request.contains("Role") || !request.contains("info") || !request.contains("password")){
                    throw std::exception();
                }

                if (name == userId || role == "admin")
                {
                    User user;
                    user.name = request["name"].get<std::string>();
                    user.password = utils::DecodeBase64(request["password"].get<std::string>());
                    user.info = request["info"].get<std::string>();
                    user.role = request["Role"].get<std::string>();
                    
                    dbReq = "update users set name = '"+ user.name + "', password = '" + user.password + "', role = '" + user.role + "', info = '" + user.info + "' where name = '" + userId + "';";
                    res = PQexec(conn, dbReq.c_str());
                    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                        std::cout << "Update failed: " << PQresultErrorMessage(res) << std::endl;
                        throw std::exception();
                    } else {
                        response["msg"] = "User change successfully";
                    }
                }
                else{
                    response["error"] = "Access denied";
                    resp->SetBody(response.dump());
                    resp->content_type = APPLICATION_JSON;
                    return 403;
                }
            }
            PQclear(res);
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

int ConnectPostgres() {
    
    conn = PQconnectdb("");
    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    } else {
        std::cout << "Connection to database succeed." << std::endl;
        return 0;
    }

    return 0;

}