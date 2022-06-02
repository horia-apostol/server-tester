#include "service.h"
#include "server_network.h"
#include "tests_repo.h"
#include "test_case.h"
#include "test_case_latency.h"
#include "test_case_dns_request_A.h"
#include "test_case_http_download.h"
#include "test_case_ftp_create_directory.h"
#include "test_case_ftp_upload.h"
#include "string"
#include "stdio.h"
#include "fstream"

std::ofstream out("history.txt");
ServerController::ServerController(ServerNetwork network, TestRepo repository)
    : network{ network }, repository{ repository }{}
    
void ServerController::start_controller(){
    while (1) {
        std::string message_from_client = this->network.receive_message_from_client();
        if (message_from_client == "1") {
            controller_show_all_tests();
        }
        else if (message_from_client == "2") {
            controller_create_test_case();
        }
        else if (message_from_client == "3") {
            controller_delete_test_case();
        }
        else if (message_from_client == "4") {
            controller_run_test_case();
        }
        else if (message_from_client == "5") {
            controller_run_all_tests();
        }
        else if (message_from_client == "6"){
            controller_show_test_results();
        }
    }
}
void ServerController::controller_show_all_tests(){
    std::string to_send = "";
        std::vector<std::string> all_tests = this->repository.get_all_tests();
        if (this->repository.get_size() == 0){
            to_send += "No tests yet!";
        }
        else{
            for (int test_id=0 ; test_id < all_tests.size(); ++test_id)
                to_send += "ID: " + std::to_string(test_id) + "---" + all_tests.at(test_id) + "\n";
        }  
        this->network.send_message_to_client(to_send);
}
void ServerController::controller_create_test_case(){
    std::string command = this->network.receive_message_from_client();
    if (command == "0")
        return;
    else {
        std::string test_name = this->network.receive_message_from_client();
        std::string host_name = this->network.receive_message_from_client();
        Server server_to_test(host_name);
        if (command == "1"){
            this->repository.create_new_test(new TestCaseLatency(server_to_test, test_name));
        }
        else if(command == "2"){
            this->repository.create_new_test(new TestCaseDnsRequestA(server_to_test, test_name));
        }
        else if(command == "3"){
            this->repository.create_new_test(new TestCaseHttpDownload(server_to_test, test_name));
        }
        else if(command == "4"){
            this->repository.create_new_test(new TestCaseFtpCreateDirectory(server_to_test, test_name));
        }
        else if(command == "5"){
            this->repository.create_new_test(new TestCaseFtpUpload(server_to_test, test_name));
        }
    }
}
void ServerController::controller_delete_test_case(){
    std::vector<std::string> all_tests = this->repository.get_all_tests();
    if (this->repository.get_size() == 0){
        std::string to_send = "";
        to_send += "No tests yet!";
        this->network.send_message_to_client(to_send);
        return;
    }
    else{
        std::string to_send = "";
        for (int test_id=0 ; test_id < all_tests.size(); ++test_id)
            to_send += "ID: " + std::to_string(test_id) + "---" + all_tests.at(test_id) + "\n";
        this->network.send_message_to_client(to_send);
    }  
    std::string id = this->network.receive_message_from_client();
    if (id == "00"){
        return;
    }
    else {
        this->network.send_message_to_client(std::to_string(this->repository.get_size()));
        std::string action = this->network.receive_message_from_client();
        if (action == "delete"){
            this->repository.delete_test(std::stoi(id));
        }
        return;    
    }

}
void ServerController::controller_run_test_case(){
    std::vector<std::string> all_tests = this->repository.get_all_tests();
    if (this->repository.get_size() == 0){
        std::string to_send = "";
        to_send += "No tests yet!";
        network.send_message_to_client(to_send);
        return;
    }
    else{
        std::string to_send = "";
        for (int test_id=0 ; test_id < all_tests.size(); ++test_id)
            to_send += "ID: " + std::to_string(test_id) + "---" + all_tests.at(test_id) + "\n";
        this->network.send_message_to_client(to_send);
    }  
    std::string id = this->network.receive_message_from_client();
    if (id == "00"){
        return;
    }
    else {
        network.send_message_to_client(std::to_string(this->repository.get_size()));
        std::string action = this->network.receive_message_from_client();
        if (action == "run"){
            this->repository.run_test_case(std::stoi(id));       
        }
        return;    
    }
}
void ServerController::controller_run_all_tests(){
    std::vector<std::string> all_tests = this->repository.get_all_tests();
    if (this->repository.get_size() == 0){
        std::string to_send = "";
        to_send += "No tests yet!";
        this->network.send_message_to_client(to_send);
        return;
    }
    else{
        std::string to_send = "";
        for (int test_id=0 ; test_id < all_tests.size(); ++test_id)
            to_send += "ID: " + std::to_string(test_id) + "---" + all_tests.at(test_id) + "\n";
        this->network.send_message_to_client(to_send);
        std::string id = this->network.receive_message_from_client();
        if (id == "0"){
            return;
        }
        else if (id == "1"){
            this->repository.run_all_tests();
            return;    
        }
    }  
    
}
void ServerController::controller_show_test_results(){
    std::vector<std::string> all_tests = this->repository.get_all_tests();
    std::vector<int> all_test_results = this->repository.get_tests_report();
    if (this->repository.get_size() == 0){
        std::string to_send = "";
        to_send += "No tests yet!";
        this->network.send_message_to_client(to_send);
        return;
    }
    else{
        std::string to_send = "";
        for (int test_id=0 ; test_id < all_test_results.size(); ++test_id) {
            if (all_test_results.at(test_id) == 0)
                to_send += "ID: " + std::to_string(test_id) + "---" + all_tests.at(test_id) + " ------------- FAILED" + '\n';
            else if (all_test_results.at(test_id) == 1)
                to_send += "ID: " + std::to_string(test_id) + "---" + all_tests.at(test_id) + " ------------- PASSED" + '\n';
            else if (all_test_results.at(test_id) == 2)
                to_send += "ID: " + std::to_string(test_id) + "---" + all_tests.at(test_id) + " ------------- UNTESTED" + '\n';
        }
        
        out << to_send;
        this->network.send_message_to_client(to_send);
        return;
    }
}
