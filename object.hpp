#pragma once

#include <iostream>
#include <string>

namespace cool {
enum CoolType {
    Void_t=0,
    Bool_t,
    Number_t,
    String_t,
};

class CoolObject{
    public:
        CoolObject() : coolType(CoolType::Void_t) {}
        explicit CoolObject(bool b): coolType{CoolType::Bool_t}, boolean{b} {}
        explicit CoolObject(int n): coolType{CoolType::Number_t}, number{n} {}
        explicit CoolObject(std::string s): coolType{CoolType::String_t}, string{s} {}
        std::string to_string();
        CoolType type() const {return coolType; }
        bool bool_value() const { return boolean; }
        int int_value() const { return number; }
        std::string string_value() const { return string; }

    private:
        CoolType coolType;
        int number;
        bool boolean;
        std::string string;

};
}