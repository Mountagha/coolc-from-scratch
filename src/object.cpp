#include "object.hpp"

namespace cool {

std::string CoolObject::to_string() {
    switch(coolType) {
        case Void_t:
            return "";
        case Bool_t:
            return boolean ? "true" : "false";
        case Number_t:
            return std::to_string(number);
        case String_t:
            return string;
        default:
            return "unknown";
    }
}
}