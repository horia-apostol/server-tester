#pragma once
#include "test_case.h"

class TestCaseHttpDownload: public TestCase{
    public:
        TestCaseHttpDownload(Server, std::string);
        void run_test();
};