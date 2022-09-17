//
// Created by 陈玄 on 2022/9/16.
//

#ifndef SEP_16_INDEXER_H
#define SEP_16_INDEXER_H

#include <unordered_map>
#include <fstream>
#include <sstream>
#include "exception.h"

struct node {
public:
    int file_id = -1;
    struct node *next = nullptr;

    node() = default;

    [[maybe_unused]] explicit node(int);

    virtual ~node() = default;
};

[[maybe_unused]] node::node(int id) {
    this->file_id = id;
    this->next = nullptr;
}

struct link_list {
private:
    int node_count = 0;
    struct node *first = nullptr;
    struct node *last = nullptr;
public:
    link_list();

    virtual ~link_list() = default;

    [[nodiscard]] struct node *begin_ptr() const;

    [[nodiscard]] struct node *end_ptr() const;

    void emplace_back(int);
};

link_list::link_list() {
    auto node = new struct node();
    this->first = node;
    this->last = node;
    ++this->node_count;
}

struct node *link_list::begin_ptr() const {
    return this->first;
}

struct node *link_list::end_ptr() const {
    return this->last->next; // 与 std::vector<T> 迭代器概念保持一致
}

void link_list::emplace_back(int id) {
    if (this->last->file_id == -1 || this->last->file_id == id) {
        this->last->file_id = id;
    } else {
        auto node = new struct node(id);
        this->last->next = node;
        this->last = node;
        ++this->node_count;
    }
}

class indexer {
private:
    std::unordered_map<std::string, struct link_list> index;
public:
    indexer() = default;

    explicit indexer(const std::vector<std::string> &file_name_list);

    virtual ~indexer() = default;

    static void _pre_process(std::string &);

    void _build_index(int id, std::string &);

    void build_index(const std::vector<std::string> &file_name_list);

    [[nodiscard]] std::unordered_map<std::string, struct link_list> get_index() const;
};

indexer::indexer(const std::vector<std::string> &file_name_list) {
    build_index(file_name_list);
}

void indexer::_pre_process(std::string &content) {
    for (char &ch: content) {
        if (ch > 64 && ch < 91) {
            ch += 32;
        }
    }
    content.erase(std::remove_if(content.begin(), content.end(), [&](char ch) -> bool {
        return (ch != ' ') && (ch < 97 || ch > 123);
    }), content.end());
    content.erase(0, content.find_first_not_of(' '));
    content.erase(content.find_last_not_of(' ') + 1, content.length());
    content.push_back(' '); // 这其实是为了处理字符串用的
}

void indexer::_build_index(int id, std::string &content) {
    _pre_process(content);
    auto ptr = content.begin(), end = content.end(), last_pos = ptr;
    while (ptr != end) {
        if (*ptr == ' ') {
            std::string string(last_pos, ptr);
            this->index[string].emplace_back(id);
            while (ptr != end && *ptr == ' ') {
                ++ptr;
            }
            last_pos = ptr;
        } else {
            ++ptr;
        }
    }
}

void indexer::build_index(const std::vector<std::string> &file_name_list) {
    int i = 0;
    for (auto &file_name: file_name_list) {
        std::ifstream ifstream(file_name);
        try {
            if (!ifstream.is_open()) {
                throw cannot_open_file_exception();
            }
        } catch (cannot_open_file_exception(&e)) {
            printf("%s", e.what());
            exit(41);
        } catch (...) {
            exit(42);
        }
        std::stringstream buffer;
        buffer << ifstream.rdbuf();
        std::string content(buffer.str());
        _build_index(i++, content);
        ifstream.close();
    }
}

std::unordered_map<std::string, struct link_list> indexer::get_index() const {
    return this->index;
}

#endif // SEP_16_INDEXER_H
