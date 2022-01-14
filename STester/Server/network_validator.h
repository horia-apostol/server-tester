#include "sys/types.h"
#include "errno.h"
#include "string"
#include "vector"
#include <cstdint>

class NetworkValidator {
    private:
        int64_t task_value;
        std::string task_name;
        bool errors;
    public:
        NetworkValidator(int64_t, std::string);
        void validate_task();
        void show_errors();
};