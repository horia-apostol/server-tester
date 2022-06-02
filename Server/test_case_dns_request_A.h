#pragma once
#include "test_case.h"

class TestCaseDnsRequestA : public TestCase{
    public:
        TestCaseDnsRequestA(Server, std::string);
        void run_test();
};