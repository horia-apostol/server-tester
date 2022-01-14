#include "test_case_dns_request_A.h"
#include "server.h"
#include "test_case.h"
#include "netdb.h"
#include "iostream"

TestCaseDnsRequestA::TestCaseDnsRequestA(Server server, std::string test_name)
    : TestCase(server, test_name) {}

void TestCaseDnsRequestA::run_test(){
    hostent *host = gethostbyname(reinterpret_cast<const char*>(this->server.get_server_name().c_str()));
    if (host == NULL)
        this->test_result = 0;
    else {
        this->test_result = 1;
    }
}