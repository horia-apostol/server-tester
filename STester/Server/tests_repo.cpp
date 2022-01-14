#include "tests_repo.h"
#include "test_case.h"
#include "vector"


TestRepo::TestRepo()
    : size{ 0 }{}
std::vector<std::string> TestRepo::get_all_tests() { 
    std::vector<std::string> all_tests;
    if (this->size == 0)
        return all_tests;
    for (int test_id=0 ; test_id < this->size; ++test_id) {
        all_tests.push_back(this->repository.at(test_id)->get_test_name());
    }
    return all_tests;
}
int TestRepo::get_size(){
    return this->size;
}
void TestRepo::create_new_test(TestCase* test_case) {
    this->repository.push_back(test_case);
    this->size++;
}
void TestRepo::delete_test(int test_id) {
    this->repository.erase(this->repository.begin() + test_id);
    this->size--;
}
void TestRepo::run_test_case(int test_id) {
    this->repository.at(test_id)->run_test();
}
void TestRepo::run_all_tests() {
    for (int test_id=0 ; test_id< this->size; ++test_id) 
        this->repository.at(test_id)->run_test();
}
std::vector<int> TestRepo::get_tests_report() {
    std::vector<int> all_test_results;
    for (int test_id=0 ; test_id< this->size; ++test_id) 
        all_test_results.push_back(this->repository.at(test_id)->get_test_result());
    return all_test_results;
}
