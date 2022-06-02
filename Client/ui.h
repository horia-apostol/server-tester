#pragma once
#include "client_network.h"
#include "command_service.h"
#include "message_service.h"
#include "login.h"

class ConsoleUi{
    private:
        ClientNetwork client_network;
        CommandController command_service;
        MessageController message_service;
        Login main_user;
        std::string read_command_from_user();
        void show_all_tests();
        void create_test_case();
        void update_test_case();
        void delete_test_case();
        void run_test_case();
        void run_all_tests();
        void show_tests_report();
        void clear_screen();
        void login_ui();
    public:
        ConsoleUi(ClientNetwork, CommandController, MessageController, Login);
        void start_ui();
};