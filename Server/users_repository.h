#pragma once
#include "map"
#include "string"

class UserRepository{
    private:
        std::map<std::string, std::string> users;
    public:
        void add_user(std::string, std::string);
        bool user_exists(std::string, std::string);
        void delete_user(std::string);
};
