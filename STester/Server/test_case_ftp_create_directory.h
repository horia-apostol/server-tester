#pragma once
#include "test_case.h"

class TestCaseFtpCreateDirectory : public TestCase{
    public:
        TestCaseFtpCreateDirectory (Server, std::string);
        void run_test();
};