#pragma once
#include "string"

class HostNameValidator{
    private:
        std::string host_name;
    public:
        HostNameValidator();
        HostNameValidator(std::string);
        bool host_name_is_valid();
};