#pragma once
#include "string"
#include "server.h"

class TestCase {
    public:
        std::string test_name;
        Server server;
        int test_result;
    public:
        TestCase(Server, std::string);
        std::string get_test_name();
        void set_test_name(std::string);
        int get_test_result();
        virtual void run_test() = 0;
};