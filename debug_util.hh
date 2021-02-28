#pragma once

#include <iostream>
#include <string>

namespace debug_util {

template <typename ArrayType>
void print_array(const std::string &label, const ArrayType &a) {
    std::cout << label << ": ";
    for (const auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}

}  // namespace debug_util
