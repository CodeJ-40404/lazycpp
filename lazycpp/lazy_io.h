// lazy_io.h
// copyright 2026 ? lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <climits>
#include <vector>
#include <iomanip>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

namespace lazy {
    namespace io {

        // ============================================================
        // 快速 IO
        // ============================================================

        inline void fast_io() {
            std::ios::sync_with_stdio(false);
            std::cin.tie(nullptr);
            std::cout.tie(nullptr);
        }

        // ============================================================
        // 文件读写
        // ============================================================

        // 读取整个文件到字符串
        inline std::string read_file(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file: " + filename);
            }
            std::ostringstream oss;
            oss << file.rdbuf();
            return oss.str();
        }

        // 读取文件所有行
        inline std::vector<std::string> read_lines(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file: " + filename);
            }
            std::vector<std::string> lines;
            std::string line;
            while (std::getline(file, line)) {
                lines.push_back(line);
            }
            return lines;
        }

        // 写入文件
        inline void write_file(const std::string& filename, const std::string& content) {
            std::ofstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file for writing: " + filename);
            }
            file << content;
        }

        // 追加到文件
        inline void append_file(const std::string& filename, const std::string& content) {
            std::ofstream file(filename, std::ios::app);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file for appending: " + filename);
            }
            file << content;
        }

        // ============================================================
        // 彩色输出
        // ============================================================

        // ANSI 颜色码（适用于 Linux/macOS）
        namespace color {
            const std::string RESET = "\033[0m";
            const std::string BLACK = "\033[30m";
            const std::string RED = "\033[31m";
            const std::string GREEN = "\033[32m";
            const std::string YELLOW = "\033[33m";
            const std::string BLUE = "\033[34m";
            const std::string MAGENTA = "\033[35m";
            const std::string CYAN = "\033[36m";
            const std::string WHITE = "\033[37m";
            const std::string BOLD = "\033[1m";
            const std::string DIM = "\033[2m";
            const std::string UNDERLINE = "\033[4m";
        }

        // Windows 控制台颜色
#ifdef _WIN32
        inline void set_console_color(int color) {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h, color);
        }

        // Windows 颜色常量
        enum ConsoleColor {
            BLACK = 0,
            DARK_BLUE = 1,
            DARK_GREEN = 2,
            DARK_CYAN = 3,
            DARK_RED = 4,
            DARK_PURPLE = 5,
            DARK_YELLOW = 6,
            GRAY = 7,
            BLUE = 9,
            GREEN = 10,
            CYAN = 11,
            RED = 12,
            PURPLE = 13,
            YELLOW = 14,
            WHITE = 15
        };

        // 彩色输出（Windows）
        template<typename... Args>
        void print_color(int color, Args&&... args) {
            set_console_color(color);
            ((std::cout << std::forward<Args>(args)), ...);
            set_console_color(7); // 重置为白色
        }

        template<typename... Args>
        void println_color(int color, Args&&... args) {
            print_color(color, std::forward<Args>(args)...);
            std::cout << '\n';
        }
#else
    // 彩色输出（Linux/macOS）
        template<typename... Args>
        void print_color(const std::string& color_code, Args&&... args) {
            std::cout << color_code;
            ((std::cout << std::forward<Args>(args)), ...);
            std::cout << color::RESET;
        }

        template<typename... Args>
        void println_color(const std::string& color_code, Args&&... args) {
            print_color(color_code, std::forward<Args>(args)...);
            std::cout << '\n';
        }
#endif

        // ============================================================
        // 格式化输出
        // ============================================================

        // 带颜色的日志输出
        template<typename... Args>
        void log_info(Args&&... args) {
#ifdef _WIN32
            println_color(11, "[INFO] ", std::forward<Args>(args)...);
#else
            println_color(color::CYAN, "[INFO] ", std::forward<Args>(args)...);
#endif
        }

        template<typename... Args>
        void log_warn(Args&&... args) {
#ifdef _WIN32
            println_color(14, "[WARN] ", std::forward<Args>(args)...);
#else
            println_color(color::YELLOW, "[WARN] ", std::forward<Args>(args)...);
#endif
        }

        template<typename... Args>
        void log_error(Args&&... args) {
#ifdef _WIN32
            println_color(12, "[ERROR] ", std::forward<Args>(args)...);
#else
            println_color(color::RED, "[ERROR] ", std::forward<Args>(args)...);
#endif
        }

        template<typename... Args>
        void log_success(Args&&... args) {
#ifdef _WIN32
            println_color(10, "[OK] ", std::forward<Args>(args)...);
#else
            println_color(color::GREEN, "[OK] ", std::forward<Args>(args)...);
#endif
        }

        // ============================================================
        // 进度条
        // ============================================================

        class ProgressBar {
        private:
            int total;
            int width;
            char fill_char;
            char empty_char;
            bool show_percentage;

        public:
            ProgressBar(int total, int width = 50, char fill = '=', char empty = ' ',
                bool show_percent = true)
                : total(total), width(width), fill_char(fill),
                empty_char(empty), show_percentage(show_percent) {
            }

            void update(int current) {
                float progress = (float)current / total;
                int filled = (int)(progress * width);

                std::cout << "\r[";
                for (int i = 0; i < width; i++) {
                    if (i < filled) std::cout << fill_char;
                    else if (i == filled) std::cout << '>';
                    else std::cout << empty_char;
                }
                std::cout << "]";

                if (show_percentage) {
                    std::cout << " " << std::setw(3) << (int)(progress * 100) << "%";
                }
                std::cout << std::flush;

                if (current >= total) {
                    std::cout << "\n";
                }
            }
        };

        // ============================================================
        // 分隔线
        // ============================================================

        inline void separator(char c = '-', int width = 80) {
            std::cout << std::string(width, c) << '\n';
        }

        inline void title(const std::string& text, char c = '=', int width = 80) {
            int padding = (width - text.size() - 2) / 2;
            if (padding < 0) padding = 0;
            std::cout << std::string(padding, c) << ' ' << text << ' '
                << std::string(width - padding - text.size() - 2, c) << '\n';
        }

        // ============================================================
        // 安全输入
        // ============================================================

        template<typename T>
        T input(const std::string& prompt = "") {
            if (!prompt.empty()) {
                std::cout << prompt;
            }
            T val;
            while (!(std::cin >> val)) {
                std::cin.clear();
                std::cin.ignore(9223372036854775807i64, '\n');
                std::cout << "Invalid input. Please try again: ";
            }
            return val;
        }

        inline std::string input_line(const std::string& prompt = "") {
            if (!prompt.empty()) {
                std::cout << prompt;
            }
            std::string line;
            std::getline(std::cin, line);
            return line;
        }

    } // namespace io

    //using namespace io;
    //不规范
#ifdef LAZY_EXPORT_ALL
    using namespace io;
#endif
} // namespace lazy