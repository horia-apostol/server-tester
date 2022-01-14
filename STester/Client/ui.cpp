#include "ui.h"
#include "command_service.h"
#include "client_network.h"
#include "command_validator.h"
#include "message_service.h"
#include "iostream"
#include "unistd.h"
#include <cstdint>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include "utmp.h"
#include "login.h"

ConsoleUi::ConsoleUi(ClientNetwork client_network, CommandController command_service, MessageController message_service, Login main_user)
    : client_network{ client_network }, command_service{ command_service }, message_service{ message_service }, main_user{ main_user } {}

std::string ConsoleUi::read_command_from_user(){
    std::cout << " 0 - Exit\n";
    std::cout << " 1 - Show my test cases\n";
    std::cout << " 2 - Create a new test case\n";
    std::cout << " 3 - Delete an existing test case\n";
    std::cout << " 4 - Run a test case\n";
    std::cout << " 5 - Run all tests\n";
    std::cout << " 6 - Get tests report\n";

    std::string command;
    std::cout << "Please give command:\n";
    getline(std::cin, command);
    return command;
}

void ConsoleUi::clear_screen(){
    std::cout << "\033[H\033[J";
}
void ConsoleUi::show_all_tests(){
    clear_screen();
    std::cout << this->client_network.receive_message_from_server() << '\n';
    std::cout << "Press 0 to go back\n";
    while(1){
        std::string go_back_command;
        getline(std::cin, go_back_command);
        if (go_back_command == "0")
            break;
    }
}
void ConsoleUi::create_test_case(){
    while (1){
        clear_screen();
        std::cout << "Please select the type of test you want to add:\n";
        std::cout << "0 - Back\n";
        std::cout << "1 - Measure latency for a server\n";
        std::cout << "2 - Test a DNS IP request\n";
        std::cout << "3 - Test if a HTTP page is downloadable\n";
        std::cout << "4 - Test if you can create a directory on a FTP server\n";
        std::cout << "5 - Test if you cand upload a file on a FTP server\n";
        std::string command = "";
        getline(std::cin, command);
        CommandValidator my_validator(command);
        if (my_validator.command_is_valid('5')){
            if (command == "0") {
                this->command_service.send_command(command, '5');
                return;
            }
            else {
                this->command_service.send_command(command, '5');
                clear_screen();
                std::string test_name;
                std::cout << "Please give a name for your test: \n";
                getline(std::cin, test_name);
                this->message_service.send_message(test_name);
                std::string host_name;
                std::cout << "Please give the hostname: \n";
                getline(std::cin, host_name);
                this->message_service.send_message(host_name);
                return;
            }
        }
        else {
            continue;
        }
    }
    
}
void ConsoleUi::delete_test_case(){
    clear_screen();
    std::string all_tests = this->client_network.receive_message_from_server();

    if (all_tests == "No tests yet!"){
        clear_screen();
        std::cout << "No tests yet!" << '\n';
        std::cout << "Press 0 to go back" << '\n';
        while(1){
            std::string command;
            getline(std::cin, command);
            if (command == "0") {
                this->client_network.send_message_to_server("0");
                return;
            }    
        }
    }
    else {
        std::cout << "Please select the id of the test you want to delete or press 00 to go back\n";
        std::cout << all_tests << '\n';
        while(1){
            std::string command;
            getline(std::cin, command);
            if (command == "00"){
                this->client_network.send_message_to_server(command);
                break;
            }
            else{
                this->client_network.send_message_to_server(command);
                std::string length = this->client_network.receive_message_from_server();
                int int_length = std::stoi(length);
                int int_command = std::stoi(command);
                if(int_command < int_length){
                    this->client_network.send_message_to_server("delete");
                    break;
                }
                else{ 
                    this->client_network.send_message_to_server("keep");
                    break;
                }
            }
        }
    }
}
void ConsoleUi::run_test_case(){
    clear_screen();
    std::string all_tests = this->client_network.receive_message_from_server();

    if (all_tests == "No tests yet!"){
        clear_screen();
        std::cout << "No tests yet!" << '\n';
        std::cout << "Press 0 to go back" << '\n';
        while(1){
            std::string command;
            getline(std::cin, command);
            if (command == "0") {
                return;
            }    
        }
    }
    else {
        std::cout << "Please select the id of the test you want to run or press 00 to go back\n";
        std::cout << all_tests << '\n';
        while(1){
            std::string command;
            getline(std::cin, command);
            if (command == "00"){
                this->client_network.send_message_to_server(command);
                break;
            }
            else{
                this->client_network.send_message_to_server(command);
                std::string length = this->client_network.receive_message_from_server();
                int int_length = std::stoi(length);
                int int_command = std::stoi(command);
                if(int_command < int_length){
                    this->client_network.send_message_to_server("run");
                    break;
                }
                else{ 
                    this->client_network.send_message_to_server("no");
                    break;
                }
            }
        }
    }
}
void ConsoleUi::run_all_tests(){ 
    clear_screen();
    std::string all_tests = this->client_network.receive_message_from_server();

    if (all_tests == "No tests yet!"){
        clear_screen();
        std::cout << "No tests yet!" << '\n';
        std::cout << "Press 0 to go back" << '\n';
        while(1){
            std::string command;
            getline(std::cin, command);
            if (command == "0") {
                return;
            }    
        }
    }
    else {
        std::cout << "Press 1 to confirm run or 0 to go back\n";
        std::cout << all_tests << '\n';
        while(1){
            std::string command;
            getline(std::cin, command);
            if (command == "0"){
                this->client_network.send_message_to_server(command);
                return;
            }
            else if (command == "1"){
                this->client_network.send_message_to_server(command);
                return;
            }
        }
    }
}
void ConsoleUi::show_tests_report(){
    clear_screen();
    time_t T = time(NULL);
    struct tm tm = *localtime(&T);
    std::cout << "User test report, from ";
    std::cout << tm.tm_mday << '.'<< (tm.tm_mon+1) << '.'<< (tm.tm_year+1900) <<'\n';
    std::cout << this->client_network.receive_message_from_server() << '\n';
    std::cout << "Press 0 to go back\n";
    while(1){
        std::string go_back_command;
        getline(std::cin, go_back_command);
        if (go_back_command == "0")
            break;
    }
}
void ConsoleUi::start_ui(){
    while(this->main_user.is_user_logged_in() == false){
        clear_screen();
        std::cout << "\t\t\t\t  WELCOME TO STESTER\n";
        std::cout << "0 - Register\n";
        std::cout << "1 - Login\n";
        std::cout << "2 - Exit\n";
        std::string command;
        getline(std::cin, command);
        if (command == "2"){
            return;
        }
        else{
            this->client_network.send_message_to_server(command);
            if(command == "0"){
                clear_screen();
                std::string username, password;
                std::cout << "Register\n";
                std::cout << "Username: ";
                std::cin >> username;
                this->client_network.send_message_to_server(username);
                std::cout << "Password: ";
                std::cin >> password;
                this->client_network.send_message_to_server(password);
            }
            else if (command == "1"){
                clear_screen();
                std::string username, password;
                std::cout << "Login\n";
                std::cout << "Username: ";
                std::cin >> username;
                this->client_network.send_message_to_server(username);
                std::cout << "Password: ";
                std::cin >> password;
                this->client_network.send_message_to_server(password);
                std::string status = this->client_network.receive_message_from_server();
                std::cout << status << '\n';
                if (status == "exists"){
                    main_user.login();
                }
            }
        }
    }
    while(1) {
        clear_screen();
        std::string command = read_command_from_user();
        std::string valid_command = this->command_service.send_command(command, '7');
        if (valid_command == "0")
            break;
        if (valid_command == "1"){
            show_all_tests();
        }
        else if (valid_command == "2"){
            create_test_case();
        }
        else if (valid_command == "3"){
            delete_test_case();
        }
        else if (valid_command == "4"){
            run_test_case();
        }
        else if (valid_command == "5"){
            run_all_tests();
        }
        else if (valid_command == "6"){
            show_tests_report();
        }
    }
    this->main_user.logout();
}

