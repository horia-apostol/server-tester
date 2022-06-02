#pragma once
#include "test_case.h"

class TestCaseLatency : public TestCase{
    public:
        double latency;
        TestCaseLatency(Server, std::string);
        void run_test();
        double get_latency();
};