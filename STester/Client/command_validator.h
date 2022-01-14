#pragma once
#include "string"

class CommandValidator{
    private:
        std::string command;
    public:
        CommandValidator();
        CommandValidator(std::string);
        bool command_is_valid(char32_t max_command);
};