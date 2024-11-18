#include "Utility.hpp"
#include <requests.h>
#include "gtest/gtest.h"

void Add_user();
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
    if ( (argc == 2) && stricmp( argv[1], "UnitTest" ) == 0 )
    {
        ::testing::InitGoogleTest();
        return RUN_ALL_TESTS();
    }

    int command=0;

    while (command != -1) {
        try
        {        
            std::cout << "Choose one of the suggested ones:" << std::endl
            << "1. Add user" << std::endl 
            << "2. Show all users" << std::endl 
            << "3. Show one user" << std::endl 
            << "4. Change a user" << std::endl 
            << "5. Delete a user" << std::endl 
            << "6. Exit" << std::endl;
            (std::cin >> command).get();
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Wrong number");
            }

            switch(command){
                case 1:
                    Add_user();
                    break;

                case 2:
                    Print_all();
                    break;

                case 3:
                    Print_one();
                    break;

                case 4:
                    Change();
                    break;

                case 5:
                    Delete();
                    break;

                case 6:
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

void Add_user(){
    std::string pass, name, info, role;

    std::cout << "Enter your username" << std::endl;
    std::cin >> name;

    std::cout << "Enter your password" << std::endl;
    std::cin >> pass;

    std::cout << "admin or user?" << std::endl;
    std::cin >> role;
    if (role != "admin")
        role = "user";

    std::cout << "Enter your info" << std::endl;
    std::cin >> info;

    std::string name_pass = name+":"+pass;

    http_headers head;
    head["Authorization"] = "Basic " + utils::EncodeBase64(name_pass);

    nlohmann::json body;
    body["password"] = pass;
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

TEST(LoginTests, SuccessfulLogin) {
    std::stringstream ss_add;
    ss_add << "admin\n123\nadmin\nsuperadmin\n";
    std::cin.rdbuf(ss_add.rdbuf());

    Add_user();
    
    EXPECT_TRUE(!current_user.empty());
}

void Print_all(){

    auto resp = requests::get("0.0.0.0:7777/users");
    
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
    std::string name;

    std::cout << "Enter name" << std::endl;
    std::cin >> name;

    std::string str = "0.0.0.0:7777/user/" + name;
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
    std::string nameIn, pass;

    std::cout << "Enter your name" << std::endl;
    std::cin >> nameIn;
    
    std::cout << "Enter your password" << std::endl;
    std::cin >> pass;

    std::string name_pass = nameIn+":"+pass;

    http_headers head;
    head["Authorization"] = "Basic " + utils::EncodeBase64(name_pass);

    std::string find_user;

    std::cout << "Enter the name of the user you want to change" << std::endl;
    std::cin >> find_user;
    
    std::string str = "0.0.0.0:7777/user/" + find_user;
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

    std::string name, info, role;

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

TEST(ChangeTests, Successful) {

    
    std::stringstream ss_change;
    ss_change << "admin\n123\nadmin\n123\nnot\nadmin\nsp\n";
    std::cin.rdbuf(ss_change.rdbuf());
    std::string origin_user[] = {"not", "admin", "sp"};

    Change();

    auto resp = requests::get("0.0.0.0:7777/user/not");
    EXPECT_FALSE(resp == NULL);
    EXPECT_FALSE(resp->status_code != 200);

    auto response = nlohmann::json::parse(resp->body);
    std::cout << response << std::endl;

    EXPECT_TRUE(origin_user[0] == response["name"].get<std::string>());
    EXPECT_TRUE(origin_user[1] == response["role"].get<std::string>());
    EXPECT_TRUE(origin_user[2] == response["info"].get<std::string>());
}

void Delete(){
    std::string nameIn, pass;

    std::cout << "Enter your name" << std::endl;
    std::cin >> nameIn;
    
    std::cout << "Enter your password" << std::endl;
    std::cin >> pass;

    std::string name_pass = nameIn+":"+pass;

    http_headers head;
    head["Authorization"] = "Basic " + utils::EncodeBase64(name_pass);

    std::string find_user;

    std::cout << "Enter the name of the user you want to delete" << std::endl;
    std::cin >> find_user;
            
    std::string str = "0.0.0.0:7777/user/" + find_user;
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
            if (splitted_auth.front() == find_user)
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