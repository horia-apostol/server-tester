#pragma once
#include "server_network.h"
#include "users_repository.h"

class LoginService{
    private:
        UserRepository repository;
        ServerNetwork network;
        void service_add_user_to_repo(std::string, std::string);
        bool service_user_exists_in_repo(std::string, std::string);
        void service_user_logout(std::string);
    public:
        LoginService(UserRepository, ServerNetwork);
        void start_login_service();
};
