#include <requests.h>
#include "Utility.hpp"

using namespace std;

void add(){
    http_headers head;
        std::string name;
        std::string pass;
        std::string who;
        std::string data;
        cin>>name;
        cin>>pass;
        cin>>who;
        cin>>data;
        string namepass = name+":"+pass;       

        head["Authorization"] =  "Basic "+utils::EncodeBase64(namepass);

        string json = "{\"who\":\""+who+"\",\"data\":\""+data+"\"}";

        auto resp = requests::post("0.0.0.0:7777/user", json, head);
        if (resp == NULL) {
            printf("request failed!\n");
        } else {
             auto request = nlohmann::json::parse(resp->body);
             cout<<request["msg"].get<string>()<<endl;
            //printf("%s\n", resp->body.c_str());
        }

}

void findall(){
    auto resp = requests::get("0.0.0.0:7777/users");
                if (resp == NULL) {
                    printf("request failed!\n");
                } else {
                    //printf("%s\n", resp->body.c_str());
                     
     if(resp->status_code==200){
        auto request = nlohmann::json::parse(resp->body);
                    for(long unsigned int i=0;i<request.size();i++){
                        //cout<<"#"<<request.at(i)["id"].get<int>();
                        cout<<"#"<<request.at(i)["id"].get<int>()<<"Name"<< request.at(i)["client"].get<string>()<<"Role"<< request.at(i)["who"].get<string>()<<"Info"<< request.at(i)["data"].get<string>()<<endl;
                    }
     }
     else{
        cout<<"Error"<<endl;
     }
                    
                }
}

int current_user=-1;

void login(){
    http_headers head;
        std::string name;
        std::string pass;

        cin>>name;
        cin>>pass;

        string namepass = name+":"+pass;       

        head["Authorization"] =  "Basic "+utils::EncodeBase64(namepass);

        string json = "";

        auto resp = requests::post("0.0.0.0:7777/login", json, head);
        if (resp == NULL) {
            printf("request failed!\n");
        } else {
            //printf("%s\n", resp->body.c_str());
            auto request = nlohmann::json::parse(resp->body);
            cout<<request["msg"].get<string>()<<endl;
            current_user = request["id"].get<int>();
            cout<<current_user<<endl;
        }

}

void logout(){
    if (current_user!=-1)
   {
        current_user=-1;
        cout<<"Logout successful"<<endl;
   }
   else
   {
       cout<<"Not logged in"<<endl;
   }
}

void whoami(){
    if (current_user!=-1)
   {        
            string h = to_string(current_user);
            string str = "0.0.0.0:7777/user/" + h;
            const char* a = str.c_str();
            auto resp = requests::get(a);
            //cout<<a<<endl;

                if (resp == NULL) {
                    printf("request failed!\n");
                } else {
                    //printf("%s\n", resp->body.c_str());
                     auto request = nlohmann::json::parse(resp->body);
     
                    //for(long unsigned int i=0;i<request.size();i++){
                        //cout<<"#"<<request.at(i)["id"].get<int>();
                        cout<<"Name "<< request["client"].get<string>()<<endl;
                    //}
                    
                }
       //response["msg"] = "Logged in as: " + current_user;
   }
   else
   {
       cout<<"Not logged in"<<endl;
   }
}

void findone(){
            string h;
            cin>>h;
            string str = "0.0.0.0:7777/user/" + h;
            const char* a = str.c_str();
            auto resp = requests::get(a);
            //cout<<a<<endl;

                if (resp == NULL) {
                    printf("request failed!\n");
                } else {
                    //printf("%s\n", resp->body.c_str());
                     auto request = nlohmann::json::parse(resp->body);
                    if(resp->status_code==200){
                            cout<<"# "<<request["id"].get<int>()<<"Name "<< request["client"].get<string>()<<"Role "<< request["who"].get<string>()<<"Info "<< request["data"].get<string>()<<endl;
                    }
                    else{
                        cout<<"Error"<<endl;
                    }
                    //for(long unsigned int i=0;i<request.size();i++){
                        //cout<<"#"<<request.at(i)["id"].get<int>();
                        
        }
}

void del(){

        http_headers head;
        string namepass = current_user+":123";       
        head["Authorization"] =  "Basic "+utils::EncodeBase64(namepass);
        string json = "";

            string h;
            cin>>h;
            
            string str = "0.0.0.0:7777/user/" + h;
            const char* a = str.c_str();
            auto resp = requests::Delete(a, head);
            //cout<<a<<endl;

                if (resp == NULL) {
                    printf("request failed!\n");
                } 
                else {
                    //printf("%s\n", resp->body.c_str());
                     auto request = nlohmann::json::parse(resp->body);
                    if(resp->status_code==200){
                            cout<<request["msg"].get<string>()<<endl;
                            if(atoi(h.c_str())==current_user){
                                current_user=-1;
                            }
                            cout<<current_user<<endl;
                    }
                    else{
                        cout<<"Error"<<endl;
                    }
                }
}

void change(){
    http_headers head;      

            string h;
            cin>>h;

string str = "0.0.0.0:7777/user/" + h;
            const char* a = str.c_str();
            auto resp = requests::get(a);
            //cout<<a<<endl;

                if (resp == NULL) {
                    printf("request failed!\n");
                } else {
                    //printf("%s\n", resp->body.c_str());
                     auto request = nlohmann::json::parse(resp->body);
                    if(resp->status_code==200){


        std::string name;
        std::string pass;
        std::string who;
        std::string data;
        cin>>name;
        cin>>pass;
        cin>>who;
        cin>>data;
            
        string namepass = name+":"+pass;       

        head["Authorization"] =  "Basic "+utils::EncodeBase64(namepass);

        string json = "{\"who\":\""+who+"\",\"data\":\""+data+"\",\"id\":\""+to_string(current_user)+"\"}";


            string str = "0.0.0.0:7777/user/" + h;
            const char* a = str.c_str();
            auto resp = requests::put(a, json, head);




            //cout<<a<<endl;

                if (resp == NULL) {
                    printf("request failed!\n");
                    //printf("%s\n", resp->body.c_str());
                     
                }
                else{
                    auto request = nlohmann::json::parse(resp->body);
                    if(resp->status_code==200){
                            cout<<request["msg"].get<string>()<<endl;
                    }
                    else{
                        cout<<"Error"<<endl;
                    }
                }

        }
        else{
            cout<<"Error"<<endl;
        }

    }
}

int main(int argc, char *argv[])
{

 

    if ( (argc == 2) && stricmp( argv[1], "2" ) == 0 )
    {

               int command=0;
        int exit=1;
    while (exit==1) {

        std::cout << "Введите команду: ";
        cin>>command;

//login+ logout add+ findall+ findone delete change whoami
        switch(command){
            case 1://login+
                login();
                break;

            case 2://logout+
                logout();
                break;

            case 3://add+
                add();
                break;

            case 4://findall+
                findall();               
                break;

            case 5://find one+
                findone();
                break;

            case 6://delete
            if(current_user==-1){
                    cout<<"Sign in,please"<<endl;
            }
            else{
                    del();
            }
                
                break;

            case 7://change
            if(current_user==-1){
                    cout<<"Sign in,please"<<endl;
            }
            else{
                change();
                }
                break;

            case 8://whoami+
                whoami();
                break;

            case 0:
                exit=0;
                 break;
        }
    }

        
        return 0;
    }
    else
    {
        return 0;
    }
}