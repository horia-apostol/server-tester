#include "client_network.h"
#include "command_service.h"
#include "message_service.h"
#include "ui.h"
#include "iostream"
#include "login.h"

int main(){
    std::string address;
    u_int16_t port;
    std::cin >> address >> port;
    ClientNetwork network(address, port);
    network.create_socket(AF_INET, SOCK_STREAM, 0);
    network.connect_to_server();
    CommandController command_controller(network);
    MessageController message_controller(network);
    Login main_user;
    ConsoleUi application_ui(network, command_controller, message_controller, main_user);
    application_ui.start_ui();
    network.disconnect_from_server();
}