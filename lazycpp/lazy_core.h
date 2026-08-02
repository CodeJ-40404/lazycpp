// lazy_core.h
// copyright 2026 ? lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <deque>
#include <list>
#include <stack>
#include <utility>
#include <functional>
#include <iterator>
#include <numeric>
#include <initializer_list>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstring>
#include <chrono>
#include <thread>
#include <future>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <type_traits>
#include <limits>
#include <cassert>
#include <exception>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

namespace lazy {
    namespace core {

        // ============================================================
        // 类型别名
        // ============================================================
        using str = std::string;
        using ll = long long;
        using ull = unsigned long long;
        using cr = char;
        using db = double;
        using ld = long double;

        using vi = std::vector<int>;
        using vvi = std::vector<std::vector<int>>;
        using vll = std::vector<long long>;
        using vull = std::vector<unsigned long long>;
        using vstr = std::vector<std::string>;

        using pii = std::pair<int, int>;
        using pll = std::pair<long long, long long>;
        using pstr = std::pair<std::string, std::string>;

        using mii = std::map<int, int>;
        using mll = std::map<long long, long long>;
        using msi = std::map<std::string, int>;

        using umii = std::unordered_map<int, int>;
        using umll = std::unordered_map<long long, long long>;
        using umsi = std::unordered_map<std::string, int>;

        using si = std::set<int>;
        using sll = std::set<long long>;
        using sstr = std::set<std::string>;

        using usi = std::unordered_set<int>;
        using usll = std::unordered_set<long long>;
        using usstr = std::unordered_set<std::string>;

        using qui = std::queue<int>;
        using qul = std::queue<long long>;
        using pqi = std::priority_queue<int>;
        using pql = std::priority_queue<long long>;

        using ti = std::tuple<int, int, int>;
        using tll = std::tuple<long long, long long, long long>;

#ifdef __GNUC__
        using i128 = __int128;
#endif

#ifdef __clang__
        using i128 = __int128;
#endif

        // ============================================================
        // 常量
        // ============================================================
        const int INF = 1e9;
        const ll LINF = (ll)4e18;
        const db PI = 3.14159265358979323846;
        const db E = 2.71828182845904523536;
        const db EPS = 1e-9;
        const int MOD = 1000000007;
        const int MOD9 = 998244353;
        const int MAXN = 1000005;

        // ============================================================
        // 输入输出
        // ============================================================

        template<typename... Args>
        void read(Args&&... args) {
            ((std::cin >> std::forward<Args>(args)), ...);
        }

        template<typename... Args>
        void out(Args&&... args) {
            ((std::cout << std::forward<Args>(args)), ...);
        }

        template<typename... Args>
        void outl(Args&&... args) {
            ((std::cout << std::forward<Args>(args)), ...);
            std::cout << '\n';
        }

        // ============================================================
        // 循环宏
        // ============================================================

#define rep(i, n) for(int i = 0; i < (n); i++)
#define rep1(i, n) for(int i = 1; i <= (n); i++)
#define rrep(i, n) for(int i = (n)-1; i >= 0; i--)
#define each(x, arr) for(auto& x : arr)
#define repeat(times) for(int _ = 0; _ < (times); _++)

#define all(arr) (arr).begin(), (arr).end()
#define rall(arr) (arr).rbegin(), (arr).rend()
#define sz(arr) ((int)(arr).size())
#define fi first
#define se second

// ============================================================
// 计时器
// ============================================================

        class Timer {
            std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
            std::chrono::time_point<std::chrono::high_resolution_clock> pause_time;
            std::string name;
            bool paused = false;
            long long elapsed_us = 0;

        public:
            Timer(const std::string& n = "") : name(n) {
                start_time = std::chrono::high_resolution_clock::now();
            }

            void pause() {
                if (!paused) {
                    auto now = std::chrono::high_resolution_clock::now();
                    elapsed_us += std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
                    paused = true;
                }
            }

            void resume() {
                if (paused) {
                    start_time = std::chrono::high_resolution_clock::now();
                    paused = false;
                }
            }

            long long elapsed() const {
                if (paused) return elapsed_us;
                auto now = std::chrono::high_resolution_clock::now();
                return elapsed_us + std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
            }

            void reset() {
                start_time = std::chrono::high_resolution_clock::now();
                elapsed_us = 0;
                paused = false;
            }

            ~Timer() {
                if (!name.empty()) std::cout << name << ": ";
                std::cout << elapsed() << " microseconds\n";
            }
        };

        template<typename F>
        auto timeit(F&& f, const std::string& name = "") {
            Timer t(name);
            return f();
        }

        // ============================================================
        // 文件操作
        // ============================================================

        inline void set_input(const std::string& filename) {
            if (!freopen(filename.c_str(), "r", stdin)) {
                std::cerr << "Failed to open " << filename << " for reading\n";
            }
        }

        inline void set_output(const std::string& filename) {
            if (!freopen(filename.c_str(), "w", stdout)) {
                std::cerr << "Failed to open " << filename << " for writing\n";
            }
        }

        inline void set_io(const std::string& in, const std::string& out) {
            set_input(in);
            set_output(out);
        }

        // ============================================================
        // 便捷函数
        // ============================================================

        inline void wait(int ms) {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

        inline void fast_IO() {
            std::ios::sync_with_stdio(false);
            std::cin.tie(nullptr);
            std::cout.tie(nullptr);
        }

    } // namespace core

    // 将 core 中的内容提升到 lazy 命名空间
    //using namespace core;
#ifdef LAZY_EXPORT_ALL
    using namespace core;
#endif
} // namespace lazy