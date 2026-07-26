// lazy_container.h
// copyright 2026 ? lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <deque>
#include <list>
#include <stack>
#include <algorithm>
#include <iostream>
#include <string>

namespace lazy {
    namespace container {

        // ============================================================
        // 向量工具
        // ============================================================

        // 检查向量是否包含某个元素
        template<typename T>
        bool contains(const std::vector<T>& vec, const T& val) {
            return std::find(vec.begin(), vec.end(), val) != vec.end();
        }

        // 获取向量中某个元素的索引，不存在返回 -1
        template<typename T>
        int index_of(const std::vector<T>& vec, const T& val) {
            auto it = std::find(vec.begin(), vec.end(), val);
            return it != vec.end() ? std::distance(vec.begin(), it) : -1;
        }

        // 向量拼接
        template<typename T>
        std::vector<T> concat(const std::vector<T>& a, const std::vector<T>& b) {
            std::vector<T> result = a;
            result.insert(result.end(), b.begin(), b.end());
            return result;
        }

        // 向量切片 [start, end)
        template<typename T>
        std::vector<T> slice(const std::vector<T>& vec, int start, int end = -1) {
            if (start < 0) start = 0;
            if (end < 0 || end >(int)vec.size()) end = vec.size();
            if (start >= end) return {};
            return std::vector<T>(vec.begin() + start, vec.begin() + end);
        }

        // 移除向量中的某个元素（第一个匹配）
        template<typename T>
        bool remove_first(std::vector<T>& vec, const T& val) {
            auto it = std::find(vec.begin(), vec.end(), val);
            if (it != vec.end()) {
                vec.erase(it);
                return true;
            }
            return false;
        }

        // 移除向量中的所有匹配元素
        template<typename T>
        void remove_all(std::vector<T>& vec, const T& val) {
            vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
        }

        // 检查向量是否有序
        template<typename T>
        bool is_sorted(const std::vector<T>& vec) {
            return std::is_sorted(vec.begin(), vec.end());
        }

        // 反转向量
        template<typename T>
        std::vector<T> reversed(const std::vector<T>& vec) {
            std::vector<T> result = vec;
            std::reverse(result.begin(), result.end());
            return result;
        }

        // ============================================================
        // Map 工具
        // ============================================================

        // 获取 map 中 key 对应的值，不存在则返回默认值
        template<typename K, typename V>
        V get_or_default(const std::map<K, V>& m, const K& key, const V& default_val = V()) {
            auto it = m.find(key);
            return it != m.end() ? it->second : default_val;
        }

        template<typename K, typename V>
        V get_or_default(const std::unordered_map<K, V>& m, const K& key, const V& default_val = V()) {
            auto it = m.find(key);
            return it != m.end() ? it->second : default_val;
        }

        // 检查 map 是否包含某个 key
        template<typename K, typename V>
        bool contains_key(const std::map<K, V>& m, const K& key) {
            return m.find(key) != m.end();
        }

        template<typename K, typename V>
        bool contains_key(const std::unordered_map<K, V>& m, const K& key) {
            return m.find(key) != m.end();
        }

        // 获取 map 的所有 key
        template<typename K, typename V>
        std::vector<K> keys(const std::map<K, V>& m) {
            std::vector<K> result;
            result.reserve(m.size());
            for (const auto& [k, v] : m) {
                result.push_back(k);
            }
            return result;
        }

        template<typename K, typename V>
        std::vector<K> keys(const std::unordered_map<K, V>& m) {
            std::vector<K> result;
            result.reserve(m.size());
            for (const auto& [k, v] : m) {
                result.push_back(k);
            }
            return result;
        }

        // 获取 map 的所有 value
        template<typename K, typename V>
        std::vector<V> values(const std::map<K, V>& m) {
            std::vector<V> result;
            result.reserve(m.size());
            for (const auto& [k, v] : m) {
                result.push_back(v);
            }
            return result;
        }

        template<typename K, typename V>
        std::vector<V> values(const std::unordered_map<K, V>& m) {
            std::vector<V> result;
            result.reserve(m.size());
            for (const auto& [k, v] : m) {
                result.push_back(v);
            }
            return result;
        }

        // 合并两个 map
        template<typename K, typename V>
        std::map<K, V> merge(const std::map<K, V>& a, const std::map<K, V>& b) {
            std::map<K, V> result = a;
            for (const auto& [k, v] : b) {
                result[k] = v;
            }
            return result;
        }

        template<typename K, typename V>
        std::unordered_map<K, V> merge(const std::unordered_map<K, V>& a, const std::unordered_map<K, V>& b) {
            std::unordered_map<K, V> result = a;
            for (const auto& [k, v] : b) {
                result[k] = v;
            }
            return result;
        }

        // ============================================================
        // Set 工具
        // ============================================================

        // 集合交集
        template<typename T>
        std::set<T> intersect(const std::set<T>& a, const std::set<T>& b) {
            std::set<T> result;
            std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                std::inserter(result, result.begin()));
            return result;
        }

        // 集合并集
        template<typename T>
        std::set<T> unite(const std::set<T>& a, const std::set<T>& b) {
            std::set<T> result;
            std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                std::inserter(result, result.begin()));
            return result;
        }

        // 集合差集 (a - b)
        template<typename T>
        std::set<T> difference(const std::set<T>& a, const std::set<T>& b) {
            std::set<T> result;
            std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                std::inserter(result, result.begin()));
            return result;
        }

        // 集合对称差 (a △ b)
        template<typename T>
        std::set<T> symmetric_difference(const std::set<T>& a, const std::set<T>& b) {
            std::set<T> result;
            std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                std::inserter(result, result.begin()));
            return result;
        }

        // ============================================================
        // 队列/栈工具
        // ============================================================

        // 清空队列
        template<typename T>
        void clear_queue(std::queue<T>& q) {
            std::queue<T> empty;
            std::swap(q, empty);
        }

        // 清空栈
        template<typename T>
        void clear_stack(std::stack<T>& s) {
            std::stack<T> empty;
            std::swap(s, empty);
        }

        // ============================================================
        // 通用容器工具
        // ============================================================

        // 判断容器是否为空
        template<typename Container>
        bool empty(const Container& c) {
            return c.empty();
        }

        // 获取容器大小
        template<typename Container>
        size_t size(const Container& c) {
            return c.size();
        }

        // 打印容器到 ostream
        template<typename Container>
        void print(const Container& c, std::ostream& os = std::cout,
            const std::string& sep = ", ", const std::string& prefix = "[",
            const std::string& suffix = "]") {
            os << prefix;
            bool first = true;
            for (const auto& item : c) {
                if (!first) os << sep;
                // 关键修复：如果是 pair，特殊处理
                if constexpr (std::is_same_v<typename Container::value_type, std::pair<const std::string, int>> ||
                    std::is_same_v<typename Container::value_type, std::pair<std::string, int>>) {
                    os << item.first << ":" << item.second;
                }
                else {
                    os << item;
                }
                first = false;
            }
            os << suffix;
        }

        // 转为字符串
        template<typename Container>
        std::string to_string(const Container& c, const std::string& sep = ", ",
            const std::string& prefix = "[", const std::string& suffix = "]") {
            std::ostringstream oss;
            print(c, oss, sep, prefix, suffix);
            return oss.str();
        }

        // ============================================================
        // 特殊容器：多索引映射
        // ============================================================

        template<typename K1, typename K2, typename V>
        class BiMap {
        private:
            std::map<K1, V> forward1;
            std::map<K2, V> forward2;
            std::map<V, std::pair<K1, K2>> reverse;

        public:
            void insert(const K1& k1, const K2& k2, const V& val) {
                forward1[k1] = val;
                forward2[k2] = val;
                reverse[val] = { k1, k2 };
            }

            bool contains_key1(const K1& k1) const {
                return forward1.find(k1) != forward1.end();
            }

            bool contains_key2(const K2& k2) const {
                return forward2.find(k2) != forward2.end();
            }

            bool contains_value(const V& val) const {
                return reverse.find(val) != reverse.end();
            }

            V get_by_key1(const K1& k1) const {
                auto it = forward1.find(k1);
                return it != forward1.end() ? it->second : V();
            }

            V get_by_key2(const K2& k2) const {
                auto it = forward2.find(k2);
                return it != forward2.end() ? it->second : V();
            }

            std::pair<K1, K2> get_keys_by_value(const V& val) const {
                auto it = reverse.find(val);
                return it != reverse.end() ? it->second : std::pair<K1, K2>();
            }

            void remove_by_key1(const K1& k1) {
                auto it = forward1.find(k1);
                if (it != forward1.end()) {
                    V val = it->second;
                    forward1.erase(it);
                    // 找到并删除对应的 forward2
                    for (auto it2 = forward2.begin(); it2 != forward2.end(); ++it2) {
                        if (it2->second == val) {
                            forward2.erase(it2);
                            break;
                        }
                    }
                    reverse.erase(val);
                }
            }

            size_t size() const { return forward1.size(); }
            bool empty() const { return forward1.empty(); }
            void clear() { forward1.clear(); forward2.clear(); reverse.clear(); }
        };

    } // namespace container

    //using namespace container;

#ifdef LAZY_EXPORT_ALL
// 只有定义了 LAZY_EXPORT_ALL 才会导出
    using namespace container;
#endif
} // namespace lazy