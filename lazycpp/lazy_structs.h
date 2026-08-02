// lazy_structs.h
// copyright 2026 lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

// 防止 Windows min/max 宏冲突
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <functional>
#include <optional>
#include <variant>
#include <any>
#include <memory>
#include <chrono>
#include <cstdint>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <queue>
#include <regex>

namespace lazy {
    namespace structs {

        // ============================================================
        // 1. 范围/区间类型
        // ============================================================

        template<typename T>
        struct Range {
            T start;
            T end;

            Range() : start(T()), end(T()) {}
            Range(T s, T e) : start(s), end(e) {}

            bool contains(const T& value) const {
                return value >= start && value <= end;
            }

            bool overlaps(const Range<T>& other) const {
                return !(end < other.start || other.end < start);
            }

            Range<T> intersect(const Range<T>& other) const {
                return Range<T>(
                    (start > other.start) ? start : other.start,
                    (end < other.end) ? end : other.end
                );
            }

            T length() const {
                return end - start;
            }

            std::string to_string() const {
                std::ostringstream oss;
                oss << "[" << start << ", " << end << "]";
                return oss.str();
            }
        };

        // 常用范围类型别名
        using RangeI = Range<int>;
        using RangeL = Range<long long>;
        using RangeD = Range<double>;
        using RangeF = Range<float>;

        // ============================================================
        // 2. 二维/三维点
        // ============================================================

        template<typename T>
        struct Point2D {
            T x, y;

            Point2D() : x(T()), y(T()) {}
            Point2D(T x_, T y_) : x(x_), y(y_) {}

            Point2D<T> operator+(const Point2D<T>& other) const {
                return Point2D<T>(x + other.x, y + other.y);
            }

            Point2D<T> operator-(const Point2D<T>& other) const {
                return Point2D<T>(x - other.x, y - other.y);
            }

            Point2D<T> operator*(T scalar) const {
                return Point2D<T>(x * scalar, y * scalar);
            }

            bool operator==(const Point2D<T>& other) const {
                return x == other.x && y == other.y;
            }

            bool operator!=(const Point2D<T>& other) const {
                return !(*this == other);
            }

            double distance_to(const Point2D<T>& other) const {
                T dx = x - other.x;
                T dy = y - other.y;
                return std::sqrt((double)(dx * dx + dy * dy));
            }

            T squared_distance(const Point2D<T>& other) const {
                T dx = x - other.x;
                T dy = y - other.y;
                return dx * dx + dy * dy;
            }

            std::string to_string() const {
                std::ostringstream oss;
                oss << "(" << x << ", " << y << ")";
                return oss.str();
            }
        };

        template<typename T>
        struct Point3D {
            T x, y, z;

            Point3D() : x(T()), y(T()), z(T()) {}
            Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

            Point3D<T> operator+(const Point3D<T>& other) const {
                return Point3D<T>(x + other.x, y + other.y, z + other.z);
            }

            Point3D<T> operator-(const Point3D<T>& other) const {
                return Point3D<T>(x - other.x, y - other.y, z - other.z);
            }

            double distance_to(const Point3D<T>& other) const {
                T dx = x - other.x;
                T dy = y - other.y;
                T dz = z - other.z;
                return std::sqrt((double)(dx * dx + dy * dy + dz * dz));
            }

            std::string to_string() const {
                std::ostringstream oss;
                oss << "(" << x << ", " << y << ", " << z << ")";
                return oss.str();
            }
        };

        // 常用点类型别名
        using Point2Di = Point2D<int>;
        using Point2Dl = Point2D<long long>;
        using Point2Df = Point2D<float>;
        using Point2Dd = Point2D<double>;

        using Point3Di = Point3D<int>;
        using Point3Dl = Point3D<long long>;
        using Point3Df = Point3D<float>;
        using Point3Dd = Point3D<double>;

        // ============================================================
        // 3. 矩形/边界框
        // ============================================================

        template<typename T>
        struct Rect {
            T x, y;          // 左上角
            T width, height; // 宽度和高度

            Rect() : x(T()), y(T()), width(T()), height(T()) {}
            Rect(T x_, T y_, T w_, T h_) : x(x_), y(y_), width(w_), height(h_) {}

            T left() const { return x; }
            T right() const { return x + width; }
            T top() const { return y; }
            T bottom() const { return y + height; }
            T center_x() const { return x + width / 2; }
            T center_y() const { return y + height / 2; }

            bool contains(const Point2D<T>& point) const {
                return point.x >= x && point.x <= x + width &&
                    point.y >= y && point.y <= y + height;
            }

            bool contains(const Rect<T>& other) const {
                return other.x >= x && other.x + other.width <= x + width &&
                    other.y >= y && other.y + other.height <= y + height;
            }

            bool intersects(const Rect<T>& other) const {
                return !(other.x > x + width || other.x + other.width < x ||
                    other.y > y + height || other.y + other.height < y);
            }

            Rect<T> intersect(const Rect<T>& other) const {
                T new_x = (x > other.x) ? x : other.x;
                T new_y = (y > other.y) ? y : other.y;
                T new_width = ((x + width) < (other.x + other.width) ? (x + width) : (other.x + other.width)) - new_x;
                T new_height = ((y + height) < (other.y + other.height) ? (y + height) : (other.y + other.height)) - new_y;
                if (new_width < 0 || new_height < 0) {
                    return Rect<T>();
                }
                return Rect<T>(new_x, new_y, new_width, new_height);
            }

            Rect<T> expand(T margin) const {
                return Rect<T>(x - margin, y - margin, width + 2 * margin, height + 2 * margin);
            }

            T area() const { return width * height; }

            std::string to_string() const {
                std::ostringstream oss;
                oss << "Rect(" << x << ", " << y << ", " << width << "x" << height << ")";
                return oss.str();
            }
        };

        using RectI = Rect<int>;
        using RectF = Rect<float>;
        using RectD = Rect<double>;

        // ============================================================
        // 4. 键值对/三元组增强
        // ============================================================

        template<typename K, typename V>
        struct KeyValue {
            K key;
            V value;

            KeyValue() : key(K()), value(V()) {}
            KeyValue(const K& k, const V& v) : key(k), value(v) {}

            operator std::pair<K, V>() const {
                return { key, value };
            }

            bool operator==(const KeyValue<K, V>& other) const {
                return key == other.key && value == other.value;
            }
        };

        template<typename A, typename B, typename C>
        struct Triple {
            A first;
            B second;
            C third;

            Triple() : first(A()), second(B()), third(C()) {}
            Triple(const A& a, const B& b, const C& c) : first(a), second(b), third(c) {}

            operator std::tuple<A, B, C>() const {
                return { first, second, third };
            }

            bool operator==(const Triple<A, B, C>& other) const {
                return first == other.first && second == other.second && third == other.third;
            }
        };

        template<typename A, typename B, typename C, typename D>
        struct Quad {
            A first;
            B second;
            C third;
            D fourth;

            Quad() : first(A()), second(B()), third(C()), fourth(D()) {}
            Quad(const A& a, const B& b, const C& c, const D& d)
                : first(a), second(b), third(c), fourth(d) {
            }

            operator std::tuple<A, B, C, D>() const {
                return { first, second, third, fourth };
            }
        };

        // ============================================================
        // 5. 可选值/结果类型
        // ============================================================

        template<typename T>
        class Result {
        private:
            std::optional<T> value_;
            std::string error_;

        public:
            Result() : value_(std::nullopt) {}
            Result(const T& val) : value_(val), error_("") {}
            Result(const std::string& error) : value_(std::nullopt), error_(error) {}

            static Result<T> ok(const T& val) { return Result<T>(val); }
            static Result<T> err(const std::string& error) { return Result<T>(error); }

            bool is_ok() const { return value_.has_value(); }
            bool is_err() const { return !value_.has_value(); }

            const T& value() const {
                if (!value_.has_value()) {
                    throw std::runtime_error("Result has no value: " + error_);
                }
                return value_.value();
            }

            const std::string& error() const { return error_; }

            T value_or(const T& default_val) const {
                return value_.value_or(default_val);
            }

            template<typename F>
            auto map(F&& f) const -> Result<decltype(f(std::declval<T>()))> {
                using U = decltype(f(std::declval<T>()));
                if (is_ok()) {
                    return Result<U>::ok(f(value_.value()));
                }
                return Result<U>::err(error_);
            }
        };

        // ============================================================
        // 6. 位标志
        // ============================================================

        template<typename Enum>
        class Flags {
        private:
            using Underlying = std::underlying_type_t<Enum>;
            Underlying bits_;

        public:
            Flags() : bits_(0) {}
            Flags(Enum flag) : bits_(static_cast<Underlying>(flag)) {}

            Flags& set(Enum flag) {
                bits_ |= static_cast<Underlying>(flag);
                return *this;
            }

            Flags& clear(Enum flag) {
                bits_ &= ~static_cast<Underlying>(flag);
                return *this;
            }

            bool has(Enum flag) const {
                return (bits_ & static_cast<Underlying>(flag)) != 0;
            }

            bool any() const { return bits_ != 0; }
            bool none() const { return bits_ == 0; }

            Flags& operator|=(Enum flag) {
                bits_ |= static_cast<Underlying>(flag);
                return *this;
            }

            Flags& operator&=(Enum flag) {
                bits_ &= static_cast<Underlying>(flag);
                return *this;
            }

            operator Underlying() const { return bits_; }
        };

        // ============================================================
        // 7. 版本号
        // ============================================================

        struct Version {
            int major;
            int minor;
            int patch;
            std::string pre_release;
            std::string build_metadata;

            Version() : major(0), minor(0), patch(0) {}
            Version(int mj, int mn, int p = 0)
                : major(mj), minor(mn), patch(p) {
            }
            Version(int mj, int mn, int p, const std::string& pre)
                : major(mj), minor(mn), patch(p), pre_release(pre) {
            }

            std::string to_string() const {
                std::string result = std::to_string(major) + "." +
                    std::to_string(minor) + "." +
                    std::to_string(patch);
                if (!pre_release.empty()) {
                    result += "-" + pre_release;
                }
                if (!build_metadata.empty()) {
                    result += "+" + build_metadata;
                }
                return result;
            }

            bool operator<(const Version& other) const {
                if (major != other.major) return major < other.major;
                if (minor != other.minor) return minor < other.minor;
                if (patch != other.patch) return patch < other.patch;
                return pre_release < other.pre_release;
            }

            bool operator==(const Version& other) const {
                return major == other.major && minor == other.minor &&
                    patch == other.patch && pre_release == other.pre_release;
            }

            bool operator!=(const Version& other) const {
                return !(*this == other);
            }

            bool operator>(const Version& other) const {
                return other < *this;
            }

            bool operator<=(const Version& other) const {
                return !(*this > other);
            }

            bool operator>=(const Version& other) const {
                return !(*this < other);
            }

            // 从字符串解析
            static Version parse(const std::string& str) {
                Version v;
                std::regex pattern(R"((\d+)\.(\d+)\.(\d+)(?:-([\w.-]+))?(?:\+([\w.-]+))?)");
                std::smatch matches;
                if (std::regex_match(str, matches, pattern)) {
                    v.major = std::stoi(matches[1]);
                    v.minor = std::stoi(matches[2]);
                    v.patch = std::stoi(matches[3]);
                    if (matches[4].matched) v.pre_release = matches[4];
                    if (matches[5].matched) v.build_metadata = matches[5];
                }
                return v;
            }
        };

        // ============================================================
        // 8. 时间区间
        // ============================================================

        struct TimeRange {
            std::chrono::system_clock::time_point start;
            std::chrono::system_clock::time_point end;

            TimeRange() = default;
            TimeRange(const std::chrono::system_clock::time_point& s,
                const std::chrono::system_clock::time_point& e)
                : start(s), end(e) {
            }

            bool contains(const std::chrono::system_clock::time_point& t) const {
                return t >= start && t <= end;
            }

            bool overlaps(const TimeRange& other) const {
                return !(end < other.start || other.end < start);
            }

            auto duration() const {
                return end - start;
            }

            bool is_valid() const {
                return start <= end;
            }
        };

        // ============================================================
        // 9. 分页信息
        // ============================================================

        struct PageInfo {
            int page;
            int size;
            int total;

            PageInfo() : page(1), size(10), total(0) {}
            PageInfo(int p, int s, int t = 0) : page(p), size(s), total(t) {}

            int total_pages() const {
                if (size <= 0) return 0;
                return (total + size - 1) / size;
            }

            int offset() const {
                return (page - 1) * size;
            }

            bool has_prev() const {
                return page > 1;
            }

            bool has_next() const {
                return page < total_pages();
            }

            PageInfo prev() const {
                return PageInfo(page - 1, size, total);
            }

            PageInfo next() const {
                return PageInfo(page + 1, size, total);
            }

            std::string to_string() const {
                std::ostringstream oss;
                oss << "Page " << page << "/" << total_pages()
                    << " (size: " << size << ", total: " << total << ")";
                return oss.str();
            }
        };

        // ============================================================
        // 10. 排序/筛选参数
        // ============================================================

        template<typename T>
        struct SortOrder {
            T field;
            bool ascending;

            SortOrder() : field(T()), ascending(true) {}
            SortOrder(T f, bool asc = true) : field(f), ascending(asc) {}
        };

        template<typename T>
        struct Filter {
            T field;
            enum class Operator {
                EQ, NE, GT, GE, LT, LE, CONTAINS, STARTS_WITH, ENDS_WITH
            } op;
            std::string value;

            Filter() : field(T()), op(Operator::EQ) {}
            Filter(T f, Operator o, const std::string& v) : field(f), op(o), value(v) {}
        };

        struct QueryParams {
            std::string search;
            std::vector<SortOrder<std::string>> sorts;
            PageInfo page;
            std::map<std::string, std::string> filters;

            QueryParams() : page(1, 10) {}
        };

        // ============================================================
        // 11. 统计信息
        // ============================================================

        template<typename T>
        struct Stats {
            size_t count;
            T sum;
            T MIN;
            T MAX;
            T mean;
            T variance;
            T stddev;

            Stats() : count(0), sum(T()), MIN(T()), MAX(T()),
                mean(T()), variance(T()), stddev(T()) {
            }
            std::string to_string() const {
                std::ostringstream oss;
                oss << "Stats{count=" << count
                    << ", sum=" << sum
                    << ", min=" << MIN
                    << ", max=" << MAX
                    << ", mean=" << mean
                    << ", stddev=" << stddev << "}";
                return oss.str();
            }
        };

        // ============================================================
        // 12. 错误信息
        // ============================================================

        struct Error {
            int code;
            std::string message;
            std::string details;
            std::string source;
            std::chrono::system_clock::time_point timestamp;

            Error() : code(0), timestamp(std::chrono::system_clock::now()) {}
            Error(int c, const std::string& msg)
                : code(c), message(msg), timestamp(std::chrono::system_clock::now()) {
            }
            Error(int c, const std::string& msg, const std::string& src)
                : code(c), message(msg), source(src),
                timestamp(std::chrono::system_clock::now()) {
            }

            std::string to_string() const {
                auto time = std::chrono::system_clock::to_time_t(timestamp);
                std::string time_str = std::ctime(&time);
                if (!time_str.empty() && time_str.back() == '\n') {
                    time_str.pop_back();
                }
                std::ostringstream oss;
                oss << "[" << time_str << "] "
                    << "Error " << code << ": " << message
                    << (source.empty() ? "" : " (" + source + ")")
                    << (details.empty() ? "" : "\n  " + details);
                return oss.str();
            }
        };

        // ============================================================
        // 13. 通用树节点
        // ============================================================

        template<typename T>
        class TreeNode : public std::enable_shared_from_this<TreeNode<T>> {
        public:
            T data;
            std::vector<std::shared_ptr<TreeNode<T>>> children;
            std::weak_ptr<TreeNode<T>> parent;

            TreeNode() = default;
            explicit TreeNode(const T& d) : data(d) {}

            void add_child(std::shared_ptr<TreeNode<T>> child) {
                child->parent = this->shared_from_this();
                children.push_back(child);
            }

            bool is_leaf() const {
                return children.empty();
            }

            bool is_root() const {
                return parent.lock() == nullptr;
            }

            size_t depth() const {
                size_t d = 0;
                auto current = parent.lock();
                while (current) {
                    d++;
                    current = current->parent.lock();
                }
                return d;
            }

            // 前序遍历
            void preorder_traversal(std::function<void(const T&)> visit) const {
                visit(data);
                for (const auto& child : children) {
                    child->preorder_traversal(visit);
                }
            }

            // 后序遍历
            void postorder_traversal(std::function<void(const T&)> visit) const {
                for (const auto& child : children) {
                    child->postorder_traversal(visit);
                }
                visit(data);
            }

            // 层序遍历（修复版 - 使用 const_cast 或改用非 const 版本）
            void levelorder_traversal(std::function<void(const T&)> visit) const {
                std::queue<std::shared_ptr<TreeNode<T>>> q;
                // 使用 const_pointer_cast 移除 const 限定
                q.push(std::const_pointer_cast<TreeNode<T>>(this->shared_from_this()));

                while (!q.empty()) {
                    auto node = q.front();
                    q.pop();
                    visit(node->data);
                    for (const auto& child : node->children) {
                        q.push(child);
                    }
                }
            }
            
            // 非 const 版本的层序遍历
            void levelorder_traversal(std::function<void(T&)> visit) {
                std::queue<std::shared_ptr<TreeNode<T>>> q;
                q.push(this->shared_from_this());

                while (!q.empty()) {
                    auto node = q.front();
                    q.pop();
                    visit(node->data);
                    for (const auto& child : node->children) {
                        q.push(child);
                    }
                }
            }

            // 查找节点
            std::shared_ptr<TreeNode<T>> find(const T& value) {
                if (data == value) {
                    return this->shared_from_this();
                }
                for (auto& child : children) {
                    auto result = child->find(value);
                    if (result) {
                        return result;
                    }
                }
                return nullptr;
            }

            // const 版本的查找
            std::shared_ptr<const TreeNode<T>> find(const T& value) const {
                if (data == value) {
                    return this->shared_from_this();
                }
                for (const auto& child : children) {
                    auto result = child->find(value);
                    if (result) {
                        return result;
                    }
                }
                return nullptr;
            }

            // 获取节点数量
            size_t node_count() const {
                size_t count = 1;
                for (const auto& child : children) {
                    count += child->node_count();
                }
                return count;
            }

            // 获取高度
            size_t height() const {
                if (children.empty()) {
                    return 1;
                }
                size_t max_height = 0;
                for (const auto& child : children) {
                    size_t h = child->height();
                    if (h > max_height) max_height = h;
                }
                return max_height + 1;
            }

            // 打印树（可视化）
            void print_tree(int indent = 0) const {
                for (int i = 0; i < indent; i++) {
                    std::cout << "  ";
                }
                std::cout << data << "\n";
                for (const auto& child : children) {
                    child->print_tree(indent + 1);
                }
            }

            // 打印树（带连接线）
            void print_pretty(const std::string& prefix = "", bool is_last = true) const {
                std::cout << prefix;
                if (!prefix.empty()) {
                    std::cout << (is_last ? "└── " : "├── ");
                }
                std::cout << data << "\n";

                for (size_t i = 0; i < children.size(); i++) {
                    bool last = (i == children.size() - 1);
                    std::string new_prefix = prefix + (is_last ? "    " : "│   ");
                    children[i]->print_pretty(new_prefix, last);
                }
            }

            // 转换为字符串表示
            std::string to_string() const {
                std::ostringstream oss;
                oss << data;
                if (!children.empty()) {
                    oss << "(";
                    for (size_t i = 0; i < children.size(); i++) {
                        if (i > 0) oss << ", ";
                        oss << children[i]->to_string();
                    }
                    oss << ")";
                }
                return oss.str();
            }
        };

        // ============================================================
        // 14. 带权重的边
        // ============================================================

        template<typename T>
        struct WeightedEdge {
            T from;
            T to;
            double weight;

            WeightedEdge() : from(T()), to(T()), weight(1.0) {}
            WeightedEdge(const T& f, const T& t, double w = 1.0)
                : from(f), to(t), weight(w) {
            }

            bool operator<(const WeightedEdge<T>& other) const {
                return weight < other.weight;
            }

            bool operator>(const WeightedEdge<T>& other) const {
                return weight > other.weight;
            }

            bool operator==(const WeightedEdge<T>& other) const {
                return from == other.from && to == other.to && weight == other.weight;
            }
        };

        template<typename T>
        struct Edge {
            T from;
            T to;

            Edge() : from(T()), to(T()) {}
            Edge(const T& f, const T& t) : from(f), to(t) {}

            bool operator==(const Edge<T>& other) const {
                return from == other.from && to == other.to;
            }
        };

        // ============================================================
        // 15. 类型别名
        // ============================================================

        //扩展自lazy_core
        template<typename T>
        using Vec = std::vector<T>;

        template<typename K, typename V>
        using Map = std::map<K, V>;

        template<typename K, typename V>
        using UMap = std::unordered_map<K, V>;

        template<typename T>
        using Set = std::set<T>;

        template<typename T>
        using USet = std::unordered_set<T>;

        template<typename T>
        using Opt = std::optional<T>;

        template<typename... Ts>
        using Var = std::variant<Ts...>;

        template<typename T>
        using Ptr = std::shared_ptr<T>;

        template<typename T>
        using UPtr = std::unique_ptr<T>;

        template<typename T>
        using WeakPtr = std::weak_ptr<T>;

    } // namespace structs

#ifdef LAZY_EXPORT_ALL
    using namespace structs;
#endif
} // namespace lazy