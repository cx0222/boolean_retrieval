//
// Created by 陈玄 on 2022/9/17.
//

#ifndef SEP_16_EXCEPTION_H
#define SEP_16_EXCEPTION_H

#include <exception>

#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define TAB "\t"

struct cannot_open_file_exception : std::exception {
    [[nodiscard]] const char *what() const noexcept final {
        return COLOR_RED TAB "cannot_open_file_exception\n";
    }
};

struct invalid_argument_exception : std::exception {
    [[nodiscard]] const char *what() const noexcept final {
        return COLOR_RED TAB "invalid_argument_exception\n";
    }
};

#endif // SEP_16_EXCEPTION_H
