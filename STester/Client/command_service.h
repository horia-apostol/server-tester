#pragma once
#include "string"
#include "command_validator.h"
#include "client_network.h"

class CommandController{
    private:
        ClientNetwork network;
        CommandValidator service_validator;
    public:
        CommandController(ClientNetwork);
        std::string send_command(std::string, char32_t);
};