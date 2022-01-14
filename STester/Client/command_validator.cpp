#include "command_validator.h"

CommandValidator::CommandValidator() {}
CommandValidator::CommandValidator(std::string command)
    : command{ command } {};

bool CommandValidator::command_is_valid(char32_t max_command){
    if (this->command.size() != 1)
        return false;
    
    if (this->command.at(0) < '0' || this->command.at(0) > max_command)
        return false;

    return true;
}