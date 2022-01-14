#pragma once
#include "string"

class Login{
    private:
        std::string username;
        bool user_logged_in = 0;
    public:
        std::string get_username();
        void login();
        void logout();
        bool is_user_logged_in();
};