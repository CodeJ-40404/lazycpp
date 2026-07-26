// lazy_string.h
// copyright 2026 ? lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>
#include <iomanip>

namespace lazy {
    namespace string {

        // ============================================================
        // 大小写转换
        // ============================================================

        inline std::string to_lower(std::string s) {
            for (char& c : s) c = std::tolower(c);
            return s;
        }

        inline std::string to_upper(std::string s) {
            for (char& c : s) c = std::toupper(c);
            return s;
        }

        // 首字母大写
        inline std::string capitalize(std::string s) {
            if (!s.empty()) {
                s[0] = std::toupper(s[0]);
            }
            return s;
        }

        // 每个单词首字母大写
        inline std::string title_case(std::string s) {
            bool new_word = true;
            for (char& c : s) {
                if (std::isspace(c)) {
                    new_word = true;
                }
                else if (new_word) {
                    c = std::toupper(c);
                    new_word = false;
                }
                else {
                    c = std::tolower(c);
                }
            }
            return s;
        }

        // ============================================================
        // 去除空白
        // ============================================================

        inline std::string trim(const std::string& s) {
            size_t start = s.find_first_not_of(" \t\n\r\f\v");
            if (start == std::string::npos) return "";
            size_t end = s.find_last_not_of(" \t\n\r\f\v");
            return s.substr(start, end - start + 1);
        }

        inline std::string trim_left(const std::string& s) {
            size_t start = s.find_first_not_of(" \t\n\r\f\v");
            return start == std::string::npos ? "" : s.substr(start);
        }

        inline std::string trim_right(const std::string& s) {
            size_t end = s.find_last_not_of(" \t\n\r\f\v");
            return end == std::string::npos ? "" : s.substr(0, end + 1);
        }

        // ============================================================
        // 分割与连接
        // ============================================================

        // 分割字符串（支持多个分隔符）
        inline std::vector<std::string> split(const std::string& s,
            const std::string& delimiters = " \t\n",
            bool skip_empty = true) {
            std::vector<std::string> result;
            size_t start = 0;
            while (start < s.length()) {
                size_t end = s.find_first_of(delimiters, start);
                if (end == std::string::npos) {
                    if (!skip_empty || start < s.length()) {
                        result.push_back(s.substr(start));
                    }
                    break;
                }
                if (!skip_empty || end > start) {
                    result.push_back(s.substr(start, end - start));
                }
                start = end + 1;
            }
            return result;
        }

        // 按单个字符分割
        inline std::vector<std::string> split_char(const std::string& s, char delim, bool skip_empty = true) {
            std::vector<std::string> result;
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                if (!skip_empty || !item.empty()) {
                    result.push_back(item);
                }
            }
            return result;
        }

        // 连接字符串
        inline std::string join(const std::vector<std::string>& parts, const std::string& separator = " ") {
            if (parts.empty()) return "";
            std::string result = parts[0];
            for (size_t i = 1; i < parts.size(); i++) {
                result += separator + parts[i];
            }
            return result;
        }

        // ============================================================
        // 查找与替换
        // ============================================================

        // 检查字符串是否包含子串
        inline bool contains(const std::string& s, const std::string& sub) {
            return s.find(sub) != std::string::npos;
        }

        // 检查字符串是否以某个前缀开头
        inline bool starts_with(const std::string& s, const std::string& prefix) {
            return s.compare(0, prefix.size(), prefix) == 0;
        }

        // 检查字符串是否以某个后缀结尾
        inline bool ends_with(const std::string& s, const std::string& suffix) {
            if (suffix.size() > s.size()) return false;
            return s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
        }

        // 替换所有
        inline std::string replace_all(std::string s, const std::string& from, const std::string& to) {
            if (from.empty()) return s;
            size_t pos = 0;
            while ((pos = s.find(from, pos)) != std::string::npos) {
                s.replace(pos, from.length(), to);
                pos += to.length();
            }
            return s;
        }

        // 替换第一个
        inline std::string replace_first(std::string s, const std::string& from, const std::string& to) {
            size_t pos = s.find(from);
            if (pos != std::string::npos) {
                s.replace(pos, from.length(), to);
            }
            return s;
        }

        // ============================================================
        // 字符串反转
        // ============================================================

        inline std::string reverse(const std::string& s) {
            std::string result = s;
            std::reverse(result.begin(), result.end());
            return result;
        }

        // ============================================================
        // 重复字符串
        // ============================================================

        inline std::string repeats(const std::string& s, int n) {
            if (n <= 0) return "";
            std::string result;
            result.reserve(s.size() * n);
            for (int i = 0; i < n; i++) {
                result += s;
            }
            return result;
        }

        // ============================================================
        // 填充/对齐
        // ============================================================

        inline std::string pad_left(std::string s, size_t width, char fill = ' ') {
            if (s.size() >= width) return s;
            return std::string(width - s.size(), fill) + s;
        }

        inline std::string pad_right(std::string s, size_t width, char fill = ' ') {
            if (s.size() >= width) return s;
            return s + std::string(width - s.size(), fill);
        }

        inline std::string pad_center(std::string s, size_t width, char fill = ' ') {
            if (s.size() >= width) return s;
            size_t total = width - s.size();
            size_t left = total / 2;
            size_t right = total - left;
            return std::string(left, fill) + s + std::string(right, fill);
        }

        // ============================================================
        // 类型转换（字符串 ? 其他类型）
        // ============================================================

        template<typename T>
        std::string to_string(const T& val) {
            std::ostringstream oss;
            oss << val;
            return oss.str();
        }

        template<typename T>
        T from_string(const std::string& s) {
            std::istringstream iss(s);
            T val;
            iss >> val;
            return val;
        }

        // 特殊：布尔值转字符串
        inline std::string bool_to_string(bool b, const std::string& true_str = "true",
            const std::string& false_str = "false") {
            return b ? true_str : false_str;
        }

        // ============================================================
        // 验证函数
        // ============================================================

        // 检查是否为数字
        inline bool is_number(const std::string& s) {
            if (s.empty()) return false;
            size_t start = 0;
            if (s[0] == '-' || s[0] == '+') {
                if (s.size() == 1) return false;
                start = 1;
            }
            bool has_dot = false;
            for (size_t i = start; i < s.size(); i++) {
                if (s[i] == '.') {
                    if (has_dot) return false;
                    has_dot = true;
                }
                else if (!std::isdigit(s[i])) {
                    return false;
                }
            }
            return true;
        }

        // 检查是否为整数
        inline bool is_integer(const std::string& s) {
            if (s.empty()) return false;
            size_t start = 0;
            if (s[0] == '-' || s[0] == '+') {
                if (s.size() == 1) return false;
                start = 1;
            }
            for (size_t i = start; i < s.size(); i++) {
                if (!std::isdigit(s[i])) return false;
            }
            return true;
        }

        // 检查是否为字母数字
        inline bool is_alnum(const std::string& s) {
            return std::all_of(s.begin(), s.end(), [](char c) {
                return std::isalnum(c);
                });
        }

        // 检查是否只包含字母
        inline bool is_alpha(const std::string& s) {
            return std::all_of(s.begin(), s.end(), [](char c) {
                return std::isalpha(c);
                });
        }

        // ============================================================
        // 随机字符串
        // ============================================================

        inline std::string random_string(int length, const std::string& chars =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") {
            static std::mt19937 gen(
                (unsigned)std::chrono::steady_clock::now().time_since_epoch().count()
            );
            std::uniform_int_distribution<> dis(0, chars.size() - 1);
            std::string result;
            result.reserve(length);
            for (int i = 0; i < length; i++) {
                result += chars[dis(gen)];
            }
            return result;
        }

        // ============================================================
        // 格式化输出（类似 Python 的 format）
        // ============================================================

        template<typename... Args>
        std::string format(const std::string& fmt, Args&&... args) {
            std::vector<std::string> arg_strs = { to_string(std::forward<Args>(args))... };
            std::string result = fmt;
            for (size_t i = 0; i < arg_strs.size(); i++) {
                std::string placeholder = "{" + std::to_string(i) + "}";
                result = replace_all(result, placeholder, arg_strs[i]);
            }
            return result;
        }

    } // namespace string

    //using namespace string;
    //不规范
#ifdef LAZY_EXPORT_ALL
    using namespace string;
#endif
} // namespace lazy