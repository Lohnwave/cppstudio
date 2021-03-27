/*
 * @Author: Lohnwave
 * @Date: 2021-03-27 14:11:36
 * @LastEditors: Lohnwave
 * @LastEditTime: 2021-03-27 14:54:38
 * @Descripttion: 
 * @version: 
 */
#ifndef UTIL_METHOD_BASE_STRING_UTIL_H
#define UTIL_METHOD_BASE_STRING_UTIL_H
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

namespace util_method {
class StringUtil {
public:
    static inline bool startWith(const std::string& str, const std::string& substr) {
        return str.size() >= substr.size() && std::equal(substr.begin(), substr.end(), str.begin());
    }
    static inline bool endWith(const std::string& str, const std::string& substr) {
        return str.size() >= substr.size() && std::equal(substr.rbegin(), substr.rend(), str.rbegin());
    }
    template <typename container>
    static void split(const std::string& str, char sp, container& result) {
        if (str == "") {
            return;
        }
        std::stringstream ss(str);
        std::string seg;
        while (std::getline(ss, seg, sp)) {
            result.push_back(seg);
        }
        if (str[str.size()-1] == sp || str.size() == 0) {
            result.push_back("");
        }
    }
    template <typename container>
    static void split(const std::string& str, char sp, container* result) {
        if (str == "") {
            return;
        }
        std::stringstream ss(str);
        std::string seg;
        while (std::getline(ss, seg, sp)) {
            result->push_back(seg);
        }
        if (str[str.size()-1] == sp || str.size() == 0) {
            result->push_back("");
        }
    }
};

} // end namespace util_base
#endif // UTIL_METHOD_BASE_STRING_UTIL_H
