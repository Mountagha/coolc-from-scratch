#pragma once

#include <iostream>
#include <string>

namespace cool {
enum CoolType {
    Void=0,
    Bool,
    Number,
    String,
};

class CoolObject{
    public:
        CoolObject() : coolType(CoolType::Void) {}
        explicit CoolObject(bool b): coolType{CoolType::Bool}, boolean{b} {}
        explicit CoolObject(int n): coolType{CoolType::Number}, number{n} {}
        explicit CoolObject(std::string s): coolType{CoolType::String}, string{s} {}

    private:
        CoolType coolType;
        int number;
        bool boolean;
        std::string string;

};
}