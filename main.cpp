#include <sstream>
#include <cstdio>
#include "boolean.h"
#include "exception.h"
#include "indexer.h"
#include "parser.h"

std::string get_string(link_list *);

int main(int argc, char **argv) {
    printf(COLOR_BLUE "pre_check_start\n");
    try {
        if (argc < 3) {
            throw invalid_argument_exception();
        }
    } catch (invalid_argument_exception &e) {
        printf(COLOR_RED "pre_check_failed\n");
        printf("%s", e.what());
        return 1;
    } catch (...) {
        printf(COLOR_RED "pre_check_failed\n");
        return 2;
    }

    std::string file_list = *(argv + 1);
    std::string command_list = *(argv + 2);
    printf(COLOR_GREEN "pre_check_passed\n");

    printf(COLOR_BLUE "program_start\n");

    printf(COLOR_BLUE "file_list_read_start\n");
    std::ifstream ifstream_file_list;
    ifstream_file_list.open(file_list, std::ios::in);
    try {
        if (!ifstream_file_list.is_open()) {
            throw cannot_open_file_exception();
        }
    } catch (cannot_open_file_exception &e) {
        printf("%s", e.what());
        return 3;
    } catch (...) {
        return 4;
    }

    std::vector<std::string> file_name;
    try {
        std::string content;
        while (getline(ifstream_file_list, content)) {
            file_name.emplace_back(content);
        }
        ifstream_file_list.close();
    } catch (...) {
        printf(COLOR_RED "file_list_read_exception\n");
        return 5;
    }
    printf(COLOR_GREEN "file_list_read_end\n");

    printf(COLOR_BLUE "build_index_start\n");
    std::unordered_map<std::string, struct link_list> index;
    try {
        index = indexer(file_name).get_index();
    } catch (...) {
        printf(COLOR_RED "build_index_exception\n");
        return 6;
    }
    printf(COLOR_GREEN "build_index_end\n");

    printf(COLOR_BLUE "command_list_read_start\n");
    std::ifstream ifstream_command_list;
    ifstream_command_list.open(command_list, std::ios::in);
    if (!ifstream_command_list.is_open()) {
        throw cannot_open_file_exception();
    }
    std::vector<std::string> commands;
    try {
        std::string content;
        while (getline(ifstream_command_list, content)) {
            commands.emplace_back(content);
        }
        ifstream_command_list.close();
    } catch (...) {
        printf(COLOR_RED "command_list_read_exception\n");
        return 7;
    }
    printf(COLOR_GREEN "command_list_read_end\n");

    printf(COLOR_BLUE "command_parse_and_execute_start\n");
    std::vector<struct link_list *> result_list;
    for (auto &command: commands) {
        try {
            auto parsed_command = parser(command).get_parsed_command();
            if (parsed_command.empty()) {
                printf(COLOR_YELLOW "maybe_invalid_command\n");
            }
            auto link_list_res = boolean_retrieval(index, parsed_command);
            if (link_list_res->begin_ptr()->file_id == -1) {
                printf(COLOR_YELLOW "maybe_unwanted_result\n");
            }
            result_list.emplace_back(link_list_res);
        } catch (...) {
            printf(COLOR_RED "command_parse_or_execute_exception\n");
            return 8;
        }
    }
    printf(COLOR_GREEN "command_parse_and_execute_end\n");

    printf(COLOR_BLUE "result_write_start\n");
    std::ofstream ofstream_result("result.txt");
    try {
        for (auto &result: result_list) {
            ofstream_result << get_string(result);
        }
        ofstream_result.close();
    } catch (...) {
        printf(COLOR_RED "result_write_exception\n");
        return 9;
    }
    printf(COLOR_GREEN "result_write_end\n");

    printf(COLOR_GREEN "program_end\n");
    return 0;
}

std::string get_string(link_list *list) {
    std::string res;
    auto ptr = list->begin_ptr();
    while (ptr != list->end_ptr()) {
        res.append(std::to_string(ptr->file_id) + ' ');
        ptr = ptr->next;
    }
    res.append("\n");
    return res;
}
