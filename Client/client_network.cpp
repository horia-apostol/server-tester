
#include "client_network.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "iostream"
#include "errno.h"
#include "network_validator.h"
#include "strings.h"

ClientNetwork::ClientNetwork(std::string address, u_int16_t port)
    : address{ address }, port{ port } {};

void ClientNetwork::create_socket(u_int32_t domain, u_int32_t type, u_int32_t protocol){
    this->socket_descriptor = socket(domain, type, protocol);
    this->domain = domain;
    NetworkValidator socket_validator(this->socket_descriptor, "[CLIENT]Socket creator");
    socket_validator.validate_task();
    socket_validator.show_errors();
}

void ClientNetwork::connect_to_server(){
    this->handler.sin_family = this->domain;
    this->handler.sin_addr.s_addr = inet_addr(this->address.c_str());
    this->handler.sin_port = htons(this->port);
    NetworkValidator connect_validator(connect(this->socket_descriptor, reinterpret_cast<struct sockaddr*>(&this->handler), sizeof(struct sockaddr)), "[CLIENT]Client connects to server");
    connect_validator.validate_task();
    connect_validator.show_errors();

}
void ClientNetwork::disconnect_from_server(){
    close(this->socket_descriptor);
}

void ClientNetwork::send_message_to_server(std::string message){
    NetworkValidator send_validator(send(this->socket_descriptor, message.c_str(), message.size()+1, 0), "[CLIENT]Client sends message to server");
    send_validator.validate_task();
    send_validator.show_errors();
}

std::string ClientNetwork::receive_message_from_server(){
    char buffer[512] = {};
    bzero(buffer, sizeof(buffer));
    NetworkValidator recieve_validator(recv(this->socket_descriptor, buffer, 512, 0), "[CLIENT]Client recieves message from server");
    recieve_validator.validate_task();
    recieve_validator.show_errors();
    return buffer;
}