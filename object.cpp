#include "object.hpp"

namespace cool {

std::string CoolObject::to_string() {
    switch(coolType) {
        case Void:
            return "";
        case Bool:
            return boolean ? "true" : "false";
        case Number:
            return std::to_string(number);
        case String:
            return string;
        default:
            return "unknown";
    }
}
}