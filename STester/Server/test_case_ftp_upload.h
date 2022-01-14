#pragma once
#include "test_case.h"

class TestCaseFtpUpload : public TestCase{
    public:
        TestCaseFtpUpload(Server, std::string);
        void run_test();
};