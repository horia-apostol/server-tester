#pragma once
#include "server_network.h"
#include "tests_repo.h"

class ServerController{
    private:
        ServerNetwork network;
        TestRepo repository;
        void controller_show_all_tests();
        void controller_create_test_case();
        void controller_delete_test_case();
        void controller_run_test_case();
        void controller_run_all_tests();
        void controller_show_test_results();
    public:
        ServerController(ServerNetwork, TestRepo);
        void start_controller();

};