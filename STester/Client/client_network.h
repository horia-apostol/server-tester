#pragma once
#include "string"
#include "netinet/in.h"
#include <sys/types.h>

class ClientNetwork {
    private:
        struct sockaddr_in handler;
        int32_t socket_descriptor, domain;
        u_int16_t port;
        std::string address;
    public:
        ClientNetwork();
        ClientNetwork(std::string, u_int16_t);
        void create_socket(u_int32_t, u_int32_t, u_int32_t);
        void connect_to_server();
        void disconnect_from_server();
        void send_message_to_server(std::string);
        std::string receive_message_from_server();
};