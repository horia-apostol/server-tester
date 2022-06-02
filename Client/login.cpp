#include "login.h"

std::string Login::get_username(){
    return this->username;
}
void Login::login(){
    this->user_logged_in = true;
}
void Login::logout(){
    this->user_logged_in = false;
}
bool Login::is_user_logged_in(){
    return this->user_logged_in;
}