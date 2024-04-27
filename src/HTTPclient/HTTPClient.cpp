#include "Utility.hpp"
#include <requests.h>

void Sign_up();
void Sign_in();
void Print_all();
void Print_one();
void Change();
void Delete();
void Whoami();
void Log_out();


std::string current_user;

int main(int argc, char *argv[])
{
    int command=0;

    while (command != -1) {
        try
        {
            std::cout << "Choose one of the suggested ones:" << std::endl
            << "1. Sign up" << std::endl
            << "2. Sign in" << std::endl
            << "3. Show all users" << std::endl 
            << "4. Show one user" << std::endl 
            << "5. Change a user" << std::endl 
            << "6. Delete a user" << std::endl 
            << "7. Who am i" << std::endl 
            << "8. Sign out" << std::endl 
            << "9. Exit" << std::endl;
            (std::cin >> command).get();
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Wrong number");
            }

            switch(command){
                case 1:
                    Sign_up();
                    break;

                case 2:
                    Sign_in();
                    break;

                case 3:
                    Print_all();
                    break;

                case 4:
                    Print_one();
                    break;

                case 5:
                    Change();
                    break;

                case 6:
                    Delete();
                    break;

                case 7:
                    Whoami();
                    break;

                case 8:
                    Log_out();
                    break;

                case 9:
                    command = -1;
                    break;
                default:
                    break;
            }
        }
        catch (std::invalid_argument& ex)
        {
            std::cout << "invalid_argument: " << ex.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
        
            std::cout << std::endl << std::endl;
    }

    return 0;
}

void Sign_up(){
    std::string login, pass, name, info, role;

    std::cout << "Enter your login" << std::endl;
    std::cin >> login;
    
    std::cout << "Enter your password" << std::endl;
    std::cin >> pass;

    std::cout << "Enter your username" << std::endl;
    std::cin >> name;

    std::cout << "admin or user?" << std::endl;
    std::cin >> role;
    if (role != "admin")
        role = "user";

    std::cout << "Enter your info" << std::endl;
    std::cin >> info;

    std::string name_pass = login+":"+pass;

    http_headers head;
    head["Authorization"] = "Basic " + utils::EncodeBase64(name_pass);

    nlohmann::json body;
    body["name"] = name;
    body["Role"] = role;
    body["info"] = info;

    auto resp = requests::post("0.0.0.0:7777/user", body.dump(), head);
    if (resp == NULL) {
        std::cout << "request failed!" << std::endl;
    } else if (resp->status_code != 200){
            auto response = nlohmann::json::parse(resp->body);
            std::string error = "Error " + std::to_string(resp->status_code) + ": " + response["error"].get<std::string>();
            std::cout << error << std::endl;
    }
    else{
        auto response = nlohmann::json::parse(resp->body);
        current_user = utils::EncodeBase64(name_pass);
        std::cout << response["msg"].get<std::string>() << std::endl;
    }
}



void Sign_in(){
    std::string login;
    std::string pass;

    std::cout << "Enter your login" << std::endl;
    std::cin >> login;
    
    std::cout << "Enter your password" << std::endl;
    std::cin >> pass;

    std::string name_pass = login+":"+pass;       

    http_headers head;
    head["Authorization"] =  "Basic " + utils::EncodeBase64(name_pass);

    auto resp = requests::post("0.0.0.0:7777/login", "", head);

    if (resp == NULL) {
        std::cout << "request failed!" << std::endl;
    } else if (resp->status_code != 200){
            auto response = nlohmann::json::parse(resp->body);
            std::string error = "Error " + std::to_string(resp->status_code) + ": " + response["error"].get<std::string>();
            std::cout << error << std::endl;
    }
    else{
        auto response = nlohmann::json::parse(resp->body);
        current_user = utils::EncodeBase64(name_pass);
        std::cout << response["msg"].get<std::string>() << std::endl;
    }
}

void Print_all(){

    if (current_user.empty()){
        std::cout << "Sign in, please" << std::endl;
        return;
    }

    http_headers head;
    head["Authorization"] =  "Basic " + current_user;
    
    auto resp = requests::get("0.0.0.0:7777/users", head);
    
    if (resp == NULL) {
        std::cout << "request failed!" << std::endl;
    } else if (resp->status_code != 200){
            auto response = nlohmann::json::parse(resp->body);
            std::string error = "Error " + std::to_string(resp->status_code) + ": " + response["error"].get<std::string>();
            std::cout << error << std::endl;
    }
    else{
        auto response = nlohmann::json::parse(resp->body);
        
        if (response.contains("msg"))
            std::cout << response["msg"].get<std::string>() << std::endl;
        else{
            for(long unsigned int i=0; i<response.size(); i++){
                std::cout << i << ") ";

                for (auto it = response.at(i).begin(); it != response.at(i).end(); ++it)
                {
                    std::cout << it.key() << ": " << it.value() << ", ";
                }
                std::cout << std::endl;
            }
        }
    }
}

void Print_one(){
    std::string login;

    std::cout << "Enter login" << std::endl;
    std::cin >> login;

    std::string str = "0.0.0.0:7777/user/" + login;
    const char* a = str.c_str();
    auto resp = requests::get(a);

    if (resp == NULL) {
        std::cout << "request failed!" << std::endl;
    } else if (resp->status_code != 200){
            auto response = nlohmann::json::parse(resp->body);
            std::string error = "Error " + std::to_string(resp->status_code) + ": " + response["error"].get<std::string>();
            std::cout << error << std::endl;
    }
    else{
        auto response = nlohmann::json::parse(resp->body);
        
        for (auto it = response.begin(); it != response.end(); ++it)
        {
            std::cout << it.key() << ": " << it.value() << std::endl;
        }
    }
}

void Change(){

    if (current_user.empty()){
        std::cout << "Sign in, please" << std::endl;
        return;
    }

    std::string find_login;

    std::cout << "Enter the login of the user you want to change" << std::endl;
    std::cin >> find_login;
    
    std::string str = "0.0.0.0:7777/user/" + find_login;
    const char* a = str.c_str();
    auto resp = requests::get(a);

    if (resp == NULL) {
        std::cout << "request failed!" << std::endl;
        return;
    } 
    else if (resp->status_code != 200){
            auto response = nlohmann::json::parse(resp->body);
            std::string error = "Error " + std::to_string(resp->status_code) + ": " + response["error"].get<std::string>();
            std::cout << error << std::endl;
            return;
    }

    std::string login, pass, name, info, role;

    std::cout << "Enter new password" << std::endl;
    std::cin >> pass;

    std::cout << "Enter new username" << std::endl;
    std::cin >> name;

    std::cout << "admin or user?" << std::endl;
    std::cin >> role;
    if (role != "admin")
        role = "user";

    std::cout << "Enter new info" << std::endl;
    std::cin >> info;

    http_headers head;
    head["Authorization"] = "Basic " + current_user;
    
    nlohmann::json body;
    body["name"] = name;
    body["password"] = utils::EncodeBase64(pass);
    body["Role"] = role;
    body["info"] = info;

    resp = requests::put(a, body.dump(), head);

    if (resp == NULL) {
        std::cout << "request failed!" << std::endl;
    } else if (resp->status_code != 200){
            auto response = nlohmann::json::parse(resp->body);
            std::string error = "Error " + std::to_string(resp->status_code) + ": " + response["error"].get<std::string>();
            std::cout << error << std::endl;
    }
    else{
        auto response = nlohmann::json::parse(resp->body);
        std::cout << response["msg"].get<std::string>() << std::endl;
    }

    
}

void Delete(){
    http_headers head;
    head["Authorization"] =  "Basic " + current_user;
    
    std::string find_login;

    std::cout << "Enter the login of the user you want to delete" << std::endl;
    std::cin >> find_login;
            
    std::string str = "0.0.0.0:7777/user/" + find_login;
    const char* a = str.c_str();
    auto resp = requests::Delete(a, head);

    if (resp == NULL) {
        std::cout << "request failed!" << std::endl;
    } else if (resp->status_code != 200){
            auto response = nlohmann::json::parse(resp->body);
            std::string error = "Error " + std::to_string(resp->status_code) + ": " + response["error"].get<std::string>();
            std::cout << error << std::endl;
    }
    else{
        auto response = nlohmann::json::parse(resp->body);
        std::cout << response["msg"].get<std::string>() << std::endl;
        
        auto decode = utils::DecodeBase64(current_user);
        auto splitted_auth = utils::Split(decode, ":");

        if (splitted_auth.size() == 2)
        {
            if (splitted_auth.front() == find_login)
                current_user.clear();
        }
        else
            throw std::exception();
    }
}

void Log_out(){
    if (!current_user.empty())
    {
        current_user.clear();
        std::cout << "Logout successful" << std::endl;
    }
    else
    {
        std::cout << "Not logged in" << std::endl;
    }
}

void Whoami(){
    if (current_user.empty())
    {
        std::cout << "Not logged in" << std::endl;
        return;
    }
    
    auto decode = utils::DecodeBase64(current_user);
    auto splitted_auth = utils::Split(decode, ":");

    if (splitted_auth.size() != 2)
    {
        throw std::exception();
    }
        
    std::string str = "0.0.0.0:7777/user/" + splitted_auth.front();
    const char* a = str.c_str();
    auto resp = requests::get(a);

    if (resp == NULL) {
        std::cout << "request failed!" << std::endl;
    } else if (resp->status_code != 200){
            auto response = nlohmann::json::parse(resp->body);
            std::string error = "Error " + std::to_string(resp->status_code) + ": " + response["error"].get<std::string>();
            std::cout << error << std::endl;
    }
    else{
        auto response = nlohmann::json::parse(resp->body);
        
            std::cout << "Login: " << splitted_auth.front() << std::endl
            << "Name: " << response["name"].get<std::string>() << std::endl;
        
    }
}