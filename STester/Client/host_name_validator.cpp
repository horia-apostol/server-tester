#include "host_name_validator.h"
#include "iostream"

bool is_valid_character(char32_t character){
    std::string valids = "1234567890abcdefghijklmnopqrstuvxyz-.";
    for (u_int32_t i=0; i< valids.size(); ++i)
        if (character == valids.at(i))
            return true;
    return false;
}
HostNameValidator::HostNameValidator(){}

HostNameValidator::HostNameValidator(std::string host_name)
    : host_name{ host_name }{}
bool HostNameValidator::host_name_is_valid(){
    if (this->host_name.size() == 0)
        return false;

    /*
    for (u_int32_t i=0; i<this->host_name.size(); ++i){
        if (!is_valid_character(this->host_name.at(i))){
            return false;
        }
            
    }*/
    return true;
}