#include "network_validator.h"
#include "sys/types.h"
#include "string"
#include "vector"
#include "iostream"
#include "errno.h"

NetworkValidator::NetworkValidator(int64_t task_value, std::string task_name)
    : task_value{ task_value }, task_name{ task_name }, errors{ false } {}

void NetworkValidator::validate_task() {
    if (this->task_value == -1)
        this->errors = true;  
}
void NetworkValidator::show_errors(){
    if (this->errors == true)
        std::cout << task_name << " Error: " << errno << "!\n";
}

