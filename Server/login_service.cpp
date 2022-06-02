#include "login_service.h"
#include "users_repository.h"
#include "server_network.h"
#include "string"
#include "iostream"

LoginService::LoginService(UserRepository repository, ServerNetwork network)
    : repository{ repository }, network{ network } {}
void LoginService::service_add_user_to_repo(std::string username, std::string password){
    this->repository.add_user(username, password);
}
bool LoginService::service_user_exists_in_repo(std::string username, std::string password){
    if (this->repository.user_exists(username, password)){
        this->network.send_message_to_client("exists");
        return true;
    }
    else {
        this->network.send_message_to_client("doesntexist"); // :))
        return false;
    }
}
void LoginService::service_user_logout(std::string username){
    this->repository.delete_user(username);
}
void LoginService::start_login_service(){
    while(1){
        std::string command = this->network.receive_message_from_client();
        if (command == "0"){
            std::string username = this->network.receive_message_from_client();
            std::string password = this->network.receive_message_from_client();
            service_add_user_to_repo(username, password);
        }
        else if(command == "1"){
            std::string username = this->network.receive_message_from_client();
            std::string password = this->network.receive_message_from_client();
            if (service_user_exists_in_repo(username, password) == true)
                return;
        }
    }
}