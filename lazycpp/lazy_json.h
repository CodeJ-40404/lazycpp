// lazy_json.h
// copyright 2026 lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <optional>
#include <variant>
#include <any>
#include <type_traits>

namespace lazy {
    namespace json {

        // ============================================================
        // 前向声明
        // ============================================================
        class Json;

        // ============================================================
        // JSON 值类型枚举
        // ============================================================
        enum class JsonType {
            Null,
            Bool,
            Number,
            String,
            Array,
            Object
        };

        // ============================================================
        // JSON 异常类
        // ============================================================
        class JsonException : public std::runtime_error {
        public:
            explicit JsonException(const std::string& msg)
                : std::runtime_error("[JSON] " + msg) {}
        };

        // ============================================================
        // JSON 核心类
        // ============================================================
        class Json {
        private:
            // 使用 variant 存储不同类型
            using JsonValue = std::variant<
                std::nullptr_t,           // null
                bool,                     // bool
                double,                   // number
                std::string,              // string
                std::vector<Json>,        // array
                std::map<std::string, Json> // object
            >;

            JsonValue value_;
            JsonType type_;

            // ============================================================
            // 内部辅助函数
            // ============================================================

            template<typename T>
            T& get_value() {
                return std::get<T>(value_);
            }

            template<typename T>
            const T& get_value() const {
                return std::get<T>(value_);
            }

            std::string escape_string(const std::string& s) const {
                std::ostringstream oss;
                for (char c : s) {
                    switch (c) {
                    case '"':  oss << "\\\""; break;
                    case '\\': oss << "\\\\"; break;
                    case '\b': oss << "\\b";  break;
                    case '\f': oss << "\\f";  break;
                    case '\n': oss << "\\n";  break;
                    case '\r': oss << "\\r";  break;
                    case '\t': oss << "\\t";  break;
                    default:
                        if (c < 32) {
                            oss << "\\u" << std::hex << std::setw(4)
                                << std::setfill('0') << (int)c;
                        }
                        else {
                            oss << c;
                        }
                        break;
                    }
                }
                return oss.str();
            }

            std::string unescape_string(const std::string& s) const {
                std::string result;
                for (size_t i = 0; i < s.length(); i++) {
                    if (s[i] == '\\' && i + 1 < s.length()) {
                        char c = s[++i];
                        switch (c) {
                        case '"':  result += '"';  break;
                        case '\\': result += '\\'; break;
                        case '/':  result += '/';  break;
                        case 'b':  result += '\b'; break;
                        case 'f':  result += '\f'; break;
                        case 'n':  result += '\n'; break;
                        case 'r':  result += '\r'; break;
                        case 't':  result += '\t'; break;
                        case 'u': {
                            // 简单 unicode 支持（只支持基本多语言平面）
                            if (i + 4 < s.length()) {
                                std::string hex = s.substr(i + 1, 4);
                                int code = std::stoi(hex, nullptr, 16);
                                result += (char)code;
                                i += 4;
                            }
                            break;
                        }
                        default: result += c; break;
                        }
                    }
                    else {
                        result += s[i];
                    }
                }
                return result;
            }

            std::string serialize_impl(int indent = 0, int spaces = 0, bool pretty = true) const {
                std::ostringstream oss;

                switch (type_) {
                case JsonType::Null:
                    oss << "null";
                    break;

                case JsonType::Bool:
                    oss << (std::get<bool>(value_) ? "true" : "false");
                    break;

                case JsonType::Number: {
                    double num = std::get<double>(value_);
                    if (std::floor(num) == num && !std::isinf(num) && !std::isnan(num)) {
                        // 整数
                        oss << (long long)num;
                    }
                    else {
                        oss << std::setprecision(16) << num;
                    }
                    break;
                }

                case JsonType::String:
                    oss << '"' << escape_string(std::get<std::string>(value_)) << '"';
                    break;

                case JsonType::Array: {
                    const auto& arr = std::get<std::vector<Json>>(value_);
                    oss << '[';
                    if (pretty && !arr.empty()) {
                        oss << '\n';
                    }
                    for (size_t i = 0; i < arr.size(); i++) {
                        if (pretty) {
                            oss << std::string(indent + spaces + 2, ' ');
                        }
                        oss << arr[i].serialize_impl(indent + spaces + 2, spaces, pretty);
                        if (i < arr.size() - 1) {
                            oss << ',';
                        }
                        if (pretty) {
                            oss << '\n';
                        }
                    }
                    if (pretty && !arr.empty()) {
                        oss << std::string(indent + spaces, ' ');
                    }
                    oss << ']';
                    break;
                }

                case JsonType::Object: {
                    const auto& obj = std::get<std::map<std::string, Json>>(value_);
                    oss << '{';
                    bool first = true;
                    if (pretty && !obj.empty()) {
                        oss << '\n';
                    }
                    for (const auto& [key, val] : obj) {
                        if (!first) {
                            oss << ',';
                            if (pretty) oss << '\n';
                        }
                        first = false;
                        if (pretty) {
                            oss << std::string(indent + spaces + 2, ' ');
                        }
                        oss << '"' << escape_string(key) << '"' << (pretty ? ": " : ":");
                        oss << val.serialize_impl(indent + spaces + 2, spaces, pretty);
                    }
                    if (pretty && !obj.empty()) {
                        oss << '\n' << std::string(indent + spaces, ' ');
                    }
                    oss << '}';
                    break;
                }
                }

                return oss.str();
            }

            // 解析辅助函数
            class Parser {
            private:
                std::string text;
                size_t pos = 0;

                void skip_whitespace() {
                    while (pos < text.length() && std::isspace(text[pos])) {
                        pos++;
                    }
                }

                char peek() const {
                    if (pos >= text.length()) return '\0';
                    return text[pos];
                }

                char get() {
                    if (pos >= text.length()) return '\0';
                    return text[pos++];
                }

                bool match(char expected) {
                    if (peek() == expected) {
                        pos++;
                        return true;
                    }
                    return false;
                }

                std::string parse_string() {
                    if (!match('"')) {
                        throw JsonException("Expected '\"' at position " + std::to_string(pos));
                    }

                    std::string result;
                    while (peek() != '"') {
                        if (peek() == '\\') {
                            get(); // 跳过反斜杠
                            char c = get();
                            switch (c) {
                            case '"':  result += '"';  break;
                            case '\\': result += '\\'; break;
                            case '/':  result += '/';  break;
                            case 'b':  result += '\b'; break;
                            case 'f':  result += '\f'; break;
                            case 'n':  result += '\n'; break;
                            case 'r':  result += '\r'; break;
                            case 't':  result += '\t'; break;
                            case 'u': {
                                // 简单的 unicode 支持
                                std::string hex;
                                for (int i = 0; i < 4; i++) {
                                    hex += get();
                                }
                                int code = std::stoi(hex, nullptr, 16);
                                result += (char)code;
                                break;
                            }
                            default: result += c; break;
                            }
                        }
                        else {
                            result += get();
                        }
                    }
                    get(); // 消费结束引号
                    return result;
                }

                Json parse_value() {
                    skip_whitespace();
                    char c = peek();

                    if (c == '"') {
                        return Json(parse_string());
                    }
                    else if (c == 'n') {
                        if (text.compare(pos, 4, "null") == 0) {
                            pos += 4;
                            return Json();
                        }
                        throw JsonException("Expected 'null' at position " + std::to_string(pos));
                    }
                    else if (c == 't') {
                        if (text.compare(pos, 4, "true") == 0) {
                            pos += 4;
                            return Json(true);
                        }
                        throw JsonException("Expected 'true' at position " + std::to_string(pos));
                    }
                    else if (c == 'f') {
                        if (text.compare(pos, 5, "false") == 0) {
                            pos += 5;
                            return Json(false);
                        }
                        throw JsonException("Expected 'false' at position " + std::to_string(pos));
                    }
                    else if (c == '[') {
                        pos++;
                        skip_whitespace();
                        std::vector<Json> arr;
                        if (peek() != ']') {
                            while (true) {
                                arr.push_back(parse_value());
                                skip_whitespace();
                                if (peek() == ']') break;
                                if (!match(',')) {
                                    throw JsonException("Expected ',' or ']' at position " + std::to_string(pos));
                                }
                                skip_whitespace();
                            }
                        }
                        match(']');
                        return Json(arr);
                    }
                    else if (c == '{') {
                        pos++;
                        skip_whitespace();
                        std::map<std::string, Json> obj;
                        if (peek() != '}') {
                            while (true) {
                                skip_whitespace();
                                if (peek() != '"') {
                                    throw JsonException("Expected '\"' for object key at position " + std::to_string(pos));
                                }
                                std::string key = parse_string();
                                skip_whitespace();
                                if (!match(':')) {
                                    throw JsonException("Expected ':' at position " + std::to_string(pos));
                                }
                                skip_whitespace();
                                obj[key] = parse_value();
                                skip_whitespace();
                                if (peek() == '}') break;
                                if (!match(',')) {
                                    throw JsonException("Expected ',' or '}' at position " + std::to_string(pos));
                                }
                            }
                        }
                        match('}');
                        return Json(obj);
                    }
                    else if (c == '-' || std::isdigit(c)) {
                        std::string num_str;
                        if (c == '-') {
                            num_str += get();
                        }
                        while (std::isdigit(peek())) {
                            num_str += get();
                        }
                        if (peek() == '.') {
                            num_str += get();
                            while (std::isdigit(peek())) {
                                num_str += get();
                            }
                        }
                        if (peek() == 'e' || peek() == 'E') {
                            num_str += get();
                            if (peek() == '+' || peek() == '-') {
                                num_str += get();
                            }
                            while (std::isdigit(peek())) {
                                num_str += get();
                            }
                        }
                        try {
                            double num = std::stod(num_str);
                            return Json(num);
                        }
                        catch (...) {
                            throw JsonException("Invalid number format at position " + std::to_string(pos));
                        }
                    }
                    else {
                        throw JsonException("Unexpected character '" + std::string(1, c) +
                            "' at position " + std::to_string(pos));
                    }
                }

            public:
                Json parse(const std::string& json_text) {
                    text = json_text;
                    pos = 0;
                    skip_whitespace();
                    Json result = parse_value();
                    skip_whitespace();
                    if (pos < text.length()) {
                        throw JsonException("Trailing characters at position " + std::to_string(pos));
                    }
                    return result;
                }
            };

            // 辅助函数：从初始化列表构造对象
            static Json from_initializer_list(std::initializer_list<std::pair<const std::string, Json>> init) {
                std::map<std::string, Json> obj;
                for (const auto& [key, val] : init) {
                    obj[key] = val;
                }
                return Json(obj);
            }

        public:
            // ============================================================
            // 构造函数
            // ============================================================

            Json() : value_(nullptr), type_(JsonType::Null) {}

            Json(std::nullptr_t) : value_(nullptr), type_(JsonType::Null) {}

            Json(bool b) : value_(b), type_(JsonType::Bool) {}

            Json(int n) : value_((double)n), type_(JsonType::Number) {}
            Json(long n) : value_((double)n), type_(JsonType::Number) {}
            Json(long long n) : value_((double)n), type_(JsonType::Number) {}
            Json(unsigned int n) : value_((double)n), type_(JsonType::Number) {}
            Json(unsigned long n) : value_((double)n), type_(JsonType::Number) {}
            Json(unsigned long long n) : value_((double)n), type_(JsonType::Number) {}
            Json(float n) : value_((double)n), type_(JsonType::Number) {}
            Json(double n) : value_(n), type_(JsonType::Number) {}

            Json(const char* s) : value_(std::string(s)), type_(JsonType::String) {}
            Json(const std::string& s) : value_(s), type_(JsonType::String) {}
            Json(std::string&& s) : value_(std::move(s)), type_(JsonType::String) {}

            Json(const std::vector<Json>& arr) : value_(arr), type_(JsonType::Array) {}
            Json(std::vector<Json>&& arr) : value_(std::move(arr)), type_(JsonType::Array) {}

            Json(const std::map<std::string, Json>& obj) : value_(obj), type_(JsonType::Object) {}
            Json(std::map<std::string, Json>&& obj) : value_(std::move(obj)), type_(JsonType::Object) {}

            // 方便构造：列表初始化（对象）
            Json(std::initializer_list<std::pair<const std::string, Json>> init)
                : value_(std::map<std::string, Json>(init.begin(), init.end())),
                type_(JsonType::Object) {
            }

            // 复制和移动
            Json(const Json& other) = default;
            Json(Json&& other) noexcept = default;
            Json& operator=(const Json& other) = default;
            Json& operator=(Json&& other) noexcept = default;

            // ============================================================
            // 工厂方法
            // ============================================================

            static Json null() { return Json(); }
            static Json boolean(bool b) { return Json(b); }
            static Json number(double n) { return Json(n); }
            static Json string(const std::string& s) { return Json(s); }
            static Json array() { return Json(std::vector<Json>()); }
            static Json object() { return Json(std::map<std::string, Json>()); }

            // ============================================================
            // 类型检查
            // ============================================================

            JsonType type() const { return type_; }
            bool is_null() const { return type_ == JsonType::Null; }
            bool is_bool() const { return type_ == JsonType::Bool; }
            bool is_number() const { return type_ == JsonType::Number; }
            bool is_string() const { return type_ == JsonType::String; }
            bool is_array() const { return type_ == JsonType::Array; }
            bool is_object() const { return type_ == JsonType::Object; }

            // ============================================================
            // 值获取（带类型检查）
            // ============================================================

            bool as_bool() const {
                if (!is_bool()) throw JsonException("Not a boolean value");
                return std::get<bool>(value_);
            }

            double as_number() const {
                if (!is_number()) throw JsonException("Not a number value");
                return std::get<double>(value_);
            }

            int as_int() const {
                return (int)as_number();
            }

            long long as_int64() const {
                return (long long)as_number();
            }

            std::string as_string() const {
                if (!is_string()) throw JsonException("Not a string value");
                return std::get<std::string>(value_);
            }

            const std::vector<Json>& as_array() const {
                if (!is_array()) throw JsonException("Not an array value");
                return std::get<std::vector<Json>>(value_);
            }

            const std::map<std::string, Json>& as_object() const {
                if (!is_object()) throw JsonException("Not an object value");
                return std::get<std::map<std::string, Json>>(value_);
            }

            // ============================================================
            // 安全获取（带默认值）
            // ============================================================

            bool get_bool(bool default_val = false) const {
                if (is_null()) return default_val;
                if (!is_bool()) throw JsonException("Not a boolean value");
                return std::get<bool>(value_);
            }

            double get_number(double default_val = 0.0) const {
                if (is_null()) return default_val;
                if (!is_number()) throw JsonException("Not a number value");
                return std::get<double>(value_);
            }

            std::string get_string(const std::string& default_val = "") const {
                if (is_null()) return default_val;
                if (!is_string()) throw JsonException("Not a string value");
                return std::get<std::string>(value_);
            }

            // ============================================================
            // 对象访问（类似 Python dict）
            // ============================================================

            Json& operator[](const std::string& key) {
                if (!is_object()) {
                    throw JsonException("Not an object");
                }
                return std::get<std::map<std::string, Json>>(value_)[key];
            }

            const Json& operator[](const std::string& key) const {
                if (!is_object()) {
                    throw JsonException("Not an object");
                }
                const auto& obj = std::get<std::map<std::string, Json>>(value_);
                auto it = obj.find(key);
                if (it == obj.end()) {
                    throw JsonException("Key not found: " + key);
                }
                return it->second;
            }

            // 数组索引访问
            Json& operator[](size_t index) {
                if (!is_array()) {
                    throw JsonException("Not an array");
                }
                auto& arr = std::get<std::vector<Json>>(value_);
                if (index >= arr.size()) {
                    throw JsonException("Index out of range: " + std::to_string(index));
                }
                return arr[index];
            }

            const Json& operator[](size_t index) const {
                if (!is_array()) {
                    throw JsonException("Not an array");
                }
                const auto& arr = std::get<std::vector<Json>>(value_);
                if (index >= arr.size()) {
                    throw JsonException("Index out of range: " + std::to_string(index));
                }
                return arr[index];
            }

            // ============================================================
            // 对象操作
            // ============================================================

            bool has_key(const std::string& key) const {
                if (!is_object()) return false;
                const auto& obj = std::get<std::map<std::string, Json>>(value_);
                return obj.find(key) != obj.end();
            }

            bool contains(const std::string& key) const {
                return has_key(key);
            }

            void erase(const std::string& key) {
                if (!is_object()) {
                    throw JsonException("Not an object");
                }
                std::get<std::map<std::string, Json>>(value_).erase(key);
            }

            void clear() {
                if (is_array()) {
                    std::get<std::vector<Json>>(value_).clear();
                }
                else if (is_object()) {
                    std::get<std::map<std::string, Json>>(value_).clear();
                }
            }

            // ============================================================
            // 数组操作
            // ============================================================

            void push_back(const Json& val) {
                if (!is_array()) {
                    throw JsonException("Not an array");
                }
                std::get<std::vector<Json>>(value_).push_back(val);
            }

            void push_back(Json&& val) {
                if (!is_array()) {
                    throw JsonException("Not an array");
                }
                std::get<std::vector<Json>>(value_).push_back(std::move(val));
            }

            size_t size() const {
                if (is_array()) {
                    return std::get<std::vector<Json>>(value_).size();
                }
                else if (is_object()) {
                    return std::get<std::map<std::string, Json>>(value_).size();
                }
                return 0;
            }

            bool empty() const {
                return size() == 0;
            }

            // ============================================================
            // 序列化
            // ============================================================

            std::string dump(bool pretty = false, int spaces = 2) const {
                if (pretty) {
                    return serialize_impl(0, 0, true);
                }
                else {
                    return serialize_impl(0, 0, false);
                }
            }

            std::string stringify(bool pretty = false, int spaces = 2) const {
                return dump(pretty, spaces);
            }

            // ============================================================
            // 解析
            // ============================================================

            static Json parse(const std::string& json_text) {
                Parser parser;
                return parser.parse(json_text);
            }

            // ============================================================
            // 类型转换辅助
            // ============================================================

            template<typename T>
            T to() const {
                if constexpr (std::is_same_v<T, bool>) {
                    return as_bool();
                }
                else if constexpr (std::is_same_v<T, int>) {
                    return as_int();
                }
                else if constexpr (std::is_same_v<T, long long>) {
                    return as_int64();
                }
                else if constexpr (std::is_same_v<T, double>) {
                    return as_number();
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    return as_string();
                }
                else if constexpr (std::is_same_v<T, std::vector<Json>>) {
                    return as_array();
                }
                else if constexpr (std::is_same_v<T, std::map<std::string, Json>>) {
                    return as_object();
                }
                else {
                    static_assert(std::is_same_v<T, void>, "Unsupported type conversion");
                }
            }

            // ============================================================
            // 迭代器支持
            // ============================================================

            // 对象迭代器
            auto begin() const {
                if (!is_object()) {
                    throw JsonException("Not an object");
                }
                return std::get<std::map<std::string, Json>>(value_).begin();
            }

            auto end() const {
                if (!is_object()) {
                    throw JsonException("Not an object");
                }
                return std::get<std::map<std::string, Json>>(value_).end();
            }

            // 数组迭代器
            auto begin() {
                if (!is_array()) {
                    throw JsonException("Not an array");
                }
                return std::get<std::vector<Json>>(value_).begin();
            }

            auto end() {
                if (!is_array()) {
                    throw JsonException("Not an array");
                }
                return std::get<std::vector<Json>>(value_).end();
            }

            // const 数组迭代器
            auto cbegin() const {
                if (!is_array()) {
                    throw JsonException("Not an array");
                }
                return std::get<std::vector<Json>>(value_).cbegin();
            }

            auto cend() const {
                if (!is_array()) {
                    throw JsonException("Not an array");
                }
                return std::get<std::vector<Json>>(value_).cend();
            }

            // ============================================================
            // 比较运算符
            // ============================================================

            bool operator==(const Json& other) const {
                if (type_ != other.type_) return false;
                return value_ == other.value_;
            }

            bool operator!=(const Json& other) const {
                return !(*this == other);
            }

            // ============================================================
            // 流输出
            // ============================================================

            friend std::ostream& operator<<(std::ostream& os, const Json& json) {
                os << json.dump(true);
                return os;
            }
        };

        // ============================================================
        // 便捷函数
        // ============================================================

        // 快速解析
        inline Json parse_json(const std::string& json_text) {
            return Json::parse(json_text);
        }

        // 快速序列化
        inline std::string to_json(const Json& json, bool pretty = false) {
            return json.dump(pretty);
        }

        // ============================================================
        // 类型转换辅助（从 JSON 到 C++ 类型）
        // ============================================================

        template<typename T>
        T from_json(const Json& json) {
            return json.to<T>();
        }

        template<typename T>
        Json to_json(const T& value) {
            return Json(value);
        }

        // ============================================================
        // 模板特化：vector 转换
        // ============================================================

        template<typename T>
        std::vector<T> json_to_vector(const Json& json) {
            if (!json.is_array()) {
                throw JsonException("Expected array for vector conversion");
            }
            std::vector<T> result;
            for (const auto& item : json.as_array()) {
                result.push_back(from_json<T>(item));
            }
            return result;
        }

        template<typename T>
        Json vector_to_json(const std::vector<T>& vec) {
            std::vector<Json> result;
            for (const auto& item : vec) {
                result.push_back(to_json(item));
            }
            return Json(result);
        }

        // ============================================================
        // 模板特化：map 转换
        // ============================================================

        template<typename V>
        std::map<std::string, V> json_to_map(const Json& json) {
            if (!json.is_object()) {
                throw JsonException("Expected object for map conversion");
            }
            std::map<std::string, V> result;
            for (const auto& [key, val] : json.as_object()) {
                result[key] = from_json<V>(val);
            }
            return result;
        }

        template<typename V>
        Json map_to_json(const std::map<std::string, V>& map) {
            std::map<std::string, Json> result;
            for (const auto& [key, val] : map) {
                result[key] = to_json(val);
            }
            return Json(result);
        }

#ifdef LAZY_EXPORT_ALL
        using namespace json;
#endif

    } // namespace json
} // namespace lazy