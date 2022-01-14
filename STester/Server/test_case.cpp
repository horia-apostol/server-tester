#include "test_case.h"

TestCase::TestCase(Server server, std::string test_name) 
    : server{ server }, test_name{test_name}, test_result{ 2 } {}

std::string TestCase::get_test_name(){
    return this->test_name;
}
void TestCase::set_test_name(std::string new_name){
    this->test_name = new_name;
}
int TestCase::get_test_result(){
    return this->test_result;
}
