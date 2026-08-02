// lazy_teaching.h
// copyright 2026 lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <functional>
#include <memory>
#include <cmath>

#include "lazy_core.h"
#include "lazy_container.h"
#include "lazy_string.h"
#include "lazy_io.h"

namespace lazy {
    namespace teaching {

        // ============================================================
        // 1. 可视化辅助工具
        // ============================================================

        // 可视化配置
        struct VisualConfig {
            int bar_width = 40;
            char fill_char = '█';
            char empty_char = '?';
            bool show_index = true;
            bool show_value = true;
            bool color_enabled = false;
        };

        // 数组可视化
        template<typename T>
        class ArrayVisualizer {
        private:
            std::vector<T> data_;
            VisualConfig config_;
            std::vector<int> highlight_;
            std::vector<int> compare_;

        public:
            ArrayVisualizer(const std::vector<T>& data, const VisualConfig& config = VisualConfig())
                : data_(data), config_(config) {
            }

            void highlight(int index) {
                highlight_.push_back(index);
            }

            void highlight(const std::vector<int>& indices) {
                highlight_.insert(highlight_.end(), indices.begin(), indices.end());
            }

            void compare(int index1, int index2) {
                compare_ = { index1, index2 };
            }

            void clear_highlights() {
                highlight_.clear();
                compare_.clear();
            }

            void print(bool with_index = true) const {
                // 打印值
                std::string separator = "  ";
                if (with_index) {
                    // 打印索引
                    for (size_t i = 0; i < data_.size(); i++) {
                        std::cout << std::setw(3) << i << separator;
                    }
                    std::cout << "\n";
                }

                // 打印分隔线
                for (size_t i = 0; i < data_.size(); i++) {
                    std::cout << "---" << separator;
                }
                std::cout << "\n";

                // 打印值
                for (size_t i = 0; i < data_.size(); i++) {
                    bool is_highlight = std::find(highlight_.begin(), highlight_.end(), i) != highlight_.end();
                    bool is_compare = std::find(compare_.begin(), compare_.end(), i) != compare_.end();

                    if (is_highlight) {
                        std::cout << "[" << std::setw(2) << data_[i] << "]";
                    }
                    else if (is_compare) {
                        std::cout << "(" << std::setw(2) << data_[i] << ")";
                    }
                    else {
                        std::cout << " " << std::setw(2) << data_[i] << " ";
                    }
                    std::cout << separator;
                }
                std::cout << "\n\n";
            }

            // 打印柱状图
            void print_bars() const {
                T max_val = *std::max_element(data_.begin(), data_.end());
                if (max_val == 0) max_val = 1;

                for (size_t i = 0; i < data_.size(); i++) {
                    int bar_len = (int)((double)data_[i] / max_val * config_.bar_width);

                    // 打印索引
                    if (config_.show_index) {
                        std::cout << std::setw(3) << i << " ";
                    }

                    // 打印柱状条
                    for (int j = 0; j < config_.bar_width; j++) {
                        if (j < bar_len) {
                            std::cout << config_.fill_char;
                        }
                        else {
                            std::cout << config_.empty_char;
                        }
                    }

                    // 打印值
                    if (config_.show_value) {
                        std::cout << " " << data_[i];
                    }

                    std::cout << "\n";
                }
                std::cout << "\n";
            }
        };

        // ============================================================
        // 2. 排序可视化（教学演示）
        // ============================================================

        template<typename T>
        class SortVisualizer {
        private:
            std::vector<T> data_;
            std::vector<std::string> steps_;
            int step_count_ = 0;
            bool animated_ = false;

            void record_step(const std::string& description = "") {
                std::ostringstream oss;
                oss << "Step " << ++step_count_ << ": ";
                for (size_t i = 0; i < data_.size(); i++) {
                    oss << data_[i];
                    if (i < data_.size() - 1) oss << " ";
                }
                if (!description.empty()) {
                    oss << "  // " << description;
                }
                steps_.push_back(oss.str());
            }

        public:
            SortVisualizer(const std::vector<T>& data) : data_(data) {
                record_step("Initial array");
            }

            void enable_animation(bool enable = true) {
                animated_ = enable;
            }

            void print_steps() const {
                for (const auto& step : steps_) {
                    std::cout << step << "\n";
                }
            }

            void print_step(int step) const {
                if (step >= 0 && step < (int)steps_.size()) {
                    std::cout << steps_[step] << "\n";
                }
            }

            int total_steps() const {
                return steps_.size();
            }

            // ========== 排序算法（教学版） ==========

            // 冒泡排序（带可视化）
            void bubble_sort() {
                int n = data_.size();
                for (int i = 0; i < n - 1; i++) {
                    bool swapped = false;
                    for (int j = 0; j < n - i - 1; j++) {
                        if (data_[j] > data_[j + 1]) {
                            std::swap(data_[j], data_[j + 1]);
                            swapped = true;
                            record_step("Swap " + std::to_string(data_[j + 1]) + " and " + std::to_string(data_[j]));
                            if (animated_) {
                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            }
                        }
                    }
                    if (!swapped) break;
                    record_step("Pass " + std::to_string(i + 1) + " completed");
                }
                record_step("Sorted!");
            }

            // 选择排序
            void selection_sort() {
                int n = data_.size();
                for (int i = 0; i < n - 1; i++) {
                    int min_idx = i;
                    for (int j = i + 1; j < n; j++) {
                        if (data_[j] < data_[min_idx]) {
                            min_idx = j;
                        }
                    }
                    if (min_idx != i) {
                        std::swap(data_[i], data_[min_idx]);
                        record_step("Place " + std::to_string(data_[i]) + " at position " + std::to_string(i));
                        if (animated_) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        }
                    }
                }
                record_step("Sorted!");
            }

            // 插入排序
            void insertion_sort() {
                int n = data_.size();
                for (int i = 1; i < n; i++) {
                    T key = data_[i];
                    int j = i - 1;
                    while (j >= 0 && data_[j] > key) {
                        data_[j + 1] = data_[j];
                        j--;
                    }
                    data_[j + 1] = key;
                    record_step("Insert " + std::to_string(key) + " at position " + std::to_string(j + 1));
                    if (animated_) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    }
                }
                record_step("Sorted!");
            }

            // 快速排序（教学版）
            void quick_sort() {
                quick_sort_helper(0, data_.size() - 1);
                record_step("Sorted!");
            }

        private:
            void quick_sort_helper(int low, int high) {
                if (low < high) {
                    int pi = partition(low, high);
                    record_step("Pivot " + std::to_string(data_[pi]) + " at position " + std::to_string(pi));
                    if (animated_) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    }
                    quick_sort_helper(low, pi - 1);
                    quick_sort_helper(pi + 1, high);
                }
            }

            int partition(int low, int high) {
                T pivot = data_[high];
                int i = low - 1;
                for (int j = low; j < high; j++) {
                    if (data_[j] < pivot) {
                        i++;
                        std::swap(data_[i], data_[j]);
                    }
                }
                std::swap(data_[i + 1], data_[high]);
                return i + 1;
            }
        };

        // ============================================================
        // 3. 数据结构可视化
        // ============================================================

        // 链表可视化
        template<typename T>
        class LinkedListVisualizer {
        private:
            struct Node {
                T data;
                std::shared_ptr<Node> next;
                Node(const T& d) : data(d), next(nullptr) {}
            };

            std::shared_ptr<Node> head_;
            size_t size_ = 0;

        public:
            void push_front(const T& data) {
                auto node = std::make_shared<Node>(data);
                node->next = head_;
                head_ = node;
                size_++;
            }

            void push_back(const T& data) {
                auto node = std::make_shared<Node>(data);
                if (!head_) {
                    head_ = node;
                }
                else {
                    auto current = head_;
                    while (current->next) {
                        current = current->next;
                    }
                    current->next = node;
                }
                size_++;
            }

            void print(bool show_addresses = false) const {
                auto current = head_;
                int index = 0;
                std::cout << "List: ";
                while (current) {
                    if (show_addresses) {
                        std::cout << "[" << current.get() << "] ";
                    }
                    std::cout << current->data;
                    if (current->next) {
                        std::cout << " -> ";
                    }
                    current = current->next;
                    index++;
                }
                std::cout << " (nullptr)\n";
                std::cout << "Size: " << size_ << "\n\n";
            }

            // 反转并可视化每一步
            void reverse_with_visualization() {
                std::cout << "Reversing list step by step:\n";
                std::shared_ptr<Node> prev = nullptr;
                auto current = head_;
                auto next = head_;

                while (current) {
                    next = current->next;
                    current->next = prev;

                    std::cout << "  Step: ";
                    print_current_state(prev, current, next);
                    std::cout << "\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));

                    prev = current;
                    current = next;
                }
                head_ = prev;
                std::cout << "Reversed!\n\n";
            }

        private:
            void print_current_state(std::shared_ptr<Node> prev,
                std::shared_ptr<Node> current,
                std::shared_ptr<Node> next) const {
                std::cout << "prev=";
                if (prev) std::cout << prev->data; else std::cout << "null";
                std::cout << ", curr=";
                if (current) std::cout << current->data; else std::cout << "null";
                std::cout << ", next=";
                if (next) std::cout << next->data; else std::cout << "null";
            }
        };

        // ============================================================
        // 4. 递归可视化
        // ============================================================

        class RecursionVisualizer {
        private:
            int depth_ = 0;
            bool enabled_ = true;

            void print_indent() const {
                for (int i = 0; i < depth_; i++) {
                    std::cout << "  ";
                }
            }

        public:
            void enable(bool enable = true) {
                enabled_ = enable;
            }

            // 递归调用追踪
            template<typename Func, typename... Args>
            auto trace(Func&& func, const std::string& name, Args&&... args)
                -> decltype(func(args...)) {
                if (!enabled_) {
                    return func(args...);
                }

                print_indent();
                std::cout << "-> " << name << "(";
                print_args(args...);
                std::cout << ")\n";

                depth_++;
                auto result = func(std::forward<Args>(args)...);
                depth_--;

                print_indent();
                std::cout << "<- " << name << " returns " << result << "\n";

                return result;
            }

        private:
            template<typename First, typename... Rest>
            void print_args(First&& first, Rest&&... rest) const {
                std::cout << first;
                if constexpr (sizeof...(rest) > 0) {
                    std::cout << ", ";
                    print_args(std::forward<Rest>(rest)...);
                }
            }

            void print_args() const {}
        };

        // ============================================================
        // 5. 数学概念可视化
        // ============================================================

        // 斐波那契数列（教学版）
        class FibonacciVisualizer {
        private:
            std::map<int, int> memo_;
            RecursionVisualizer tracer_;

        public:
            // 递归版本（带可视化）
            int fib_recursive(int n) {
                if (n <= 1) return n;
                std::cout << "fib(" << n << ") = fib(" << n - 1 << ") + fib(" << n - 2 << ")\n";
                return fib_recursive(n - 1) + fib_recursive(n - 2);
            }

            // 记忆化版本（带可视化）
            int fib_memo(int n) {
                if (n <= 1) return n;
                if (memo_.find(n) != memo_.end()) {
                    std::cout << "fib(" << n << ") = " << memo_[n] << " (cached)\n";
                    return memo_[n];
                }
                std::cout << "fib(" << n << ") = fib(" << n - 1 << ") + fib(" << n - 2 << ")\n";
                memo_[n] = fib_memo(n - 1) + fib_memo(n - 2);
                return memo_[n];
            }

            // 打印递归树（简化版）
            void print_tree(int n, int indent = 0) {
                for (int i = 0; i < indent; i++) std::cout << "  ";
                if (n <= 1) {
                    std::cout << "fib(" << n << ") = " << n << "\n";
                    return;
                }
                std::cout << "fib(" << n << ")\n";
                print_tree(n - 1, indent + 1);
                print_tree(n - 2, indent + 1);
            }
        };

        // ============================================================
        // 6. 算法步骤追踪器
        // ============================================================

        class StepTracer {
        private:
            std::vector<std::string> steps_;
            std::vector<std::chrono::microseconds> timings_;
            bool verbose_ = true;

        public:
            void enable_verbose(bool enable = true) {
                verbose_ = enable;
            }

            void step(const std::string& description, bool auto_log = true) {
                auto now = std::chrono::high_resolution_clock::now();
                static auto start = now;

                auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
                steps_.push_back(description);
                timings_.push_back(elapsed);

                if (verbose_ && auto_log) {
                    std::cout << "[Step " << steps_.size() << "] "
                        << description << "  (+" << elapsed.count() << "μs)\n";
                }
            }

            void print_report() const {
                std::cout << "\n=== Step Report ===\n";
                for (size_t i = 0; i < steps_.size(); i++) {
                    std::cout << "  Step " << std::setw(2) << i + 1 << ": " << steps_[i];
                    if (i < timings_.size()) {
                        std::cout << "  (" << timings_[i].count() << " μs)";
                    }
                    std::cout << "\n";
                }
                if (!timings_.empty()) {
                    auto total = timings_.back().count() - timings_.front().count();
                    std::cout << "  Total: " << total << " μs\n";
                }
                std::cout << "========================\n\n";
            }

            void clear() {
                steps_.clear();
                timings_.clear();
            }
        };

        // ============================================================
        // 7. 二分查找可视化
        // ============================================================

        template<typename T>
        class BinarySearchVisualizer {
        private:
            std::vector<T> data_;

        public:
            BinarySearchVisualizer(const std::vector<T>& data) : data_(data) {
                std::sort(data_.begin(), data_.end());
            }

            int search(const T& target, bool visualize = true) {
                int left = 0, right = data_.size() - 1;
                int steps = 0;

                std::cout << "Searching for " << target << " in sorted array:\n";
                print_array(left, right, -1);

                while (left <= right) {
                    int mid = left + (right - left) / 2;
                    steps++;

                    std::cout << "\nStep " << steps << ": ";
                    std::cout << "left=" << left << ", right=" << right << ", mid=" << mid;
                    std::cout << ", data[mid]=" << data_[mid] << "\n";

                    if (visualize) {
                        print_array(left, right, mid);
                    }

                    if (data_[mid] == target) {
                        std::cout << "Found at index " << mid << "!\n";
                        return mid;
                    }

                    if (data_[mid] < target) {
                        std::cout << "  " << data_[mid] << " < " << target
                            << ", searching right half\n";
                        left = mid + 1;
                    }
                    else {
                        std::cout << "  " << data_[mid] << " > " << target
                            << ", searching left half\n";
                        right = mid - 1;
                    }
                }

                std::cout << "Not found!\n";
                return -1;
            }

        private:
            void print_array(int left, int right, int mid) const {
                std::cout << "  ";
                for (size_t i = 0; i < data_.size(); i++) {
                    if ((int)i == mid) {
                        std::cout << "[";
                    }
                    else if ((int)i == left || (int)i == right) {
                        std::cout << "(";
                    }
                    else {
                        std::cout << " ";
                    }
                    std::cout << data_[i];
                    if ((int)i == mid) {
                        std::cout << "]";
                    }
                    else if ((int)i == left || (int)i == right) {
                        std::cout << ")";
                    }
                    else {
                        std::cout << " ";
                    }
                    std::cout << " ";
                }
                std::cout << "\n";
            }
        };

        // ============================================================
        // 8. 数据结构操作教学
        // ============================================================

        // 栈操作演示
        template<typename T>
        class StackDemo {
        private:
            std::stack<T> stack_;

        public:
            void push(const T& value) {
                std::cout << "push(" << value << ") -> ";
                stack_.push(value);
                print();
            }

            void pop() {
                if (stack_.empty()) {
                    std::cout << "pop() -> EMPTY!\n";
                    return;
                }
                T value = stack_.top();
                stack_.pop();
                std::cout << "pop() -> " << value << " -> ";
                print();
            }

            void top() const {
                if (stack_.empty()) {
                    std::cout << "top() -> EMPTY!\n";
                    return;
                }
                std::cout << "top() -> " << stack_.top() << "\n";
            }

            void print() const {
                std::cout << "Stack: [";
                auto temp = stack_;
                std::vector<T> items;
                while (!temp.empty()) {
                    items.push_back(temp.top());
                    temp.pop();
                }
                for (int i = items.size() - 1; i >= 0; i--) {
                    std::cout << items[i];
                    if (i > 0) std::cout << ", ";
                }
                std::cout << "] (top=";
                if (!stack_.empty()) std::cout << stack_.top();
                else std::cout << "empty";
                std::cout << ")\n";
            }
        };

        // 队列操作演示
        template<typename T>
        class QueueDemo {
        private:
            std::queue<T> queue_;

        public:
            void enqueue(const T& value) {
                std::cout << "enqueue(" << value << ") -> ";
                queue_.push(value);
                print();
            }

            void dequeue() {
                if (queue_.empty()) {
                    std::cout << "dequeue() -> EMPTY!\n";
                    return;
                }
                T value = queue_.front();
                queue_.pop();
                std::cout << "dequeue() -> " << value << " -> ";
                print();
            }

            void front() const {
                if (queue_.empty()) {
                    std::cout << "front() -> EMPTY!\n";
                    return;
                }
                std::cout << "front() -> " << queue_.front() << "\n";
            }

            void print() const {
                std::cout << "Queue: [";
                auto temp = queue_;
                bool first = true;
                while (!temp.empty()) {
                    if (!first) std::cout << ", ";
                    std::cout << temp.front();
                    temp.pop();
                    first = false;
                }
                std::cout << "] (front=";
                if (!queue_.empty()) std::cout << queue_.front();
                else std::cout << "empty";
                std::cout << ", back=";
                if (!queue_.empty()) std::cout << queue_.back();
                else std::cout << "empty";
                std::cout << ")\n";
            }
        };

        // ============================================================
        // 9. 复杂度分析工具
        // ============================================================

        class ComplexityAnalyzer {
        private:
            std::map<std::string, std::vector<double>> measurements_;

        public:
            template<typename Func>
            double measure(const std::string& name, Func&& func, int repetitions = 5) {
                std::vector<double> times;
                for (int i = 0; i < repetitions; i++) {
                    auto start = std::chrono::high_resolution_clock::now();
                    func();
                    auto end = std::chrono::high_resolution_clock::now();
                    double time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                    times.push_back(time);
                }

                // 去除最大最小值，取平均
                std::sort(times.begin(), times.end());
                double sum = 0;
                for (int i = 1; i < repetitions - 1; i++) {
                    sum += times[i];
                }
                double avg = sum / (repetitions - 2);

                measurements_[name].push_back(avg);
                return avg;
            }

            void print_report() const {
                std::cout << "\n=== Complexity Analysis Report ===\n";
                for (const auto& [name, times] : measurements_) {
                    std::cout << name << ":\n";
                    for (size_t i = 0; i < times.size(); i++) {
                        std::cout << "  n=" << (i + 1) * 100 << ": "
                            << times[i] << " μs\n";
                    }
                }
                std::cout << "====================================\n\n";
            }
        };
        
        // ============================================================
        // 10. 算法比较器
        // ============================================================

        class AlgorithmComparator {
        private:
            std::map<std::string, std::function<void()>> algorithms_;
            std::vector<int> test_sizes_;

        public:
            void add_algorithm(const std::string& name, std::function<void()> algo) {
                algorithms_[name] = algo;
            }

            void test_sizes(const std::vector<int>& sizes) {
                test_sizes_ = sizes;
            }

            void compare(int repetitions = 3) {
                ComplexityAnalyzer analyzer;

                std::cout << "\n=== Algorithm Comparison ===\n";
                for (const auto& [name, algo] : algorithms_) {
                    std::cout << "\nTesting: " << name << "\n";
                    for (int size : test_sizes_) {
                        double avg = analyzer.measure(name + "_n=" + std::to_string(size), algo, repetitions);
                        std::cout << "  n=" << size << ": " << avg << " μs\n";
                    }
                }
                std::cout << "============================\n\n";
            }
        };

        // ============================================================
        // 11. 交互式教学工具
        // ============================================================

        class InteractiveTutor {
        private:
            std::vector<std::string> history_;
            int current_step_ = 0;

        public:
            void print_help() const {
                std::cout << R"(
=== Interactive Tutor Help ===
  next    - Next step
  prev    - Previous step
  reset   - Reset to beginning
  show    - Show current state
  help    - Show this help
  quit    - Exit tutor
===============================
)";
            }

            void record_state(const std::string& state) {
                history_.push_back(state);
                current_step_ = history_.size() - 1;
            }

            void next() {
                if (current_step_ < (int)history_.size() - 1) {
                    current_step_++;
                    show_current();
                }
                else {
                    std::cout << "Already at the end.\n";
                }
            }

            void prev() {
                if (current_step_ > 0) {
                    current_step_--;
                    show_current();
                }
                else {
                    std::cout << "Already at the beginning.\n";
                }
            }

            void reset() {
                current_step_ = 0;
                show_current();
            }

            void show_current() const {
                if (current_step_ < (int)history_.size()) {
                    std::cout << "Step " << current_step_ << ": "
                        << history_[current_step_] << "\n";
                }
            }

            void run() {
                std::string command;
                print_help();
                while (true) {
                    std::cout << "> ";
                    std::getline(std::cin, command);

                    if (command == "next") next();
                    else if (command == "prev") prev();
                    else if (command == "reset") reset();
                    else if (command == "show") show_current();
                    else if (command == "help") print_help();
                    else if (command == "quit") break;
                    else std::cout << "Unknown command. Type 'help' for options.\n";
                }
            }
        };

        // ============================================================
        // 12. 教学辅助函数
        // ============================================================

        // 生成教学用的数组
        template<typename T>
        std::vector<T> generate_teaching_array(int size, bool sorted = false, bool random = true) {
            std::vector<T> result(size);

            if (random) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<T> dis(0, 100);
                for (int i = 0; i < size; i++) {
                    result[i] = dis(gen);
                }
            }
            else {
                for (int i = 0; i < size; i++) {
                    result[i] = i + 1;
                }
            }

            if (sorted) {
                std::sort(result.begin(), result.end());
            }

            return result;
        }

        // 打印数组（带标记）
        template<typename T>
        void print_array(const std::vector<T>& arr,
            const std::string& name = "Array",
            int highlight1 = -1,
            int highlight2 = -1) {
            std::cout << name << ":\n  ";
            for (size_t i = 0; i < arr.size(); i++) {
                if ((int)i == highlight1 || (int)i == highlight2) {
                    std::cout << "[";
                }
                else {
                    std::cout << " ";
                }
                std::cout << arr[i];
                if ((int)i == highlight1 || (int)i == highlight2) {
                    std::cout << "]";
                }
                else {
                    std::cout << " ";
                }
                std::cout << " ";
            }
            std::cout << "\n";
        }

        // 打印2D数组（教学用）
        template<typename T>
        void print_2d_array(const std::vector<std::vector<T>>& arr) {
            for (const auto& row : arr) {
                for (const auto& val : row) {
                    std::cout << std::setw(4) << val;
                }
                std::cout << "\n";
            }
        }

    } // namespace teaching

#ifdef LAZY_EXPORT_ALL
    using namespace teaching;
#endif
} // namespace lazy