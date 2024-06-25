#pragma once

#define DEBUG_PRINT_CODE

class cool_runtime_error: std::runtime_error {
    using std::runtime_error::runtime_error;
};