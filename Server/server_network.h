#pragma once
#include "string"
#include "netinet/in.h"
#include <sys/types.h>

class ServerNetwork{
    private:
        struct sockaddr_in server_handler;
        struct sockaddr_in client_handler;
        int32_t server_socket_descriptor, client_socket_descriptor, domain;
        u_int16_t port;
        in_addr_t address;
    public:
        ServerNetwork(in_addr_t, u_int16_t);
        void create_server_socket(u_int32_t, u_int32_t, u_int32_t);
        void bind_socket();
        void listen_for_clients(int32_t);
        void accept_clients();
        void close_client_socket();
        void close_server_socket();
        void send_message_to_client(std::string);
        std::string receive_message_from_client();
};