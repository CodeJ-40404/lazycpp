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

#ifdef _WIN32
#include <io.h>
#include <sys/stat.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace lazy {
    namespace json {

        // ============================================================
        // 前向声明
        // ============================================================
        class Json;
        class JsonFile;

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
        // 文件 I/O 异常类
        // ============================================================
        class JsonFileException : public JsonException {
        public:
            explicit JsonFileException(const std::string& msg)
                : JsonException("File IO: " + msg) {
            }
        };

        // ============================================================
        // 文件操作选项
        // ============================================================
        enum class FileMode {
            Text,
            Binary
        };

        struct FileOptions {
            bool pretty = true;
            int indent_spaces = 2;
            FileMode mode = FileMode::Text;
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
                        if (static_cast<unsigned char>(c) < 32) {
                            oss << "\\u" << std::hex << std::setw(4)
                                << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
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
                            if (i + 4 < s.length()) {
                                std::string hex = s.substr(i + 1, 4);
                                int code = std::stoi(hex, nullptr, 16);
                                result += static_cast<char>(code);
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
                        oss << static_cast<long long>(num);
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
                            if (pretty) oss << ' ';
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
                        oss << '"' << escape_string(key) << '"';
                        if (pretty) {
                            oss << ": ";
                        }
                        else {
                            oss << ":";
                        }
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
                    while (pos < text.length() && std::isspace(static_cast<unsigned char>(text[pos]))) {
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
                            get();
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
                                std::string hex;
                                for (int i = 0; i < 4; i++) {
                                    hex += get();
                                }
                                int code = std::stoi(hex, nullptr, 16);
                                result += static_cast<char>(code);
                                break;
                            }
                            default: result += c; break;
                            }
                        }
                        else {
                            result += get();
                        }
                    }
                    get();
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
                    else if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
                        std::string num_str;
                        if (c == '-') {
                            num_str += get();
                        }
                        while (std::isdigit(static_cast<unsigned char>(peek()))) {
                            num_str += get();
                        }
                        if (peek() == '.') {
                            num_str += get();
                            while (std::isdigit(static_cast<unsigned char>(peek()))) {
                                num_str += get();
                            }
                        }
                        if (peek() == 'e' || peek() == 'E') {
                            num_str += get();
                            if (peek() == '+' || peek() == '-') {
                                num_str += get();
                            }
                            while (std::isdigit(static_cast<unsigned char>(peek()))) {
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

        public:
            // ============================================================
            // 构造函数
            // ============================================================

            Json() : value_(nullptr), type_(JsonType::Null) {}

            Json(std::nullptr_t) : value_(nullptr), type_(JsonType::Null) {}

            Json(bool b) : value_(b), type_(JsonType::Bool) {}

            Json(int n) : value_(static_cast<double>(n)), type_(JsonType::Number) {}
            Json(long n) : value_(static_cast<double>(n)), type_(JsonType::Number) {}
            Json(long long n) : value_(static_cast<double>(n)), type_(JsonType::Number) {}
            Json(unsigned int n) : value_(static_cast<double>(n)), type_(JsonType::Number) {}
            Json(unsigned long n) : value_(static_cast<double>(n)), type_(JsonType::Number) {}
            Json(unsigned long long n) : value_(static_cast<double>(n)), type_(JsonType::Number) {}
            Json(float n) : value_(static_cast<double>(n)), type_(JsonType::Number) {}
            Json(double n) : value_(n), type_(JsonType::Number) {}

            Json(const char* s) : value_(std::string(s)), type_(JsonType::String) {}
            Json(const std::string& s) : value_(s), type_(JsonType::String) {}
            Json(std::string&& s) : value_(std::move(s)), type_(JsonType::String) {}

            Json(const std::vector<Json>& arr) : value_(arr), type_(JsonType::Array) {}
            Json(std::vector<Json>&& arr) : value_(std::move(arr)), type_(JsonType::Array) {}

            Json(const std::map<std::string, Json>& obj) : value_(obj), type_(JsonType::Object) {}
            Json(std::map<std::string, Json>&& obj) : value_(std::move(obj)), type_(JsonType::Object) {}

            Json(std::initializer_list<std::pair<const std::string, Json>> init)
                : value_(std::map<std::string, Json>(init.begin(), init.end())),
                type_(JsonType::Object) {
            }

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
                return static_cast<int>(as_number());
            }

            long long as_int64() const {
                return static_cast<long long>(as_number());
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
            // 对象访问
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

            std::optional<Json> find(const std::string& key) const {
                if (!is_object()) {
                    return std::nullopt;
                }
                const auto& obj = std::get<std::map<std::string, Json>>(value_);
                auto it = obj.find(key);
                if (it == obj.end()) {
                    return std::nullopt;
                }
                return it->second;
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
                return serialize_impl(0, 0, pretty);
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
            // 文件 I/O（声明，实现在 JsonFile 中）
            // ============================================================

            void save(const std::string& filename, bool pretty = true, int indent = 2) const;
            void save(const std::string& filename, const FileOptions& options) const;
            static Json load(const std::string& filename);
            static Json load(const std::string& filename, std::string& error_msg);
            static Json load_or_create(const std::string& filename,
                const Json& default_value = Json::object());

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
        // JsonFile 类实现
        // ============================================================
        class JsonFile {
        private:
            JsonFile() = delete;

            static std::string read_file_content(const std::string& filename) {
#ifdef _WIN32
                FILE* file = nullptr;
                if (fopen_s(&file, filename.c_str(), "rb") != 0) {
                    throw JsonFileException("Cannot open file: " + filename);
                }
#else
                FILE* file = fopen(filename.c_str(), "rb");
                if (!file) {
                    throw JsonFileException("Cannot open file: " + filename);
                }
#endif

                fseek(file, 0, SEEK_END);
                long size = ftell(file);
                fseek(file, 0, SEEK_SET);

                if (size < 0) {
                    fclose(file);
                    throw JsonFileException("Cannot determine file size: " + filename);
                }

                std::string content;
                content.resize(static_cast<size_t>(size));
                size_t read_count = fread(&content[0], 1, static_cast<size_t>(size), file);
                fclose(file);

                if (read_count != static_cast<size_t>(size)) {
                    throw JsonFileException("Read error: " + filename);
                }

                return content;
            }

            static void write_file_content(const std::string& filename,
                const std::string& content) {
#ifdef _WIN32
                FILE* file = nullptr;
                if (fopen_s(&file, filename.c_str(), "w") != 0) {
                    throw JsonFileException("Cannot create file: " + filename);
                }
#else
                FILE* file = fopen(filename.c_str(), "w");
                if (!file) {
                    throw JsonFileException("Cannot create file: " + filename);
                }
#endif

                size_t written = fwrite(content.c_str(), 1, content.size(), file);
                fclose(file);

                if (written != content.size()) {
                    throw JsonFileException("Write error: " + filename);
                }
            }

        public:
            static Json read(const std::string& filename) {
                std::string content = read_file_content(filename);
                return Json::parse(content);
            }

            static Json read(const std::string& filename, std::string& error_msg) {
                try {
                    std::string content = read_file_content(filename);
                    return Json::parse(content);
                }
                catch (const std::exception& e) {
                    error_msg = e.what();
                    return Json();
                }
            }

            static void write(const std::string& filename, const Json& json,
                bool pretty = true, int indent = 2) {
                std::string content = json.dump(pretty, indent);
                write_file_content(filename, content);
            }

            static void write(const std::string& filename, const Json& json,
                const FileOptions& options) {
                std::string content = json.dump(options.pretty, options.indent_spaces);
                write_file_content(filename, content);
            }

            static void append(const std::string& filename, const std::string& key,
                const Json& value) {
                Json root;
                if (file_exists(filename)) {
                    root = read(filename);
                }
                else {
                    root = Json::object();
                }

                if (!root.is_object()) {
                    throw JsonFileException("Cannot append to non-object JSON");
                }

                root[key] = value;
                write(filename, root);
            }

            static void append(const std::string& filename,
                const std::map<std::string, Json>& values) {
                Json root;
                if (file_exists(filename)) {
                    root = read(filename);
                }
                else {
                    root = Json::object();
                }

                if (!root.is_object()) {
                    throw JsonFileException("Cannot append to non-object JSON");
                }

                for (const auto& [key, val] : values) {
                    root[key] = val;
                }

                write(filename, root);
            }

            static void append_array(const std::string& filename, const Json& value) {
                Json root;
                if (file_exists(filename)) {
                    root = read(filename);
                }
                else {
                    root = Json::array();
                }

                if (!root.is_array()) {
                    throw JsonFileException("Cannot append to non-array JSON");
                }

                root.push_back(value);
                write(filename, root);
            }

            static bool file_exists(const std::string& filename) {
#ifdef _WIN32
                return _access(filename.c_str(), 0) == 0;
#else
                return access(filename.c_str(), F_OK) == 0;
#endif
            }

            static size_t file_size(const std::string& filename) {
#ifdef _WIN32
                struct _stat st;
                if (_stat(filename.c_str(), &st) == 0) {
                    return static_cast<size_t>(st.st_size);
                }
#else
                struct stat st;
                if (stat(filename.c_str(), &st) == 0) {
                    return static_cast<size_t>(st.st_size);
                }
#endif
                return 0;
            }

            static Json read_or_default(const std::string& filename,
                const Json& default_value = Json::object()) {
                try {
                    if (file_exists(filename)) {
                        return read(filename);
                    }
                    write(filename, default_value);
                    return default_value;
                }
                catch (const std::exception&) {
                    return default_value;
                }
            }
        };

        // ============================================================
        // Json 类的文件 I/O 方法实现
        // ============================================================

        inline void Json::save(const std::string& filename, bool pretty, int indent) const {
            JsonFile::write(filename, *this, pretty, indent);
        }

        inline void Json::save(const std::string& filename, const FileOptions& options) const {
            JsonFile::write(filename, *this, options);
        }

        inline Json Json::load(const std::string& filename) {
            return JsonFile::read(filename);
        }

        inline Json Json::load(const std::string& filename, std::string& error_msg) {
            return JsonFile::read(filename, error_msg);
        }

        inline Json Json::load_or_create(const std::string& filename,
            const Json& default_value) {
            return JsonFile::read_or_default(filename, default_value);
        }

        // ============================================================
        // 便捷函数
        // ============================================================

        inline Json parse_json(const std::string& json_text) {
            return Json::parse(json_text);
        }

        inline std::string to_json(const Json& json, bool pretty = false) {
            return json.dump(pretty);
        }

        // ============================================================
        // 类型转换辅助
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