#pragma once
#include "test_case.h"
#include "string"
#include "vector"
#include <cstddef>

class TestRepo {
    private:
        std::vector<TestCase*> repository;
        int size;
    public:
        TestRepo();
        std::vector<std::string> get_all_tests();
        int get_size();
        void create_new_test(TestCase*);
        void delete_test(int);
        void run_test_case(int);
        void run_all_tests();
        std::vector<int> get_tests_report();
};