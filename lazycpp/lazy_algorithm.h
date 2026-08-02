// lazy_algorithm.h
// copyright 2026 lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once


#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <climits>
#include <cstring>
#include <string>
#include <utility>
#include <iterator>
#include <random>
#include <chrono>
//#include <bitset>
#include <cstdint>

namespace lazy {
    namespace algorithm {

        // ============================================================
        // 本header有一些STL已有功能
        // ============================================================

        template<typename T>
        int lower_bound(const std::vector<T>& arr, T target) {
            int l = 0, r = (int)arr.size();
            while (l < r) {
                int mid = l + (r - l) / 2;
                if (arr[mid] < target) l = mid + 1;
                else r = mid;
            }
            return l;
        }

        template<typename T>
        int upper_bound(const std::vector<T>& arr, T target) {
            int l = 0, r = (int)arr.size();
            while (l < r) {
                int mid = l + (r - l) / 2;
                if (arr[mid] <= target) l = mid + 1;
                else r = mid;
            }
            return l;
        }

        template<typename T>
        void reverse(T& arr) {
            std::reverse(arr.begin(), arr.end());
        }

        template<typename T>
        T maxof(const std::vector<T>& arr) {
            return *std::max_element(arr.begin(), arr.end());
        }

        template<typename T>
        T minof(const std::vector<T>& arr) {
            return *std::min_element(arr.begin(), arr.end());
        }

        template<typename T>
        std::vector<T> unique(const std::vector<T>& arr) {
            std::vector<T> result = arr;
            std::sort(result.begin(), result.end());
            result.erase(std::unique(result.begin(), result.end()), result.end());
            return result;
        }

        template<typename T>
        std::vector<T> prefix_sum(const std::vector<T>& arr) {
            std::vector<T> pref(arr.size() + 1, 0);
            for (size_t i = 0; i < arr.size(); i++) {
                pref[i + 1] = pref[i] + arr[i];
            }
            return pref;
        }

        template<typename T>
        T range_sum(const std::vector<T>& pref, int l, int r) {
            return pref[r + 1] - pref[l];
        }

        // ============================================================
        // 1. 排序算法变体
        // ============================================================

        template<typename Container>
        void stable_sort(Container& c) {
            std::stable_sort(c.begin(), c.end());
        }

        template<typename Container, typename Compare>
        void stable_sort(Container& c, Compare comp) {
            std::stable_sort(c.begin(), c.end(), comp);
        }

        template<typename Container>
        void partial_sort(Container& c, size_t k) {
            if (k > c.size()) k = c.size();
            std::partial_sort(c.begin(), c.begin() + k, c.end());
        }

        template<typename Container, typename Compare>
        void partial_sort(Container& c, size_t k, Compare comp) {
            if (k > c.size()) k = c.size();
            std::partial_sort(c.begin(), c.begin() + k, c.end(), comp);
        }

        template<typename Container>
        typename Container::value_type nth_element(Container& c, size_t k) {
            if (k >= c.size()) {
                throw std::out_of_range("k out of range in nth_element");
            }
            std::nth_element(c.begin(), c.begin() + k, c.end());
            return c[k];
        }

        // ============================================================
        // 2. 归并
        // ============================================================

        template<typename T>
        std::vector<T> merge(const std::vector<T>& a, const std::vector<T>& b) {
            std::vector<T> result(a.size() + b.size());
            std::merge(a.begin(), a.end(), b.begin(), b.end(), result.begin());
            return result;
        }

        template<typename T, typename Compare>
        std::vector<T> merge(const std::vector<T>& a, const std::vector<T>& b, Compare comp) {
            std::vector<T> result(a.size() + b.size());
            std::merge(a.begin(), a.end(), b.begin(), b.end(), result.begin(), comp);
            return result;
        }

        template<typename T>
        std::vector<T> merge_multi(const std::vector<std::vector<T>>& arrays) {
            if (arrays.empty()) return {};
            if (arrays.size() == 1) return arrays[0];

            std::vector<T> result = arrays[0];
            for (size_t i = 1; i < arrays.size(); i++) {
                result = merge(result, arrays[i]);
            }
            return result;
        }

        template<typename T>
        std::vector<T> merge_k_sorted(const std::vector<std::vector<T>>& arrays) {
            if (arrays.empty()) return {};

            struct Node {
                T value;
                int arr_idx;
                int pos_idx;
                bool operator>(const Node& other) const {
                    return value > other.value;
                }
            };

            std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
            std::vector<T> result;

            for (size_t i = 0; i < arrays.size(); i++) {
                if (!arrays[i].empty()) {
                    pq.push({ arrays[i][0], (int)i, 0 });
                }
            }

            while (!pq.empty()) {
                Node node = pq.top();
                pq.pop();
                result.push_back(node.value);

                int next_pos = node.pos_idx + 1;
                if (next_pos < (int)arrays[node.arr_idx].size()) {
                    pq.push({
                        arrays[node.arr_idx][next_pos],
                        node.arr_idx,
                        next_pos
                        });
                }
            }

            return result;
        }

        // ============================================================
        // 3. 查找
        // ============================================================

        template<typename T>
        int linear_search(const std::vector<T>& arr, const T& target) {
            for (size_t i = 0; i < arr.size(); i++) {
                if (arr[i] == target) return (int)i;
            }
            return -1;
        }

        template<typename T, typename Pred>
        int linear_search_if(const std::vector<T>& arr, Pred pred) {
            for (size_t i = 0; i < arr.size(); i++) {
                if (pred(arr[i])) return (int)i;
            }
            return -1;
        }

        template<typename T>
        std::vector<int> find_all(const std::vector<T>& arr, const T& target) {
            std::vector<int> result;
            for (size_t i = 0; i < arr.size(); i++) {
                if (arr[i] == target) result.push_back((int)i);
            }
            return result;
        }

        // ============================================================
        // 4. 快速选择
        // ============================================================

        namespace detail {
            template<typename T>
            int partition_impl(std::vector<T>& arr, int left, int right) {
                static std::mt19937 gen(
                    (unsigned)std::chrono::steady_clock::now().time_since_epoch().count()
                );
                int pivot_idx = left + (gen() % (right - left + 1));
                std::swap(arr[pivot_idx], arr[right]);

                T pivot = arr[right];
                int i = left - 1;
                for (int j = left; j < right; j++) {
                    if (arr[j] <= pivot) {
                        i++;
                        std::swap(arr[i], arr[j]);
                    }
                }
                std::swap(arr[i + 1], arr[right]);
                return i + 1;
            }
        }

        template<typename T>
        T quick_select(std::vector<T>& arr, int k) {
            if (k < 0 || k >= (int)arr.size()) {
                throw std::out_of_range("k out of range in quick_select");
            }

            int left = 0, right = (int)arr.size() - 1;
            while (left <= right) {
                int pivot_idx = detail::partition_impl(arr, left, right);
                if (pivot_idx == k) return arr[pivot_idx];
                if (pivot_idx < k) left = pivot_idx + 1;
                else right = pivot_idx - 1;
            }
            return arr[k];
        }

        // ============================================================
        // 5. 排列组合
        // ============================================================

        template<typename Container>
        bool next_permutation(Container& c) {
            return std::next_permutation(c.begin(), c.end());
        }

        template<typename Container>
        bool prev_permutation(Container& c) {
            return std::prev_permutation(c.begin(), c.end());
        }

        template<typename T>
        std::vector<std::vector<T>> all_permutations(const std::vector<T>& arr) {
            std::vector<std::vector<T>> result;
            std::vector<T> temp = arr;
            std::sort(temp.begin(), temp.end());

            do {
                result.push_back(temp);
            } while (std::next_permutation(temp.begin(), temp.end()));

            return result;
        }

        template<typename T>
        std::vector<std::vector<T>> all_combinations(const std::vector<T>& arr, int k) {
            std::vector<std::vector<T>> result;
            int n = (int)arr.size();
            if (k < 0 || k > n) return result;

            std::vector<int> indices(k);
            std::iota(indices.begin(), indices.end(), 0);

            while (true) {
                std::vector<T> combo;
                for (int idx : indices) {
                    combo.push_back(arr[idx]);
                }
                result.push_back(combo);

                int i = k - 1;
                while (i >= 0 && indices[i] == n - k + i) i--;
                if (i < 0) break;

                indices[i]++;
                for (int j = i + 1; j < k; j++) {
                    indices[j] = indices[j - 1] + 1;
                }
            }

            return result;
        }

        // ============================================================
        // 6. 集合（针对vector）
        // ============================================================

        template<typename T>
        std::vector<T> set_intersection(const std::vector<T>& a, const std::vector<T>& b) {
            std::vector<T> result;
            std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                std::back_inserter(result));
            return result;
        }

        template<typename T>
        std::vector<T> set_union(const std::vector<T>& a, const std::vector<T>& b) {
            std::vector<T> result;
            std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                std::back_inserter(result));
            return result;
        }

        template<typename T>
        std::vector<T> set_difference(const std::vector<T>& a, const std::vector<T>& b) {
            std::vector<T> result;
            std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                std::back_inserter(result));
            return result;
        }

        template<typename T>
        std::vector<T> set_symmetric_difference(const std::vector<T>& a, const std::vector<T>& b) {
            std::vector<T> result;
            std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                std::back_inserter(result));
            return result;
        }

        template<typename T>
        bool is_subset(const std::vector<T>& a, const std::vector<T>& b) {
            std::unordered_set<T> set_b(b.begin(), b.end());
            for (const T& val : a) {
                if (set_b.find(val) == set_b.end()) return false;
            }
            return true;
        }

        // ============================================================
        // 7. 区间操作
        // ============================================================

        template<typename T>
        bool intervals_overlap(T l1, T r1, T l2, T r2) {
            return !(r1 < l2 || r2 < l1);
        }

        template<typename T>
        T intervals_intersection_length(T l1, T r1, T l2, T r2) {
            if (!intervals_overlap(l1, r1, l2, r2)) return 0;
            return (std::min)(r1, r2) - (std::max)(l1, l2);
        }

        template<typename T>
        std::vector<std::pair<T, T>> merge_intervals(std::vector<std::pair<T, T>> intervals) {
            if (intervals.empty()) return {};

            std::sort(intervals.begin(), intervals.end());
            std::vector<std::pair<T, T>> result;
            result.push_back(intervals[0]);

            for (size_t i = 1; i < intervals.size(); i++) {
                auto& last = result.back();
                if (intervals[i].first <= last.second) {
                    last.second = (std::max)(last.second, intervals[i].second);
                }
                else {
                    result.push_back(intervals[i]);
                }
            }

            return result;
        }

        template<typename T>
        std::vector<std::pair<T, T>> insert_interval(
            std::vector<std::pair<T, T>> intervals,
            std::pair<T, T> new_interval) {
            intervals.push_back(new_interval);
            return merge_intervals(intervals);
        }

        // ============================================================
        // 8. 滑动窗口
        // ============================================================

        template<typename T>
        std::vector<T> sliding_window_max(const std::vector<T>& arr, int k) {
            if (arr.empty() || k <= 0 || k > (int)arr.size()) return {};

            std::vector<T> result;
            std::deque<int> dq;

            for (int i = 0; i < (int)arr.size(); i++) {
                while (!dq.empty() && dq.front() <= i - k) {
                    dq.pop_front();
                }

                while (!dq.empty() && arr[dq.back()] <= arr[i]) {
                    dq.pop_back();
                }

                dq.push_back(i);

                if (i >= k - 1) {
                    result.push_back(arr[dq.front()]);
                }
            }

            return result;
        }

        template<typename T>
        std::vector<T> sliding_window_min(const std::vector<T>& arr, int k) {
            if (arr.empty() || k <= 0 || k > (int)arr.size()) return {};

            std::vector<T> result;
            std::deque<int> dq;

            for (int i = 0; i < (int)arr.size(); i++) {
                while (!dq.empty() && dq.front() <= i - k) {
                    dq.pop_front();
                }

                while (!dq.empty() && arr[dq.back()] >= arr[i]) {
                    dq.pop_back();
                }

                dq.push_back(i);

                if (i >= k - 1) {
                    result.push_back(arr[dq.front()]);
                }
            }

            return result;
        }

        template<typename T>
        std::vector<T> sliding_window_sum(const std::vector<T>& arr, int k) {
            if (arr.empty() || k <= 0 || k > (int)arr.size()) return {};

            std::vector<T> result;
            T window_sum = 0;

            for (int i = 0; i < k; i++) {
                window_sum += arr[i];
            }
            result.push_back(window_sum);

            for (int i = k; i < (int)arr.size(); i++) {
                window_sum += arr[i] - arr[i - k];
                result.push_back(window_sum);
            }

            return result;
        }

        // ============================================================
        // 9. 最大子数组和（Kadane）
        // ============================================================

        template<typename T>
        T max_subarray_sum(const std::vector<T>& arr) {
            if (arr.empty()) return 0;

            T max_ending_here = arr[0];
            T max_so_far = arr[0];

            for (size_t i = 1; i < arr.size(); i++) {
                max_ending_here = (std::max)(arr[i], max_ending_here + arr[i]);
                max_so_far = (std::max)(max_so_far, max_ending_here);
            }

            return max_so_far;
        }

        template<typename T>
        std::pair<int, int> max_subarray_range(const std::vector<T>& arr) {
            if (arr.empty()) return { -1, -1 };

            T max_ending_here = arr[0];
            T max_so_far = arr[0];
            int start = 0, end = 0;
            int temp_start = 0;

            for (size_t i = 1; i < arr.size(); i++) {
                if (arr[i] > max_ending_here + arr[i]) {
                    max_ending_here = arr[i];
                    temp_start = (int)i;
                }
                else {
                    max_ending_here += arr[i];
                }

                if (max_ending_here > max_so_far) {
                    max_so_far = max_ending_here;
                    start = temp_start;
                    end = (int)i;
                }
            }

            return { start, end };
        }

        // ============================================================
        // 10. 最长递增子序列（LIS）
        // ============================================================

        template<typename T>
        std::vector<T> longest_increasing_subsequence(const std::vector<T>& arr) {
            if (arr.empty()) return {};

            std::vector<T> tail;
            std::vector<int> parent(arr.size(), -1);
            std::vector<int> pos(arr.size(), 0);

            for (size_t i = 0; i < arr.size(); i++) {
                auto it = std::lower_bound(tail.begin(), tail.end(), arr[i]);
                int idx = (int)(it - tail.begin());

                if (it == tail.end()) {
                    tail.push_back(arr[i]);
                    pos[idx] = (int)i;
                    if (idx > 0) parent[i] = pos[idx - 1];
                }
                else {
                    *it = arr[i];
                    pos[idx] = (int)i;
                    if (idx > 0) parent[i] = pos[idx - 1];
                }
            }

            std::vector<T> result;
            int idx = pos[tail.size() - 1];
            while (idx != -1) {
                result.push_back(arr[idx]);
                idx = parent[idx];
            }
            std::reverse(result.begin(), result.end());

            return result;
        }

        template<typename T>
        int lis_length(const std::vector<T>& arr) {
            std::vector<T> tail;
            for (const T& val : arr) {
                auto it = std::lower_bound(tail.begin(), tail.end(), val);
                if (it == tail.end()) {
                    tail.push_back(val);
                }
                else {
                    *it = val;
                }
            }
            return (int)tail.size();
        }

        // ============================================================
        // 11. 最长公共子序列（LCS）
        // ============================================================

        template<typename T>
        std::vector<T> longest_common_subsequence(const std::vector<T>& a, const std::vector<T>& b) {
            int m = (int)a.size(), n = (int)b.size();
            std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

            for (int i = 1; i <= m; i++) {
                for (int j = 1; j <= n; j++) {
                    if (a[i - 1] == b[j - 1]) {
                        dp[i][j] = dp[i - 1][j - 1] + 1;
                    }
                    else {
                        dp[i][j] = (std::max)(dp[i - 1][j], dp[i][j - 1]);
                    }
                }
            }

            std::vector<T> result;
            int i = m, j = n;
            while (i > 0 && j > 0) {
                if (a[i - 1] == b[j - 1]) {
                    result.push_back(a[i - 1]);
                    i--; j--;
                }
                else if (dp[i - 1][j] >= dp[i][j - 1]) {
                    i--;
                }
                else {
                    j--;
                }
            }

            std::reverse(result.begin(), result.end());
            return result;
        }

        // ============================================================
        // 12. 字符串匹配算法
        // ============================================================

        inline std::vector<int> kmp_search(const std::string& text, const std::string& pattern) {
            if (pattern.empty() || text.empty() || pattern.size() > text.size()) return {};

            std::vector<int> pi(pattern.size(), 0);
            for (size_t i = 1; i < pattern.size(); i++) {
                int j = pi[i - 1];
                while (j > 0 && pattern[i] != pattern[j]) {
                    j = pi[j - 1];
                }
                if (pattern[i] == pattern[j]) j++;
                pi[i] = j;
            }

            std::vector<int> result;
            int j = 0;
            for (size_t i = 0; i < text.size(); i++) {
                while (j > 0 && text[i] != pattern[j]) {
                    j = pi[j - 1];
                }
                if (text[i] == pattern[j]) j++;

                if (j == (int)pattern.size()) {
                    result.push_back((int)(i - pattern.size() + 1));
                    j = pi[j - 1];
                }
            }

            return result;
        }

        inline bool is_substring(const std::string& text, const std::string& pattern) {
            return !kmp_search(text, pattern).empty();
        }

        // ============================================================
        // 13. 图算法基础
        // ============================================================

        template<typename T>
        std::vector<T> bfs(const std::unordered_map<T, std::vector<T>>& graph, T start) {
            std::vector<T> result;
            std::unordered_set<T> visited;
            std::queue<T> q;

            visited.insert(start);
            q.push(start);

            while (!q.empty()) {
                T node = q.front();
                q.pop();
                result.push_back(node);

                auto it = graph.find(node);
                if (it != graph.end()) {
                    for (const T& neighbor : it->second) {
                        if (visited.find(neighbor) == visited.end()) {
                            visited.insert(neighbor);
                            q.push(neighbor);
                        }
                    }
                }
            }

            return result;
        }

        template<typename T>
        std::vector<T> dfs(const std::unordered_map<T, std::vector<T>>& graph, T start) {
            std::vector<T> result;
            std::unordered_set<T> visited;
            std::stack<T> s;

            s.push(start);

            while (!s.empty()) {
                T node = s.top();
                s.pop();

                if (visited.find(node) != visited.end()) continue;
                visited.insert(node);
                result.push_back(node);

                auto it = graph.find(node);
                if (it != graph.end()) {
                    for (auto rit = it->second.rbegin(); rit != it->second.rend(); ++rit) {
                        if (visited.find(*rit) == visited.end()) {
                            s.push(*rit);
                        }
                    }
                }
            }

            return result;
        }

        template<typename T>
        std::vector<T> topological_sort(const std::unordered_map<T, std::vector<T>>& graph) {
            std::unordered_map<T, int> in_degree;
            std::queue<T> q;
            std::vector<T> result;

            for (const auto& [node, neighbors] : graph) {
                if (in_degree.find(node) == in_degree.end()) {
                    in_degree[node] = 0;
                }
                for (const T& neighbor : neighbors) {
                    in_degree[neighbor]++;
                }
            }

            for (const auto& [node, degree] : in_degree) {
                if (degree == 0) q.push(node);
            }

            while (!q.empty()) {
                T node = q.front();
                q.pop();
                result.push_back(node);

                auto it = graph.find(node);
                if (it != graph.end()) {
                    for (const T& neighbor : it->second) {
                        in_degree[neighbor]--;
                        if (in_degree[neighbor] == 0) {
                            q.push(neighbor);
                        }
                    }
                }
            }

            if ((int)result.size() != (int)in_degree.size()) {
                return {};
            }

            return result;
        }

        // ============================================================
        // 14. 矩阵操作
        // ============================================================

        template<typename T>
        std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>>& matrix) {
            if (matrix.empty() || matrix[0].empty()) return {};

            int rows = (int)matrix.size(), cols = (int)matrix[0].size();
            std::vector<std::vector<T>> result(cols, std::vector<T>(rows));

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    result[j][i] = matrix[i][j];
                }
            }

            return result;
        }

        template<typename T>
        std::vector<std::vector<T>> rotate_90(const std::vector<std::vector<T>>& matrix) {
            if (matrix.empty() || matrix[0].empty()) return {};

            int n = (int)matrix.size(), m = (int)matrix[0].size();
            std::vector<std::vector<T>> result(m, std::vector<T>(n));

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    result[j][n - 1 - i] = matrix[i][j];
                }
            }

            return result;
        }

        template<typename T>
        std::vector<std::vector<T>> rotate_180(const std::vector<std::vector<T>>& matrix) {
            if (matrix.empty() || matrix[0].empty()) return {};

            int rows = (int)matrix.size(), cols = (int)matrix[0].size();
            std::vector<std::vector<T>> result(rows, std::vector<T>(cols));

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    result[rows - 1 - i][cols - 1 - j] = matrix[i][j];
                }
            }

            return result;
        }

        template<typename T>
        std::vector<std::vector<T>> matrix_multiply(
            const std::vector<std::vector<T>>& A,
            const std::vector<std::vector<T>>& B) {
            if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) return {};
            if (A[0].size() != B.size()) {
                throw std::invalid_argument("Matrix dimensions mismatch");
            }

            int rows = (int)A.size(), cols = (int)B[0].size(), inner = (int)A[0].size();
            std::vector<std::vector<T>> result(rows, std::vector<T>(cols, 0));

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    for (int k = 0; k < inner; k++) {
                        result[i][j] += A[i][k] * B[k][j];
                    }
                }
            }

            return result;
        }

        // ============================================================
        // 15. 双指针
        // ============================================================

        template<typename T>
        std::pair<int, int> two_sum(const std::vector<T>& arr, T target) {
            std::unordered_map<T, int> seen;
            for (int i = 0; i < (int)arr.size(); i++) {
                T complement = target - arr[i];
                auto it = seen.find(complement);
                if (it != seen.end()) {
                    return { it->second, i };
                }
                seen[arr[i]] = i;
            }
            return { -1, -1 };
        }

        template<typename T>
        std::vector<std::vector<T>> three_sum(std::vector<T> arr, T target) {
            std::vector<std::vector<T>> result;
            std::sort(arr.begin(), arr.end());

            for (size_t i = 0; i < arr.size(); i++) {
                if (i > 0 && arr[i] == arr[i - 1]) continue;

                int left = (int)i + 1, right = (int)arr.size() - 1;
                while (left < right) {
                    T sum = arr[i] + arr[left] + arr[right];
                    if (sum == target) {
                        result.push_back({ arr[i], arr[left], arr[right] });
                        while (left < right && arr[left] == arr[left + 1]) left++;
                        while (left < right && arr[right] == arr[right - 1]) right--;
                        left++;
                        right--;
                    }
                    else if (sum < target) {
                        left++;
                    }
                    else {
                        right--;
                    }
                }
            }

            return result;
        }

        // ============================================================
        // 16. 二分查找-变体
        // ============================================================

        template<typename T>
        int first_equal(const std::vector<T>& arr, T target) {
            int l = 0, r = (int)arr.size() - 1;
            int result = -1;
            while (l <= r) {
                int mid = l + (r - l) / 2;
                if (arr[mid] == target) {
                    result = mid;
                    r = mid - 1;
                }
                else if (arr[mid] < target) {
                    l = mid + 1;
                }
                else {
                    r = mid - 1;
                }
            }
            return result;
        }

        template<typename T>
        int last_equal(const std::vector<T>& arr, T target) {
            int l = 0, r = (int)arr.size() - 1;
            int result = -1;
            while (l <= r) {
                int mid = l + (r - l) / 2;
                if (arr[mid] == target) {
                    result = mid;
                    l = mid + 1;
                }
                else if (arr[mid] < target) {
                    l = mid + 1;
                }
                else {
                    r = mid - 1;
                }
            }
            return result;
        }

        // ============================================================
        // 17. 随机
        // ============================================================

        template<typename Container>
        void fisher_yates_shuffle(Container& c) {
            static std::mt19937 gen(
                (unsigned)std::chrono::steady_clock::now().time_since_epoch().count()
            );

            for (int i = (int)c.size() - 1; i > 0; i--) {
                std::uniform_int_distribution<int> dis(0, i);
                int j = dis(gen);
                std::swap(c[i], c[j]);
            }
        }

        template<typename T>
        std::vector<T> sample_with_replacement(const std::vector<T>& population, int k) {
            if (population.empty() || k <= 0) return {};

            static std::mt19937 gen(
                (unsigned)std::chrono::steady_clock::now().time_since_epoch().count()
            );

            std::vector<T> result;
            result.reserve(k);
            std::uniform_int_distribution<int> dis(0, (int)population.size() - 1);

            for (int i = 0; i < k; i++) {
                result.push_back(population[dis(gen)]);
            }

            return result;
        }

        // ============================================================
        // 18. 分治 - 最大子数组和
        // ============================================================

        namespace detail {
            template<typename T>
            T max_subarray_sum_divide_impl(const std::vector<T>& arr, int l, int r) {
                if (l == r) return arr[l];

                int mid = l + (r - l) / 2;
                T left_max = max_subarray_sum_divide_impl(arr, l, mid);
                T right_max = max_subarray_sum_divide_impl(arr, mid + 1, r);

                T left_border = arr[mid];
                T right_border = arr[mid + 1];
                T temp = 0;

                for (int i = mid; i >= l; i--) {
                    temp += arr[i];
                    left_border = (std::max)(left_border, temp);
                }

                temp = 0;
                for (int i = mid + 1; i <= r; i++) {
                    temp += arr[i];
                    right_border = (std::max)(right_border, temp);
                }

                T cross_max = left_border + right_border;
                T max_val = left_max;
                if (right_max > max_val) max_val = right_max;
                if (cross_max > max_val) max_val = cross_max;
                return max_val;
            }
        }

        template<typename T>
        T max_subarray_sum_divide(const std::vector<T>& arr) {
            if (arr.empty()) return 0;
            return detail::max_subarray_sum_divide_impl(arr, 0, (int)arr.size() - 1);
        }

        // ============================================================
        // 19. 归并
        // ============================================================

        namespace detail {
            template<typename T>
            void merge_sort_impl(std::vector<T>& arr, int l, int r) {
                if (l >= r) return;

                int mid = l + (r - l) / 2;
                merge_sort_impl(arr, l, mid);
                merge_sort_impl(arr, mid + 1, r);

                std::vector<T> temp(r - l + 1);
                int i = l, j = mid + 1, k = 0;

                while (i <= mid && j <= r) {
                    if (arr[i] <= arr[j]) {
                        temp[k++] = arr[i++];
                    }
                    else {
                        temp[k++] = arr[j++];
                    }
                }

                while (i <= mid) temp[k++] = arr[i++];
                while (j <= r) temp[k++] = arr[j++];

                for (int idx = 0; idx < (int)temp.size(); idx++) {
                    arr[l + idx] = temp[idx];
                }
            }
        }

        template<typename T>
        void merge_sort(std::vector<T>& arr) {
            if (!arr.empty()) {
                detail::merge_sort_impl(arr, 0, (int)arr.size() - 1);
            }
        }

        // ============================================================
        // 20. 快排
        // ============================================================

        namespace detail {
            template<typename T>
            int qsort_partition(std::vector<T>& arr, int l, int r) {
                int mid = l + (r - l) / 2;
                if (arr[l] > arr[mid]) std::swap(arr[l], arr[mid]);
                if (arr[l] > arr[r]) std::swap(arr[l], arr[r]);
                if (arr[mid] > arr[r]) std::swap(arr[mid], arr[r]);
                std::swap(arr[mid], arr[r]);

                T pivot = arr[r];
                int i = l - 1;

                for (int j = l; j < r; j++) {
                    if (arr[j] <= pivot) {
                        i++;
                        std::swap(arr[i], arr[j]);
                    }
                }

                std::swap(arr[i + 1], arr[r]);
                return i + 1;
            }

            template<typename T>
            void quick_sort_impl(std::vector<T>& arr, int l, int r) {
                if (l >= r) return;

                int pivot_idx = qsort_partition(arr, l, r);
                quick_sort_impl(arr, l, pivot_idx - 1);
                quick_sort_impl(arr, pivot_idx + 1, r);
            }
        }

        template<typename T>
        void quick_sort(std::vector<T>& arr) {
            if (!arr.empty()) {
                detail::quick_sort_impl(arr, 0, (int)arr.size() - 1);
            }
        }

        // ============================================================
        // 21. 堆操作
        // ============================================================

        template<typename Container>
        void make_heap(Container& c) {
            std::make_heap(c.begin(), c.end());
        }

        template<typename Container, typename Compare>
        void make_heap(Container& c, Compare comp) {
            std::make_heap(c.begin(), c.end(), comp);
        }

        template<typename Container>
        void heap_sort(Container& c) {
            std::make_heap(c.begin(), c.end());
            std::sort_heap(c.begin(), c.end());
        }

        template<typename Container, typename Compare>
        void heap_sort(Container& c, Compare comp) {
            std::make_heap(c.begin(), c.end(), comp);
            std::sort_heap(c.begin(), c.end(), comp);
        }

        template<typename Container>
        bool is_heap(const Container& c) {
            return std::is_heap(c.begin(), c.end());
        }

        // ============================================================
        // 22. 统计量
        // ============================================================

        template<typename T>
        double mean(const std::vector<T>& arr) {
            if (arr.empty()) return 0;
            T sum = std::accumulate(arr.begin(), arr.end(), T(0));
            return (double)sum / arr.size();
        }

        template<typename T>
        double median(std::vector<T> arr) {
            if (arr.empty()) return 0;
            std::sort(arr.begin(), arr.end());
            size_t n = arr.size();
            if (n % 2 == 1) {
                return arr[n / 2];
            }
            else {
                return (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
            }
        }

        template<typename T>
        std::vector<T> mode(const std::vector<T>& arr) {
            if (arr.empty()) return {};

            std::unordered_map<T, int> freq;
            int max_freq = 0;

            for (const T& val : arr) {
                int count = ++freq[val];
                if (count > max_freq) max_freq = count;
            }

            std::vector<T> result;
            for (const auto& [val, count] : freq) {
                if (count == max_freq) result.push_back(val);
            }

            return result;
        }

        template<typename T>
        double variance(const std::vector<T>& arr, bool population = true) {
            if (arr.size() < 2) return 0;

            double m = mean(arr);
            double sum_sq = 0;
            for (const T& val : arr) {
                double diff = (double)val - m;
                sum_sq += diff * diff;
            }

            int divisor = population ? (int)arr.size() : (int)arr.size() - 1;
            return sum_sq / divisor;
        }

        template<typename T>
        double stddev(const std::vector<T>& arr, bool population = true) {
            return std::sqrt(variance(arr, population));
        }

        // ============================================================
        // 23. 位运算
        // ============================================================

        inline int popcount(unsigned int x) {
#ifdef _MSC_VER
            return __popcnt(x);
#else
            return __builtin_popcount(x);
#endif
        }

        inline int popcount(unsigned long long x) {
#ifdef _MSC_VER
            return __popcnt64(x);
#else
            return __builtin_popcountll(x);
#endif
        }

        inline bool is_power_of_two(unsigned int x) {
            return x > 0 && (x & (x - 1)) == 0;
        }

        inline int lowbit_pos(unsigned int x) {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanForward(&index, x);
            return (int)index;
#else
            return __builtin_ctz(x);
#endif
        }

        inline int lowbit_pos(unsigned long long x) {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanForward64(&index, x);
            return (int)index;
#else
            return __builtin_ctzll(x);
#endif
        }

        template<typename T>
        T lowbit(T x) {
            return x & -x;
        }

        inline int highbit_pos(unsigned int x) {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanReverse(&index, x);
            return (int)index;
#else
            return 31 - __builtin_clz(x);
#endif
        }

        inline int highbit_pos(unsigned long long x) {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanReverse64(&index, x);
            return (int)index;
#else
            return 63 - __builtin_clzll(x);
#endif
        }

        // ============================================================
        // 24. 数值算法
        // ============================================================

        inline double sqrt_newton(double x, double eps = 1e-12) {
            if (x < 0) return -1;
            if (x == 0) return 0;

            double guess = x / 2;
            while (true) {
                double next = (guess + x / guess) / 2;
                if (std::abs(next - guess) < eps) break;
                guess = next;
            }
            return guess;
        }

        template<typename T>
        T fast_pow(T base, int exp) {
            T result = 1;
            while (exp > 0) {
                if (exp & 1) result *= base;
                base *= base;
                exp >>= 1;
            }
            return result;
        }

        inline bool is_palindrome_number(int x) {
            if (x < 0) return false;
            if (x < 10) return true;
            if (x % 10 == 0) return false;

            int reversed = 0;
            while (x > reversed) {
                reversed = reversed * 10 + x % 10;
                x /= 10;
            }
            return x == reversed || x == reversed / 10;
        }

        inline int reverse_number(int x) {
            int result = 0;
            while (x != 0) {
                if (result > INT_MAX / 10 || result < INT_MIN / 10) {
                    return 0;
                }
                result = result * 10 + x % 10;
                x /= 10;
            }
            return result;
        }

    } // namespace algorithm

#ifdef LAZY_EXPORT_ALL
    using namespace algorithm;
#endif

} // namespace lazy