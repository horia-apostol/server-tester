#include "server_network.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "strings.h"
#include "iostream"
#include "network_validator.h"

typedef void* void_pointer;

ServerNetwork::ServerNetwork(in_addr_t address, u_int16_t port) 
    : port{ port }, address{ address }{
        bzero(&this->server_handler, sizeof(this->server_handler));
        bzero(&this->client_handler, sizeof(this->client_handler));
    }

void ServerNetwork::create_server_socket(u_int32_t domain, u_int32_t type, u_int32_t protocol){
    this->server_socket_descriptor = socket(domain, type, protocol);
    NetworkValidator create_socket_validator(this->server_socket_descriptor, "[SERVER]Socket creator");
    create_socket_validator.validate_task();
    create_socket_validator.show_errors();
    void_pointer random_void_pointer;
    setsockopt(this->server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, random_void_pointer, sizeof(int32_t));
    this->domain = domain;
}

void ServerNetwork::bind_socket(){
    this->server_handler.sin_family = this->domain;
    this->server_handler.sin_addr.s_addr = this->address;
    this->server_handler.sin_port = htons(this->port);
    NetworkValidator bind_validator(bind(this->server_socket_descriptor, reinterpret_cast<struct sockaddr*>(&this->server_handler), sizeof(struct sockaddr)), "[SERVER]Bind");
    bind_validator.validate_task();
    bind_validator.show_errors();
}

void ServerNetwork::listen_for_clients(int32_t backlog){
    NetworkValidator listen_validator(listen(this->server_socket_descriptor, backlog), "[SERVER]Listen");
    listen_validator.validate_task();
    listen_validator.show_errors();
}

void ServerNetwork::accept_clients(){
    socklen_t client_address_length;
    this->client_socket_descriptor = accept(this->server_socket_descriptor, reinterpret_cast<struct sockaddr*>(&this->server_handler), &client_address_length);
    NetworkValidator accept_validator(this->client_socket_descriptor, "[SERVER]Accept");
    accept_validator.validate_task();
    accept_validator.show_errors();
}

void ServerNetwork::close_client_socket(){
    close(this->client_socket_descriptor);
}

void ServerNetwork::close_server_socket(){
    close(this->server_socket_descriptor);
}

void ServerNetwork::send_message_to_client(std::string message){
    NetworkValidator send_validator(send(this->client_socket_descriptor, message.c_str(), message.size(), 0), "[SERVER]Send message to server");
}

std::string ServerNetwork::receive_message_from_client(){
    char buffer[512] = {};
    bzero(buffer, sizeof(buffer));
    NetworkValidator receive_validator(recv(this->client_socket_descriptor, buffer, 512, 0), "[SERVER]Receive message from client");
    return buffer;
}