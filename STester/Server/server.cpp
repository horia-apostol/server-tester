#include "server.h"
#include "string"

Server::Server(std::string server_name)
    : server_name{ server_name } {}

std::string Server::get_server_name() {
    return this->server_name;
}
