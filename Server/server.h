#pragma once
#include "string"

class Server{
    private:
        std::string server_name;
    public:
        Server(std::string);
        std::string get_server_name();
};