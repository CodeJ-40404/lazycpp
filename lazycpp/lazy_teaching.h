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

// 跨平台清屏支持
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "lazy_core.h"
#include "lazy_container.h"
#include "lazy_string.h"
#include "lazy_io.h"

namespace lazy {
    namespace teaching {

        // ============================================================
        // 语言设置
        // ============================================================
        enum class Language {
            ENGLISH, // ENGLISHH!
            CHINESE // 让我们说中文
        };

        class LanguageManager {
        private:
            static Language current_language_;
            static std::map<std::string, std::map<Language, std::string>> strings_;

            static void init_strings() {
                // 排序相关
                strings_["sort_start"] = {
                    {Language::ENGLISH, "Starting Sort"},
                    {Language::CHINESE, "开始排序"}
                };
                strings_["sort_sorted"] = {
                    {Language::ENGLISH, "Array is sorted"},
                    {Language::CHINESE, "数组已排序完成"}
                };
                strings_["sort_comparing"] = {
                    {Language::ENGLISH, "Comparing"},
                    {Language::CHINESE, "比较"}
                };
                strings_["sort_and"] = {
                    {Language::ENGLISH, "and"},
                    {Language::CHINESE, "和"}
                };
                strings_["sort_swapped"] = {
                    {Language::ENGLISH, "Swapped"},
                    {Language::CHINESE, "交换了"}
                };
                strings_["sort_pass"] = {
                    {Language::ENGLISH, "Pass"},
                    {Language::CHINESE, "第"}
                };
                strings_["sort_pass_completed"] = {
                    {Language::ENGLISH, "completed"},
                    {Language::CHINESE, "轮完成"}
                };
                strings_["sort_finding_min"] = {
                    {Language::ENGLISH, "Finding min: comparing"},
                    {Language::CHINESE, "寻找最小值: 比较"}
                };
                strings_["sort_with_current_min"] = {
                    {Language::ENGLISH, "with current min"},
                    {Language::CHINESE, "与当前最小值"}
                };
                strings_["sort_new_min"] = {
                    {Language::ENGLISH, "New min found:"},
                    {Language::CHINESE, "找到新的最小值:"}
                };
                strings_["sort_at_index"] = {
                    {Language::ENGLISH, "at index"},
                    {Language::CHINESE, "在索引"}
                };
                strings_["sort_placed"] = {
                    {Language::ENGLISH, "Placed"},
                    {Language::CHINESE, "放置了"}
                };
                strings_["sort_at_position"] = {
                    {Language::ENGLISH, "at position"},
                    {Language::CHINESE, "到位置"}
                };
                strings_["sort_inserting"] = {
                    {Language::ENGLISH, "Inserting"},
                    {Language::CHINESE, "插入"}
                };
                strings_["sort_into_sorted"] = {
                    {Language::ENGLISH, "into sorted portion"},
                    {Language::CHINESE, "到已排序部分"}
                };
                strings_["sort_shifting"] = {
                    {Language::ENGLISH, "Shifting"},
                    {Language::CHINESE, "右移"}
                };
                strings_["sort_right"] = {
                    {Language::ENGLISH, "right"},
                    {Language::CHINESE, ""}
                };
                strings_["sort_inserted"] = {
                    {Language::ENGLISH, "Inserted"},
                    {Language::CHINESE, "插入了"}
                };
                strings_["sort_pivot"] = {
                    {Language::ENGLISH, "Pivot"},
                    {Language::CHINESE, "基准值"}
                };
                strings_["sort_partition"] = {
                    {Language::ENGLISH, "Partition: comparing"},
                    {Language::CHINESE, "分区: 比较"}
                };
                strings_["sort_with_pivot"] = {
                    {Language::ENGLISH, "with pivot"},
                    {Language::CHINESE, "与基准值"}
                };

                // 搜索相关
                strings_["search_start"] = {
                    {Language::ENGLISH, "Searching for"},
                    {Language::CHINESE, "正在搜索"}
                };
                strings_["search_in_sorted"] = {
                    {Language::ENGLISH, "in sorted array"},
                    {Language::CHINESE, "在已排序数组中"}
                };
                strings_["search_found"] = {
                    {Language::ENGLISH, "Found at index"},
                    {Language::CHINESE, "找到于索引"}
                };
                strings_["search_not_found"] = {
                    {Language::ENGLISH, "Not found"},
                    {Language::CHINESE, "未找到"}
                };
                strings_["search_left_half"] = {
                    {Language::ENGLISH, "searching left half"},
                    {Language::CHINESE, "搜索左半部分"}
                };
                strings_["search_right_half"] = {
                    {Language::ENGLISH, "searching right half"},
                    {Language::CHINESE, "搜索右半部分"}
                };

                // 栈相关
                strings_["stack_empty"] = {
                    {Language::ENGLISH, "EMPTY"},
                    {Language::CHINESE, "空"}
                };
                strings_["stack_top"] = {
                    {Language::ENGLISH, "top"},
                    {Language::CHINESE, "栈顶"}
                };

                // 队列相关
                strings_["queue_front"] = {
                    {Language::ENGLISH, "front"},
                    {Language::CHINESE, "队首"}
                };
                strings_["queue_back"] = {
                    {Language::ENGLISH, "back"},
                    {Language::CHINESE, "队尾"}
                };

                // 链表相关
                strings_["list_reversing"] = {
                    {Language::ENGLISH, "Reversing List"},
                    {Language::CHINESE, "反转链表"}
                };
                strings_["list_reversed"] = {
                    {Language::ENGLISH, "Reversed"},
                    {Language::CHINESE, "已反转"}
                };
                strings_["list_size"] = {
                    {Language::ENGLISH, "Size"},
                    {Language::CHINESE, "大小"}
                };

                // 递归相关
                strings_["recursive_base"] = {
                    {Language::ENGLISH, "base case"},
                    {Language::CHINESE, "基准情况"}
                };
                strings_["recursive_cached"] = {
                    {Language::ENGLISH, "cached"},
                    {Language::CHINESE, "已缓存"}
                };
                strings_["recursive_returns"] = {
                    {Language::ENGLISH, "returns"},
                    {Language::CHINESE, "返回"}
                };

                // 通用
                strings_["step"] = {
                    {Language::ENGLISH, "Step"},
                    {Language::CHINESE, "步骤"}
                };
                strings_["initial_array"] = {
                    {Language::ENGLISH, "Initial array"},
                    {Language::CHINESE, "初始数组"}
                };
                strings_["help_title"] = {
                    {Language::ENGLISH, "Interactive Tutor Help"},
                    {Language::CHINESE, "交互式教学帮助"}
                };
                strings_["help_next"] = {
                    {Language::ENGLISH, "Next step"},
                    {Language::CHINESE, "下一步"}
                };
                strings_["help_prev"] = {
                    {Language::ENGLISH, "Previous step"},
                    {Language::CHINESE, "上一步"}
                };
                strings_["help_reset"] = {
                    {Language::ENGLISH, "Reset to beginning"},
                    {Language::CHINESE, "重置到开始"}
                };
                strings_["help_show"] = {
                    {Language::ENGLISH, "Show current state"},
                    {Language::CHINESE, "显示当前状态"}
                };
                strings_["help_help"] = {
                    {Language::ENGLISH, "Show this help"},
                    {Language::CHINESE, "显示此帮助"}
                };
                strings_["help_quit"] = {
                    {Language::ENGLISH, "Exit tutor"},
                    {Language::CHINESE, "退出教学"}
                };
                strings_["help_lang"] = {
                    {Language::ENGLISH, "Switch language (en/zh)"},
                    {Language::CHINESE, "切换语言 (en/zh)"}
                };
            }

        public:
            static void set_language(Language lang) {
                current_language_ = lang;
            }

            static Language get_language() {
                return current_language_;
            }

            static std::string get(const std::string& key) {
                if (strings_.empty()) {
                    init_strings();
                }
                auto it = strings_.find(key);
                if (it != strings_.end()) {
                    auto lang_it = it->second.find(current_language_);
                    if (lang_it != it->second.end()) {
                        return lang_it->second;
                    }
                }
                return key;
            }

            static void toggle_language() {
                if (current_language_ == Language::ENGLISH) {
                    current_language_ = Language::CHINESE;
                }
                else {
                    current_language_ = Language::ENGLISH;
                }
            }

            static void print_language_status() {
                std::cout << "Language: "
                    << (current_language_ == Language::ENGLISH ? "English" : "Chinese")
                    << "\n";
            }
        };

        // 静态成员初始化
        Language LanguageManager::current_language_ = Language::ENGLISH;
        std::map<std::string, std::map<Language, std::string>> LanguageManager::strings_;

        // ============================================================
        // 清屏工具函数
        // ============================================================
        inline void clear_screen() {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
        }

        inline void sleep_ms(int ms) {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

        // ============================================================
        // 1. 可视化辅助工具
        // ============================================================

        struct VisualConfig {
            int bar_width = 40;
            char fill_char = '#';
            char empty_char = '.';
            bool show_index = true;
            bool show_value = true;
            bool color_enabled = false;
        };

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
                if (with_index && config_.show_index) {
                    for (size_t i = 0; i < data_.size(); i++) {
                        std::cout << std::setw(3) << i << "  ";
                    }
                    std::cout << "\n";
                }

                if (with_index && config_.show_index) {
                    for (size_t i = 0; i < data_.size(); i++) {
                        std::cout << "---" << "  ";
                    }
                    std::cout << "\n";
                }

                for (size_t i = 0; i < data_.size(); i++) {
                    bool is_highlight = std::find(highlight_.begin(), highlight_.end(), (int)i) != highlight_.end();
                    bool is_compare = std::find(compare_.begin(), compare_.end(), (int)i) != compare_.end();

                    if (is_highlight) {
                        std::cout << "[" << std::setw(2) << data_[i] << "]";
                    }
                    else if (is_compare) {
                        std::cout << "(" << std::setw(2) << data_[i] << ")";
                    }
                    else {
                        std::cout << " " << std::setw(2) << data_[i] << " ";
                    }
                    std::cout << "  ";
                }
                std::cout << "\n\n";
            }

            void print_bars() const {
                T max_val = *std::max_element(data_.begin(), data_.end());
                if (max_val == 0) max_val = 1;

                for (size_t i = 0; i < data_.size(); i++) {
                    int bar_len = (int)((double)data_[i] / max_val * config_.bar_width);

                    if (config_.show_index) {
                        std::cout << std::setw(3) << i << " ";
                    }

                    for (int j = 0; j < config_.bar_width; j++) {
                        if (j < bar_len) {
                            std::cout << config_.fill_char;
                        }
                        else {
                            std::cout << config_.empty_char;
                        }
                    }

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
            int delay_ms_ = 500;
            bool show_steps_ = true;

            void record_step(const std::string& description = "") {
                std::ostringstream oss;
                oss << LanguageManager::get("step") << " " << ++step_count_ << ": ";
                for (size_t i = 0; i < data_.size(); i++) {
                    oss << data_[i];
                    if (i < data_.size() - 1) oss << " ";
                }
                if (!description.empty()) {
                    oss << "  // " << description;
                }
                steps_.push_back(oss.str());
            }

            void print_state(const std::vector<int>& highlights = {},
                const std::string& message = "") const {
                std::cout << "\n";

                for (size_t i = 0; i < data_.size(); i++) {
                    std::cout << "  " << std::setw(2) << i;
                }
                std::cout << "\n";

                for (size_t i = 0; i < data_.size(); i++) {
                    std::cout << "----";
                }
                std::cout << "\n";

                for (size_t i = 0; i < data_.size(); i++) {
                    bool is_highlight = std::find(highlights.begin(), highlights.end(), (int)i) != highlights.end();
                    if (is_highlight) {
                        std::cout << " [" << std::setw(2) << data_[i] << "]";
                    }
                    else {
                        std::cout << "  " << std::setw(2) << data_[i] << " ";
                    }
                }
                std::cout << "\n";

                if (!message.empty()) {
                    std::cout << "\n  " << message << "\n";
                }
                std::cout << "\n";
            }

            std::string to_string(const T& val) const {
                std::ostringstream oss;
                oss << val;
                return oss.str();
            }

        public:
            SortVisualizer(const std::vector<T>& data) : data_(data) {
                record_step(LanguageManager::get("initial_array"));
            }

            void enable_animation(bool enable = true, int delay_ms = 500) {
                animated_ = enable;
                delay_ms_ = delay_ms;
            }

            void enable_step_logging(bool enable = true) {
                show_steps_ = enable;
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

            // ========== 排序算法 ==========

            void bubble_sort() {
                int n = data_.size();
                bool swapped;
                int pass = 0;

                if (animated_) {
                    clear_screen();
                    print_state({}, LanguageManager::get("sort_start") + " - Bubble Sort");
                    sleep_ms(delay_ms_);
                }

                for (int i = 0; i < n - 1; i++) {
                    swapped = false;
                    pass++;

                    for (int j = 0; j < n - i - 1; j++) {
                        if (animated_) {
                            clear_screen();
                            std::string msg = LanguageManager::get("sort_comparing") + " " +
                                to_string(data_[j]) + " " +
                                LanguageManager::get("sort_and") + " " +
                                to_string(data_[j + 1]) +
                                " (" + LanguageManager::get("sort_pass") + " " +
                                std::to_string(pass) + ")";
                            print_state({ j, j + 1 }, msg);
                            sleep_ms(delay_ms_);
                        }

                        if (data_[j] > data_[j + 1]) {
                            std::swap(data_[j], data_[j + 1]);
                            swapped = true;
                            record_step(LanguageManager::get("sort_swapped") + " " +
                                to_string(data_[j + 1]) + " " +
                                LanguageManager::get("sort_and") + " " +
                                to_string(data_[j]));

                            if (animated_) {
                                clear_screen();
                                std::string msg = LanguageManager::get("sort_swapped") + " " +
                                    to_string(data_[j + 1]) + " " +
                                    LanguageManager::get("sort_and") + " " +
                                    to_string(data_[j]);
                                print_state({ j, j + 1 }, msg);
                                sleep_ms(delay_ms_);
                            }
                        }
                    }

                    if (!swapped) break;
                    record_step(LanguageManager::get("sort_pass") + " " +
                        std::to_string(pass) + " " +
                        LanguageManager::get("sort_pass_completed"));

                    if (animated_) {
                        clear_screen();
                        print_state({}, LanguageManager::get("sort_pass") + " " +
                            std::to_string(pass) + " " +
                            LanguageManager::get("sort_pass_completed"));
                        sleep_ms(delay_ms_ / 2);
                    }
                }

                record_step(LanguageManager::get("sort_sorted") + "!");
                if (animated_) {
                    clear_screen();
                    print_state({}, LanguageManager::get("sort_sorted") + "!");
                }
            }

            void selection_sort() {
                int n = data_.size();

                if (animated_) {
                    clear_screen();
                    print_state({}, LanguageManager::get("sort_start") + " - Selection Sort");
                    sleep_ms(delay_ms_);
                }

                for (int i = 0; i < n - 1; i++) {
                    int min_idx = i;

                    for (int j = i + 1; j < n; j++) {
                        if (animated_) {
                            clear_screen();
                            std::string msg = LanguageManager::get("sort_finding_min") + " " +
                                to_string(data_[j]) + " " +
                                LanguageManager::get("sort_with_current_min") + " " +
                                to_string(data_[min_idx]);
                            print_state({ i, min_idx, j }, msg);
                            sleep_ms(delay_ms_);
                        }

                        if (data_[j] < data_[min_idx]) {
                            min_idx = j;
                            if (animated_) {
                                clear_screen();
                                std::string msg = LanguageManager::get("sort_new_min") + " " +
                                    to_string(data_[min_idx]) + " " +
                                    LanguageManager::get("sort_at_index") + " " +
                                    std::to_string(min_idx);
                                print_state({ i, min_idx, j }, msg);
                                sleep_ms(delay_ms_ / 2);
                            }
                        }
                    }

                    if (min_idx != i) {
                        std::swap(data_[i], data_[min_idx]);
                        record_step(LanguageManager::get("sort_placed") + " " +
                            to_string(data_[i]) + " " +
                            LanguageManager::get("sort_at_position") + " " +
                            std::to_string(i));

                        if (animated_) {
                            clear_screen();
                            std::string msg = LanguageManager::get("sort_placed") + " " +
                                to_string(data_[i]) + " " +
                                LanguageManager::get("sort_at_position") + " " +
                                std::to_string(i);
                            print_state({ i, min_idx }, msg);
                            sleep_ms(delay_ms_);
                        }
                    }
                }

                record_step(LanguageManager::get("sort_sorted") + "!");
                if (animated_) {
                    clear_screen();
                    print_state({}, LanguageManager::get("sort_sorted") + "!");
                }
            }

            void insertion_sort() {
                int n = data_.size();

                if (animated_) {
                    clear_screen();
                    print_state({}, LanguageManager::get("sort_start") + " - Insertion Sort");
                    sleep_ms(delay_ms_);
                }

                for (int i = 1; i < n; i++) {
                    T key = data_[i];
                    int j = i - 1;

                    if (animated_) {
                        clear_screen();
                        std::string msg = LanguageManager::get("sort_inserting") + " " +
                            to_string(key) + " " +
                            LanguageManager::get("sort_into_sorted");
                        print_state({ i, j }, msg);
                        sleep_ms(delay_ms_);
                    }

                    while (j >= 0 && data_[j] > key) {
                        data_[j + 1] = data_[j];

                        if (animated_) {
                            clear_screen();
                            std::string msg = LanguageManager::get("sort_shifting") + " " +
                                to_string(data_[j + 1]) + " " +
                                LanguageManager::get("sort_right");
                            print_state({ j + 1, j }, msg);
                            sleep_ms(delay_ms_ / 2);
                        }
                        j--;
                    }

                    data_[j + 1] = key;
                    record_step(LanguageManager::get("sort_inserted") + " " +
                        to_string(key) + " " +
                        LanguageManager::get("sort_at_position") + " " +
                        std::to_string(j + 1));

                    if (animated_) {
                        clear_screen();
                        std::string msg = LanguageManager::get("sort_inserted") + " " +
                            to_string(key) + " " +
                            LanguageManager::get("sort_at_position") + " " +
                            std::to_string(j + 1);
                        print_state({ j + 1 }, msg);
                        sleep_ms(delay_ms_);
                    }
                }

                record_step(LanguageManager::get("sort_sorted") + "!");
                if (animated_) {
                    clear_screen();
                    print_state({}, LanguageManager::get("sort_sorted") + "!");
                }
            }

            void quick_sort() {
                if (animated_) {
                    clear_screen();
                    print_state({}, LanguageManager::get("sort_start") + " - Quick Sort");
                    sleep_ms(delay_ms_);
                }
                quick_sort_helper(0, data_.size() - 1);
                record_step(LanguageManager::get("sort_sorted") + "!");
                if (animated_) {
                    clear_screen();
                    print_state({}, LanguageManager::get("sort_sorted") + "!");
                }
            }

        private:
            void quick_sort_helper(int low, int high) {
                if (low < high) {
                    int pi = partition(low, high);
                    record_step(LanguageManager::get("sort_pivot") + " " +
                        to_string(data_[pi]) + " " +
                        LanguageManager::get("sort_at_position") + " " +
                        std::to_string(pi));

                    if (animated_) {
                        clear_screen();
                        std::string msg = LanguageManager::get("sort_pivot") + " " +
                            to_string(data_[pi]) + " " +
                            LanguageManager::get("sort_at_position") + " " +
                            std::to_string(pi);
                        print_state({ pi }, msg);
                        sleep_ms(delay_ms_);
                    }

                    quick_sort_helper(low, pi - 1);
                    quick_sort_helper(pi + 1, high);
                }
            }

            int partition(int low, int high) {
                T pivot = data_[high];
                int i = low - 1;

                for (int j = low; j < high; j++) {
                    if (animated_) {
                        clear_screen();
                        std::string msg = LanguageManager::get("sort_partition") + " " +
                            to_string(data_[j]) + " " +
                            LanguageManager::get("sort_with_pivot") + " " +
                            to_string(pivot);
                        print_state({ i + 1, j, high }, msg);
                        sleep_ms(delay_ms_ / 2);
                    }

                    if (data_[j] < pivot) {
                        i++;
                        std::swap(data_[i], data_[j]);

                        if (animated_) {
                            clear_screen();
                            std::string msg = LanguageManager::get("sort_swapped") + " " +
                                to_string(data_[i]) + " " +
                                LanguageManager::get("sort_and") + " " +
                                to_string(data_[j]);
                            print_state({ i, j }, msg);
                            sleep_ms(delay_ms_ / 2);
                        }
                    }
                }

                std::swap(data_[i + 1], data_[high]);
                return i + 1;
            }
        };

        // ============================================================
        // 3. 数据结构可视化
        // ============================================================

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
            bool animated_ = false;
            int delay_ms_ = 500;

        public:
            void enable_animation(bool enable = true, int delay_ms = 500) {
                animated_ = enable;
                delay_ms_ = delay_ms;
            }

            void push_front(const T& data) {
                auto node = std::make_shared<Node>(data);
                node->next = head_;
                head_ = node;
                size_++;

                if (animated_) {
                    clear_screen();
                    print();
                    std::cout << "-> push_front(" << data << ")\n";
                    sleep_ms(delay_ms_);
                }
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

                if (animated_) {
                    clear_screen();
                    print();
                    std::cout << "-> push_back(" << data << ")\n";
                    sleep_ms(delay_ms_);
                }
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
                std::cout << LanguageManager::get("list_size") << ": " << size_ << "\n\n";
            }

            void reverse_with_visualization() {
                std::cout << "\n=== " << LanguageManager::get("list_reversing") << " ===\n";
                std::shared_ptr<Node> prev = nullptr;
                auto current = head_;
                auto next = head_;

                while (current) {
                    next = current->next;
                    current->next = prev;

                    if (animated_) {
                        clear_screen();
                        std::cout << "Step: ";
                        print_current_state(prev, current, next);
                        std::cout << "\n\n";
                        print();
                        sleep_ms(delay_ms_);
                    }

                    prev = current;
                    current = next;
                }
                head_ = prev;
                std::cout << "\n" << LanguageManager::get("list_reversed") << "!\n\n";
                print();
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
            bool animated_ = false;
            int delay_ms_ = 300;

            void print_indent() const {
                for (int i = 0; i < depth_; i++) {
                    std::cout << "  ";
                }
            }

        public:
            void enable(bool enable = true) {
                enabled_ = enable;
            }

            void enable_animation(bool enable = true, int delay_ms = 300) {
                animated_ = enable;
                delay_ms_ = delay_ms;
            }

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

                if (animated_) {
                    sleep_ms(delay_ms_);
                }

                depth_++;
                auto result = func(std::forward<Args>(args)...);
                depth_--;

                print_indent();
                std::cout << "<- " << name << " "
                    << LanguageManager::get("recursive_returns") << " "
                    << result << "\n";

                if (animated_) {
                    sleep_ms(delay_ms_);
                }

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

        class FibonacciVisualizer {
        private:
            std::map<int, int> memo_;
            RecursionVisualizer tracer_;
            bool animated_ = false;
            int delay_ms_ = 300;

        public:
            void enable_animation(bool enable = true, int delay_ms = 300) {
                animated_ = enable;
                delay_ms_ = delay_ms;
                tracer_.enable_animation(enable, delay_ms);
            }

            int fib_recursive(int n) {
                if (n <= 1) {
                    if (animated_) {
                        std::cout << "fib(" << n << ") = " << n
                            << " (" << LanguageManager::get("recursive_base") << ")\n";
                        sleep_ms(delay_ms_);
                    }
                    return n;
                }

                if (animated_) {
                    clear_screen();
                    std::cout << "fib(" << n << ") = fib(" << n - 1
                        << ") + fib(" << n - 2 << ")\n";
                    sleep_ms(delay_ms_);
                }

                return fib_recursive(n - 1) + fib_recursive(n - 2);
            }

            int fib_memo(int n) {
                if (n <= 1) {
                    if (animated_) {
                        std::cout << "fib(" << n << ") = " << n
                            << " (" << LanguageManager::get("recursive_base") << ")\n";
                        sleep_ms(delay_ms_);
                    }
                    return n;
                }

                if (memo_.find(n) != memo_.end()) {
                    if (animated_) {
                        std::cout << "fib(" << n << ") = " << memo_[n]
                            << " (" << LanguageManager::get("recursive_cached") << ")\n";
                        sleep_ms(delay_ms_);
                    }
                    return memo_[n];
                }

                if (animated_) {
                    clear_screen();
                    std::cout << "fib(" << n << ") = fib(" << n - 1
                        << ") + fib(" << n - 2 << ")\n";
                    sleep_ms(delay_ms_);
                }

                memo_[n] = fib_memo(n - 1) + fib_memo(n - 2);
                return memo_[n];
            }

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
            bool animated_ = false;
            int delay_ms_ = 300;

        public:
            void enable_verbose(bool enable = true) {
                verbose_ = enable;
            }

            void enable_animation(bool enable = true, int delay_ms = 300) {
                animated_ = enable;
                delay_ms_ = delay_ms;
            }

            void step(const std::string& description, bool auto_log = true) {
                auto now = std::chrono::high_resolution_clock::now();
                static auto start = now;

                auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
                steps_.push_back(description);
                timings_.push_back(elapsed);

                if (verbose_ && auto_log) {
                    std::cout << "[" << LanguageManager::get("step") << " "
                        << steps_.size() << "] " << description
                        << "  (+" << elapsed.count() << "us)\n";

                    if (animated_) {
                        sleep_ms(delay_ms_);
                    }
                }
            }

            void print_report() const {
                std::cout << "\n=== Step Report ===\n";
                for (size_t i = 0; i < steps_.size(); i++) {
                    std::cout << "  " << LanguageManager::get("step") << " "
                        << std::setw(2) << i + 1 << ": " << steps_[i];
                    if (i < timings_.size()) {
                        std::cout << "  (" << timings_[i].count() << " us)";
                    }
                    std::cout << "\n";
                }
                if (!timings_.empty()) {
                    auto total = timings_.back().count() - timings_.front().count();
                    std::cout << "  Total: " << total << " us\n";
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
            bool animated_ = false;
            int delay_ms_ = 500;

        public:
            BinarySearchVisualizer(const std::vector<T>& data) : data_(data) {
                std::sort(data_.begin(), data_.end());
            }

            void enable_animation(bool enable = true, int delay_ms = 500) {
                animated_ = enable;
                delay_ms_ = delay_ms;
            }

            int search(const T& target, bool visualize = true) {
                int left = 0, right = data_.size() - 1;
                int steps = 0;

                if (animated_) {
                    clear_screen();
                }
                std::cout << LanguageManager::get("search_start") << " " << target
                    << " " << LanguageManager::get("search_in_sorted") << ":\n";
                print_array(left, right, -1);

                while (left <= right) {
                    int mid = left + (right - left) / 2;
                    steps++;

                    if (animated_) {
                        clear_screen();
                    }
                    std::cout << "\n" << LanguageManager::get("step") << " " << steps << ": ";
                    std::cout << "left=" << left << ", right=" << right << ", mid=" << mid;
                    std::cout << ", data[mid]=" << data_[mid] << "\n";

                    print_array(left, right, mid);

                    if (data_[mid] == target) {
                        std::cout << "\n" << LanguageManager::get("search_found")
                            << " " << mid << "!\n";
                        return mid;
                    }

                    if (data_[mid] < target) {
                        std::cout << "  " << data_[mid] << " < " << target
                            << ", " << LanguageManager::get("search_right_half")
                            << " ->\n";
                        left = mid + 1;
                    }
                    else {
                        std::cout << "  " << data_[mid] << " > " << target
                            << ", " << LanguageManager::get("search_left_half")
                            << " <-\n";
                        right = mid - 1;
                    }

                    if (animated_) {
                        sleep_ms(delay_ms_);
                    }
                }

                std::cout << "\n" << LanguageManager::get("search_not_found") << "!\n";
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
                    std::cout << std::setw(2) << data_[i];
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
        // 8. 数据结构操作
        // ============================================================

        template<typename T>
        class StackDemo {
        private:
            std::stack<T> stack_;
            bool animated_ = false;
            int delay_ms_ = 400;

        public:
            void enable_animation(bool enable = true, int delay_ms = 400) {
                animated_ = enable;
                delay_ms_ = delay_ms;
            }

            void push(const T& value) {
                if (animated_) clear_screen();
                std::cout << "push(" << value << ") -> ";
                stack_.push(value);
                print();
                if (animated_) sleep_ms(delay_ms_);
            }

            void pop() {
                if (stack_.empty()) {
                    std::cout << "pop() -> " << LanguageManager::get("stack_empty") << "!\n";
                    return;
                }
                T value = stack_.top();
                stack_.pop();
                if (animated_) clear_screen();
                std::cout << "pop() -> " << value << " -> ";
                print();
                if (animated_) sleep_ms(delay_ms_);
            }

            void top() const {
                if (stack_.empty()) {
                    std::cout << "top() -> " << LanguageManager::get("stack_empty") << "!\n";
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
                std::cout << "] (" << LanguageManager::get("stack_top") << "=";
                if (!stack_.empty()) std::cout << stack_.top();
                else std::cout << LanguageManager::get("stack_empty");
                std::cout << ")\n";
            }
        };

        template<typename T>
        class QueueDemo {
        private:
            std::queue<T> queue_;
            bool animated_ = false;
            int delay_ms_ = 400;

        public:
            void enable_animation(bool enable = true, int delay_ms = 400) {
                animated_ = enable;
                delay_ms_ = delay_ms;
            }

            void enqueue(const T& value) {
                if (animated_) clear_screen();
                std::cout << "enqueue(" << value << ") -> ";
                queue_.push(value);
                print();
                if (animated_) sleep_ms(delay_ms_);
            }

            void dequeue() {
                if (queue_.empty()) {
                    std::cout << "dequeue() -> " << LanguageManager::get("stack_empty") << "!\n";
                    return;
                }
                T value = queue_.front();
                queue_.pop();
                if (animated_) clear_screen();
                std::cout << "dequeue() -> " << value << " -> ";
                print();
                if (animated_) sleep_ms(delay_ms_);
            }

            void front() const {
                if (queue_.empty()) {
                    std::cout << "front() -> " << LanguageManager::get("stack_empty") << "!\n";
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
                std::cout << "] (" << LanguageManager::get("queue_front") << "=";
                if (!queue_.empty()) std::cout << queue_.front();
                else std::cout << LanguageManager::get("stack_empty");
                std::cout << ", " << LanguageManager::get("queue_back") << "=";
                if (!queue_.empty()) std::cout << queue_.back();
                else std::cout << LanguageManager::get("stack_empty");
                std::cout << ")\n";
            }
        };

        // ============================================================
        // 9. 复杂度分析
        // ============================================================
        // 特别注意，复杂度通常不可判定，这只是估算
        class ComplexityAnalyzer {
        private:
            std::map<std::string, std::vector<double>> measurements_;
            bool verbose_ = true;

        public:
            void enable_verbose(bool enable = true) {
                verbose_ = enable;
            }

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

                std::sort(times.begin(), times.end());
                double sum = 0;
                for (int i = 1; i < repetitions - 1; i++) {
                    sum += times[i];
                }
                double avg = sum / (repetitions - 2);

                measurements_[name].push_back(avg);

                if (verbose_) {
                    std::cout << "[Measure] " << name << ": " << avg << " us\n";
                }

                return avg;
            }

            void print_report() const {
                std::cout << "\n=== Complexity Analysis Report ===\n";
                for (const auto& [name, times] : measurements_) {
                    std::cout << name << ":\n";
                    for (size_t i = 0; i < times.size(); i++) {
                        std::cout << "  n=" << (i + 1) * 100 << ": "
                            << times[i] << " us\n";
                    }
                }
                std::cout << "====================================\n\n";
            }

            void clear() {
                measurements_.clear();
            }
        };

        // ============================================================
        // 10. algo比较器
        // ============================================================

        class AlgorithmComparator {
        private:
            std::map<std::string, std::function<void()>> algorithms_;
            std::vector<int> test_sizes_;
            bool verbose_ = true;

        public:
            void enable_verbose(bool enable = true) {
                verbose_ = enable;
            }

            void add_algorithm(const std::string& name, std::function<void()> algo) {
                algorithms_[name] = algo;
            }

            void test_sizes(const std::vector<int>& sizes) {
                test_sizes_ = sizes;
            }

            void compare(int repetitions = 3) {
                ComplexityAnalyzer analyzer;
                analyzer.enable_verbose(verbose_);

                std::cout << "\n=== Algorithm Comparison ===\n";
                for (const auto& [name, algo] : algorithms_) {
                    std::cout << "\n[Testing] " << name << "\n";
                    for (int size : test_sizes_) {
                        auto test_func = [algo, size]() {
                            algo();
                            };
                        double avg = analyzer.measure(name + "_n=" + std::to_string(size),
                            test_func, repetitions);
                        if (verbose_) {
                            std::cout << "  n=" << size << ": " << avg << " us\n";
                        }
                    }
                }
                std::cout << "============================\n\n";
            }
        };

        // ============================================================
        // 11. 交互式教学工具 (震惊)
        // ============================================================

        class InteractiveTutor {
        private:
            std::vector<std::string> history_;
            int current_step_ = 0;
            bool animated_ = false;

            void print_help_internal() const {
                std::cout << "\n";
                std::cout << "=== " << LanguageManager::get("help_title") << " ===\n";
                std::cout << "  next    - " << LanguageManager::get("help_next") << "\n";
                std::cout << "  prev    - " << LanguageManager::get("help_prev") << "\n";
                std::cout << "  reset   - " << LanguageManager::get("help_reset") << "\n";
                std::cout << "  show    - " << LanguageManager::get("help_show") << "\n";
                std::cout << "  help    - " << LanguageManager::get("help_help") << "\n";
                std::cout << "  lang    - " << LanguageManager::get("help_lang") << "\n";
                std::cout << "  quit    - " << LanguageManager::get("help_quit") << "\n";
                std::cout << "=========================\n\n";
            }

        public:
            void enable_animation(bool enable = true) {
                animated_ = enable;
            }

            void print_help() const {
                print_help_internal();
            }

            void record_state(const std::string& state) {
                history_.push_back(state);
                current_step_ = history_.size() - 1;
                if (animated_) {
                    clear_screen();
                    show_current();
                    sleep_ms(300);
                }
            }

            void next() {
                if (current_step_ < (int)history_.size() - 1) {
                    current_step_++;
                    if (animated_) clear_screen();
                    show_current();
                }
                else {
                    std::cout << "Already at the end.\n";
                }
            }

            void prev() {
                if (current_step_ > 0) {
                    current_step_--;
                    if (animated_) clear_screen();
                    show_current();
                }
                else {
                    std::cout << "Already at the beginning.\n";
                }
            }

            void reset() {
                current_step_ = 0;
                if (animated_) clear_screen();
                show_current();
            }

			//显示当前步骤
            void show_current() const {
                if (current_step_ < (int)history_.size()) {
                    std::cout << LanguageManager::get("step") << " " << current_step_
                        << ": " << history_[current_step_] << "\n";
                }
            }

            //能跑啦
            void run() {
                std::string command;
                print_help_internal();
                while (true) {
                    std::cout << "> ";
                    std::getline(std::cin, command);

                    if (command == "next") next();
                    else if (command == "prev") prev();
                    else if (command == "reset") reset();
                    else if (command == "show") show_current();
                    else if (command == "help") print_help_internal();
                    else if (command == "lang" || command == "en" || command == "zh") {
                        if (command == "en") {
                            LanguageManager::set_language(Language::ENGLISH);
                        }
                        else if (command == "zh") {
                            LanguageManager::set_language(Language::CHINESE);
                        }
                        else {
                            LanguageManager::toggle_language();
                        }
                        LanguageManager::print_language_status();
                    }
                    else if (command == "quit") break;
                    else std::cout << "Unknown command. Type 'help' for options.\n";
                }
            }
        };

        // ============================================================
        // 12. 辅助函数
        // ============================================================

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

        //国际惯例，为了解耦，用独立的函数
        template<typename T>
        std::vector<T> generate_teaching_array_seeded(int size, unsigned int seed, bool sorted = false) {
            std::vector<T> result(size);
            std::mt19937 gen(seed);
            std::uniform_int_distribution<T> dis(0, 100);

            for (int i = 0; i < size; i++) {
                result[i] = dis(gen);
            }

            if (sorted) {
                std::sort(result.begin(), result.end());
            }

            return result;
        }

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
                std::cout << std::setw(2) << arr[i];
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

        template<typename T>
        void print_2d_array(const std::vector<std::vector<T>>& arr) {
            for (const auto& row : arr) {
                for (const auto& val : row) {
                    std::cout << std::setw(4) << val;
                }
                std::cout << "\n";
            }
        }

        inline void print_separator(char ch = '=', int width = 50) {
            std::cout << std::string(width, ch) << "\n";
        }

        inline void print_title(const std::string& title, char ch = '=') {
            print_separator(ch);
            std::cout << "  " << title << "\n";
            print_separator(ch);
        }

        // ============================================================
        // 语言切换 (爷爷你follow的codej终于支持双语了)
        // ============================================================
        inline void set_language_english() {
            LanguageManager::set_language(Language::ENGLISH);
        }

        inline void set_language_chinese() {
            LanguageManager::set_language(Language::CHINESE);
        }

        inline void toggle_language() {
            LanguageManager::toggle_language();
        }

        inline void print_language() {
            LanguageManager::print_language_status();
        }

    } // namespace teaching

#ifdef LAZY_EXPORT_ALL
    using namespace teaching;
#endif
} // namespace lazy