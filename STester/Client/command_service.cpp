#include "command_service.h"
#include "command_validator.h"
#include "client_network.h"

CommandController::CommandController(ClientNetwork network)
    : network{ network } {}

std::string CommandController::send_command(std::string command, char32_t max_command){
    this->service_validator = CommandValidator(command);
    if (this->service_validator.command_is_valid(max_command)) {
        this->network.send_message_to_server(command);
        return command;
    }       
    return "invalid command";
}