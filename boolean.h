//
// Created by 陈玄 on 2022/9/16.
//

#ifndef SEP_16_BOOLEAN_H
#define SEP_16_BOOLEAN_H

#include <vector>
#include "indexer.h"

struct link_list *boolean_and_1(std::unordered_map<std::string, struct link_list> &map,
                                const struct link_list &list1, const struct link_list &list2) {
    auto res = new struct link_list();
    struct node *list1_ptr = list1.begin_ptr(), *list2_ptr = list2.begin_ptr();
    while (list1_ptr && list2_ptr) {
        int list1_id = list1_ptr->file_id, list2_id = list2_ptr->file_id;
        if (list1_id == list2_id) {
            res->emplace_back(list1_id);
            list1_ptr = list1_ptr->next;
            list2_ptr = list2_ptr->next;
        } else if (list1_id > list2_id) {
            list2_ptr = list2_ptr->next;
        } else {
            list1_ptr = list1_ptr->next;
        }
    }
    return res;
}

struct link_list *boolean_and(std::unordered_map<std::string, struct link_list> &map,
                              const std::string &str1, const std::string &str2) {
    link_list &list1 = map[str1], &list2 = map[str2];
    return boolean_and_1(map, list1, list2);
}

struct link_list *boolean_or_1(std::unordered_map<std::string, struct link_list> &map,
                               const struct link_list &list1, const struct link_list &list2) {
    auto res = new struct link_list();
    struct node *list1_ptr = list1.begin_ptr(), *list2_ptr = list2.begin_ptr();
    while (list1_ptr && list2_ptr) {
        int list1_id = list1_ptr->file_id, list2_id = list2_ptr->file_id;
        if (list1_id == list2_id) {
            res->emplace_back(list1_id);
            list1_ptr = list1_ptr->next;
            list2_ptr = list2_ptr->next;
        } else if (list1_id > list2_id) {
            res->emplace_back(list2_id);
            list2_ptr = list2_ptr->next;
        } else {
            res->emplace_back(list1_id);
            list1_ptr = list1_ptr->next;
        }
    }
    while (list1_ptr) {
        res->emplace_back(list1_ptr->file_id);
        list1_ptr = list1_ptr->next;
    }
    while (list2_ptr) {
        res->emplace_back(list2_ptr->file_id);
        list2_ptr = list2_ptr->next;
    }
    return res;
}

struct link_list *boolean_or(std::unordered_map<std::string, struct link_list> &map,
                             const std::string &str1, const std::string &str2) {
    link_list &list1 = map[str1], &list2 = map[str2];
    return boolean_or_1(map, list1, list2);
}

struct link_list *
boolean_retrieval(std::unordered_map<std::string, struct link_list> &map, const std::string &command) {
    std::stack<struct link_list *> word_stack;
    auto ptr = command.begin(), end = command.end();
    while (ptr != end) {
        char ch = *ptr;
        if (ch > 96 && ch < 123) {
            auto last_pos = ptr;
            ++ptr;
            while (ptr != end && *ptr > 96 && *ptr < 123) {
                ++ptr;
            }
            std::string word(last_pos, ptr);
            word_stack.push(&map[word]);
            continue;
        } else if (ch == '&' || ch == '|') {
            auto list1 = *word_stack.top();
            word_stack.pop();
            auto list2 = *word_stack.top();
            word_stack.pop();
            if (ch == '&') {
                word_stack.push(boolean_and_1(map, list2, list1));
            } else {
                word_stack.push(boolean_or_1(map, list2, list1));
            }
            ++ptr;
            continue;
        } else {
            ++ptr;
        }
    }
    return word_stack.top();
}

#endif // SEP_16_BOOLEAN_H
