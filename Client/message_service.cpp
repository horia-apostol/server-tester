#include "message_service.h"
#include "host_name_validator.h"
#include "client_network.h"
#include "iostream"

MessageController::MessageController(ClientNetwork network)
    : network{ network } {}

void MessageController::send_message(std::string message){
    this->service_validator = HostNameValidator(message);
    if (this->service_validator.host_name_is_valid()){
        this->network.send_message_to_server(message);
    }
}