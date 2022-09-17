//
// Created by 陈玄 on 2022/9/16.
//

#ifndef SEP_16_PARSER_H
#define SEP_16_PARSER_H

#include <iostream>
#include <string>
#include <stack>

class parser {
private:
    std::string raw_command_string; // the raw string of parser
    std::string parsed_command_string; // the raw string of parser
public:
    parser() = default;

    explicit parser(const std::string &);

    virtual ~parser() = default;

    void command_parser();

    [[nodiscard]] std::string get_parsed_command() const;
};

parser::parser(const std::string &raw_command_string) {
    this->raw_command_string = raw_command_string;
    command_parser();
}

void parser::command_parser() {
    std::stack<char> operator_stack;
    auto ptr = this->raw_command_string.begin();
    auto end = this->raw_command_string.end();
    while (ptr != end) {
        char ch = *ptr;
        if (ch > 96 && ch < 123) {
            auto last_pos = ptr;
            ++ptr;
            while (ptr != end && *ptr > 96 && *ptr < 123) {
                ++ptr;
            }
            this->parsed_command_string.append(std::string(last_pos, ptr) + ' ');
            continue;
        } else if (ch > 64 && ch < 91) {
            *ptr += 32; // 注意这里不要 ++ptr;
            continue;
        } else if (ch == '|') {
            if (!operator_stack.empty() && operator_stack.top() == '&') {
                this->parsed_command_string += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.push('|');
            ptr += 2;
            continue;
        } else if (ch == '&') {
            operator_stack.push('&');
            ptr += 2;
            continue;
        } else if (ch == '(') {
            operator_stack.push('(');
            ++ptr;
            continue;
        } else if (ch == ')') {
            while (operator_stack.top() != '(') {
                this->parsed_command_string += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.pop();
            ++ptr;
            continue;
        } else {
            ++ptr;
            continue;
        }
    }
    while (!operator_stack.empty()) {
        this->parsed_command_string += operator_stack.top();
        operator_stack.pop();
    }
}

std::string parser::get_parsed_command() const {
    return this->parsed_command_string;
}

#endif // SEP_16_PARSER_H
