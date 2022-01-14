#pragma once
#include "string"
#include "client_network.h"
#include "host_name_validator.h"

class MessageController{
    private:
        ClientNetwork network;
        HostNameValidator service_validator;
    public:
        MessageController(ClientNetwork);
        void send_message(std::string);
};