#include "users_repository.h"
#include "string"

void UserRepository::add_user(std::string username, std::string password){
    this->users[username] = password;
}

bool UserRepository::user_exists(std::string username, std::string password){
    for(std::map<std::string, std::string>::iterator i=this->users.begin(); i!=this->users.end(); ++i)
        if (i->first == username && i->second == password)
            return true;
    return false;
}

void UserRepository::delete_user(std::string username){
    this->users.erase(username);
}