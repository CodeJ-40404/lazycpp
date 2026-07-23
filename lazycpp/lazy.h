// lazy.h 
#define _CRT_SECURE_NO_WARNINGS
#pragma once

// ========== 基础 ==========
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

// ========== 容器 ==========
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <deque>
#include <list>
#include <stack>

// ========== 工具 ==========
#include <utility>
#include <functional>
#include <iterator>
#include <numeric>
#include <initializer_list>

// ========== 字符串/流 ==========
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstring>

// ========== 时间/随机 ==========
#include <chrono>
#include <random>
#include <thread>
#include <future>

// ========== 文件 ==========
#include <fstream>
#include <cstdio>
#include <filesystem>   // C++17，注意版本

#ifdef _WIN32
// ========== 平台相关 (Windows) ==========
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

// ========== 类型支持 ==========
#include <cstdint>
#include <climits>
#include <cfloat>
#include <type_traits>
#include <limits>

// ========== 调试/异常 ==========
#include <cassert>
#include <exception>
#include <stdexcept>

//禁止在非竞赛环境下使用
#define us using namespace
//非常推荐
#define fio std::ios::sync_with_stdio(false);std::cin.tie(nullptr);std::cout.tie(nullptr)

//严禁在MSVC环境下使用
#define incb #include<bits/stdc++.h>



inline void setColor(int color) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, color);
#endif
}

//0 = 黑色 8 = 灰色
//1 = 蓝色 9 = 淡蓝色
//2 = 绿色 10 = 淡绿色  cyan
//3 = 浅绿色 11 = 淡浅绿色   l_cyan
//4 = 红色 12 = 淡红色
//5 = 紫色 13 = 淡紫色
//6 = 黄色 14 = 淡黄色
//7 = 白色 15 = 亮白色
//一般使用的时候使用无前缀颜色

#define COLOR_BLACK 0
#define COLOR_DARK_BLUE 1
#define COLOR_DARK_GREEN 2
#define COLOR_DARK_CYAN 3  
#define COLOR_DARK_RED 4
#define COLOR_DARK_PURPLE 5
#define COLOR_DARK_YELLOW 6
#define COLOR_RESET 7
#define COLOR_GREY 8
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_CYAN 11  
#define COLOR_RED 12
#define COLOR_PURPLE 13
#define COLOR_YELLOW 14
#define COLOR_LIGHT_WHITE 15



//正
#define rep(i, n) for(int i = 0; i < (n); i++)
//自定义，注意是<n
#define repn(i,a,n,b) for(int i = (a); i < (n); i+=(b))
//反
#define rrep(i, n) for(int i = (n)-1; i >= 0; i--)
//迭代器
#define each(x, arr) for(auto& x : arr)
//重复
#define repeat(times) for(int _ = 0; _ < (times); _++)


// ========== 1. 命名空间隔离 ==========
namespace lazy {

    // ========== 2. 类型别名 ==========
    // 这里可能影响可读性，所以不太建议使用
    // 当然你在合理使用的前提下这个特别特别方便
    // 注意命名污染问题
    using str = std::string;
    using ll = long long;
    using cr = char;
    using ull = unsigned long long;
    using vi = std::vector<int>;
    using vvi = std::vector<vi>;
    using vll = std::vector<long long>;
    using pii = std::pair<int, int>;
    using pll = std::pair<long long, long long>;
    using mii = std::map<int, int>;
    using umii = std::unordered_map<int, int>;
    using usi = std::unordered_set<int>;
    using mll = std::map<long long, long long>;
    using umll = std::unordered_map<long long, long long>;
    using usll = std::unordered_set<long long>;
    using si = std::set<int>;
    using sll = std::set<long long>;
    using qui = std::queue<int>;
    using pqi = std::priority_queue<int>;
    using qul = std::queue<long long>;
    using pql = std::priority_queue<long long>;
    using ti = std::tuple<int, int, int>;
#ifdef __GNUC__
    using i128 = __int128
#endif

#ifdef __clang__
        using i128 = __int128
#endif

        // ========== 3. 输入输出 ==========
        // 基础输入
        template<typename... Args>
    void read(Args&&... args) {
        ((std::cin >> std::forward<Args>(args)), ...);
    }


    // 基础输出（无换行，无空格）
    template<typename... Args>
    void out(Args&&... args) {
        ((std::cout << std::forward<Args>(args)), ...);
    }

    // 输出并换行
    template<typename... Args>
    void outl(Args&&... args) {
        ((std::cout << std::forward<Args>(args)), ...);
        std::cout << '\n';
    }

    // 警告，不要在非MSVC使用
    // 错误输出（自动空格分隔，带换行）
    template<typename... Args>
    void err(Args&&... args) {
        setColor(COLOR_RED);
        ((std::cerr << args << ' '), ...);
        std::cerr << '\n';
        setColor(COLOR_RESET);
    }

    // 警告，不要在非MSVC使用
    // 正确输出（自动空格分隔，带换行）
    template<typename... Args>
    void correct(Args&&... args) {
        setColor(COLOR_GREEN);
        ((std::cout << args << ' '), ...);
        std::cout << '\n';
        setColor(COLOR_RESET);
    }

    // 警告，不要在非MSVC使用
    //info输出（自动空格分隔，带换行）
    template<typename... Args>
    void info(Args&&... args) {
        setColor(COLOR_CYAN);
        ((std::cout << args << ' '), ...);
        std::cout << '\n';
        setColor(COLOR_RESET);
    }

    // 警告，不要在非MSVC使用
    //警告输出（自动空格分隔，带换行）
    template<typename... Args>
    void warn(Args&&... args) {
        setColor(COLOR_YELLOW);
        ((std::cout << args << ' '), ...);
        std::cout << '\n';
        setColor(COLOR_RESET);
    }

    // ========== 4. 调试 ==========
#ifdef DEBUG
#define dbg(x) std::cerr << "[DEBUG] " << #x << " = " << x << '\n'
#define dbg_arr(arr, n) do { \
    std::cerr << "[DEBUG] " << #arr << " = ["; \
    for(int _i = 0; _i < n; _i++) std::cerr << arr[_i] << ",\n"[_i==n-1]; \
} while(0)
#else
#define dbg(x)
#define dbg_arr(arr, n)
#endif

// ========== 5. 循环辅助（用lambda替代宏） ==========
    //上面的是可以使用的，下面的也可以  
    //但是比较麻烦
    template<typename F>
    inline void lrep(int n, F&& f) {
        for (int i = 0; i < n; ++i) f(i);
    }

    template<typename F>
    inline void lrep1(int n, F&& f) {
        for (int i = 1; i <= n; ++i) f(i);
    }

    template<typename F>
    inline void lrrep(int n, F&& f) {
        for (int i = n - 1; i >= 0; --i) f(i);
    }

    template<typename Container, typename F>
    inline void leach(Container& c, F&& f) {
        for (auto& x : c) f(x);
    }

    // ========== 6. 数学函数（修正bug） ==========
    template<typename T>
    T gcd(T a, T b) {
        a = std::abs(a);
        b = std::abs(b);
        return b ? gcd(b, a % b) : a;
    }

    template<typename T>
    T lcm(T a, T b) {
        return a / gcd(a, b) * b;
    }

    // 常量
    const int INF = 1e9;
    const long long LINF = 4e18;
    const double PI = 3.14159265358979323846;
    const int MOD = 1000000007;
    const int MOD9 = 998244353;
    const int MAXN = 1000005;
    const double E = 2.71828182845904523536;



    //判断素数
    // 快速幂取模,其实这个函数也可以用来计算普通的幂
    ull mod_pow(ull base, ull exp, ull mod) {
        ull result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) {
                result = (ull)result * base % mod;
            }
            base = (ull)base * base % mod;
            exp >>= 1;
        }
        return result;
    }

    // 米勒拉宾
    bool miller_rabin(ull n) {
        if (n < 2) return false;
        for (ull p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
            if (n % p == 0) return n == p;
        }

        // 将 n-1 分解为 d * 2^r
        ull d = n - 1;
        int r = 0;
        while ((d & 1) == 0) {
            d >>= 1;
            r++;
        }

        for (ull a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
            if (a >= n) continue;
            ull x = mod_pow(a, d, n);
            if (x == 1 || x == n - 1) continue;

            bool composite = true;
            for (int i = 0; i < r - 1; i++) {
                x = (ull)x * x % n;
                if (x == n - 1) {
                    composite = false;
                    break;
                }
            }
            if (composite) return false;
        }
        return true;
    }

    // 传统 O(√n) 判定
    bool is_prime_traditional(ull n) {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;

        ull limit = sqrt(n);
        for (ull i = 5; i <= limit; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    // 6k±1优化版（带小素数预筛）
    bool is_prime_fast(ull n) {
        if (n < 2) return false;
        static const ull small_primes[] = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
        };

        for (ull p : small_primes) {
            if (n == p) return true;
            if (n % p == 0) return false;
        }
        ull limit = sqrt(n);
        for (ull i = 101; i <= limit; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    // mode选择不同的素性测试方法
    bool is_prime(ull n, const std::string& mode = "m-r") {
        if (mode == "easy") {
            return is_prime_traditional(n);
        }
        else if (mode == "m-r") {
            return miller_rabin(n);
        }
        else if (mode == "fast") {
            return is_prime_fast(n);
        }
        else {
            throw std::invalid_argument("Unknown mode: " + mode + ". Use 'easy', 'm-r', or 'fast'.");
            throw std::invalid_argument("未知模式: " + mode + "。 请使用 'easy', 'm-r', 或者 'fast'.");
        }
    }





    // ============ 回文数判定函数 ============

    // 1. 数字转字符串
    bool is_palindrome_string(ull n) {
        std::string s = std::to_string(n);
        std::string rev = s;
        std::reverse(rev.begin(), rev.end());
        return s == rev;
    }

    // 2. 数字反转法
    bool is_palindrome_reverse(ull n) {
        if (n < 0) return false;
        if (n < 10) return true;
        if (n % 10 == 0) return false;

        ull reversed = 0;
        ull temp = n;
        while (temp > 0) {
            reversed = reversed * 10 + temp % 10;
            temp /= 10;
        }
        return n == reversed;
    }

    // 3. 半反转法
    bool is_palindrome_half(ull n) {
        if (n < 0) return false;
        if (n < 10) return true;
        if (n % 10 == 0) return false;

        ull reversed_half = 0;
        while (n > reversed_half) {
            reversed_half = reversed_half * 10 + n % 10;
            n /= 10;
        }
        // 对于奇数位数，去掉中间位
        return n == reversed_half || n == reversed_half / 10;
    }

    // 4. 双指针法
    bool is_palindrome_string_dual(const std::string& s) {
        int left = 0, right = s.length() - 1;
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }

    // 5. 忽略大小写和非字母数字字符
    bool is_palindrome_alnum(const std::string& s) {
        int left = 0, right = s.length() - 1;
        while (left < right) {
            while (left < right && !isalnum(s[left])) left++;
            while (left < right && !isalnum(s[right])) right--;

            if (tolower(s[left]) != tolower(s[right])) return false;
            left++;
            right--;
        }
        return true;
    }

    // 6. 检查子串是否为回文
    bool is_palindrome_substring(const std::string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }

    bool is_palindrome(ull n, const std::string& mode = "auto") {
        if (mode == "string") {
            return is_palindrome_string(n);
        }
        else if (mode == "reverse") {
            return is_palindrome_reverse(n);
        }
        else if (mode == "half") {
            return is_palindrome_half(n);
        }
        else if (mode == "auto") {
            return is_palindrome_half(n);
        }
        else {
            throw std::invalid_argument("Unknown mode: " + mode +
                ". Use 'string', 'reverse', 'half', or 'auto'.");
        }
    }

    bool is_palindrome(const std::string& s, const std::string& mode = "auto") {
        if (mode == "exact") {
            return is_palindrome_string_dual(s);
        }
        else if (mode == "ignore_case") {
            std::string lower_s = s;
            transform(lower_s.begin(), lower_s.end(), lower_s.begin(), ::tolower);
            return is_palindrome_string_dual(lower_s);
        }
        else if (mode == "alnum") {
            return is_palindrome_alnum(s);
        }
        else if (mode == "auto") {
            bool has_alnum = false;
            bool has_non_alnum = false;
            for (char c : s) {
                if (isalnum(c)) has_alnum = true;
                else has_non_alnum = true;
            }
            if (has_non_alnum) return is_palindrome_alnum(s);
            else return is_palindrome_string_dual(s);
        }
        else {
            throw std::invalid_argument("Unknown mode: " + mode +
                ". Use 'exact', 'ignore_case', 'alnum', or 'auto'.");
        }
    }

    // ============ 扩展功能 ============

    // 找出范围内的所有回文数
    std::vector<ull> find_palindromes(ull start, ull end) {
        std::vector<ull> result;
        for (ull i = start; i <= end; i++) {
            if (is_palindrome(i, "half")) {
                result.push_back(i);
            }
        }
        return result;
    }


    // 生成第n个回文数
    ull nth_palindrome(int n) {
        if (n <= 0) return 0;
        if (n <= 9) return n;

        int digits = 1;
        long long count = 9;
        while (n > count) {
            n -= count;
            digits++;
            if (digits % 2 == 1) {
                count = 9 * pow(10, digits / 2);
            }
            else {
                count = 9 * pow(10, digits / 2 - 1);
            }
        }

        ull half = pow(10, (digits - 1) / 2) + n - 1;
        ull result = half;
        if (digits % 2 == 1) half /= 10;
        while (half > 0) {
            result = result * 10 + half % 10;
            half /= 10;
        }
        return result;
    }


    // ========== 7. 数组快捷操作 ==========
#define l_sz(arr) ((int)(arr).size())
#define l_all(arr) (arr).begin(), (arr).end()
#define l_rall(arr) (arr).rbegin(), (arr).rend()
#define l_fi first
#define l_se second

// ========== 8. 计时器 ==========
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

    // 快速计时
    template<typename F>
    auto timeit(F&& f, const std::string& name = "") {
        Timer t(name);
        return f();
    }

    // ========== 9. 随机数 ==========
    inline int randint(int min, int max) {
        static std::mt19937 gen([] {
            std::random_device rd;
            // MinGW fallback
#ifdef __MINGW32__
            return std::chrono::steady_clock::now().time_since_epoch().count();
#else
            return rd();
#endif
            }());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    inline double randdouble(double min, double max) {
        static std::mt19937 gen([] {
            std::random_device rd;
#ifdef __MINGW32__
            return std::chrono::steady_clock::now().time_since_epoch().count();
#else
            return rd();
#endif
            }());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    // ========== 10. 文件操作 ==========
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

    // ========== 11. 字符串工具 ==========
    inline std::string lower(std::string s) {
        for (char& c : s) c = std::tolower(c);
        return s;
    }

    inline std::string upper(std::string s) {
        for (char& c : s) c = std::toupper(c);
        return s;
    }

    inline std::vector<std::string> split(const std::string& s, char delim, bool skip_empty = true) {
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

    // ========== 12. 容器输出 ==========
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
        os << '[';
        for (size_t i = 0; i < v.size(); ++i) {
            os << v[i] << (i + 1 == v.size() ? "" : ", ");
        }
        return os << ']';
    }

    template<typename K, typename V>
    std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
        os << '{';
        bool first = true;
        for (const auto& [k, v] : m) {
            if (!first) os << ", ";
            os << k << ": " << v;
            first = false;
        }
        return os << "}";
    }

    // ========== 13. 便捷函数 ==========
    inline void wait(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }


    //============= 14, OIer专用函数=============

    //背包dp，支持01背包、完全背包、多重背包（使用二进制优化），以及求最大价值或最小费用
    class BackpackDP {
    public:
        // 主函数：返回最大价值
        static int solve(
            const std::vector<int>& weights,
            const std::vector<int>& values,
            int capacity,
            const std::string& mode = "01",
            const std::vector<int>& counts = {},
            bool maximize = true
        ) {
            int n = weights.size();
            if (n == 0 || capacity <= 0) return 0;

            // 初始化DP数组
            const int INF = 1e9;
            std::vector<int> dp(capacity + 1, maximize ? 0 : INF);
            if (!maximize) dp[0] = 0;

            if (mode == "01") {
                // 01背包：逆序更新
                for (int i = 0; i < n; i++) {
                    for (int j = capacity; j >= weights[i]; j--) {
                        if (maximize) {
                            dp[j] = (((dp[j]) > (dp[j - weights[i]] + values[i])) ? (dp[j]) : (dp[j - weights[i]] + values[i]));
                        }
                        else {
                            dp[j] = (((dp[j]) < (dp[j - weights[i]] + values[i])) ? (dp[j]) : (dp[j - weights[i]] + values[i]));
                        }
                    }
                }
            }
            else if (mode == "inf" || mode == "infb") {
                // 完全背包：正序更新
                for (int i = 0; i < n; i++) {
                    for (int j = weights[i]; j <= capacity; j++) {
                        if (maximize) {
                            dp[j] = (((dp[j]) > (dp[j - weights[i]] + values[i])) ? (dp[j]) : (dp[j - weights[i]] + values[i]));
                        }
                        else {
                            dp[j] = (((dp[j]) < (dp[j - weights[i]] + values[i])) ? (dp[j]) : (dp[j - weights[i]] + values[i]));
                        }
                    }
                }
            }
            else if (mode == "multi") {
                // 多重背包：二进制优化
                if (counts.size() != n) {
                    std::cerr << "Error: counts size must equal weights size for multi mode" << std::endl;
                    return 0;
                }

                // 二进制拆分
                std::vector<int> newWeights, newValues;
                for (int i = 0; i < n; i++) {
                    int k = 1;
                    int remaining = counts[i];
                    while (k <= remaining) {
                        newWeights.push_back(weights[i] * k);
                        newValues.push_back(values[i] * k);
                        remaining -= k;
                        k <<= 1;
                    }
                    if (remaining > 0) {
                        newWeights.push_back(weights[i] * remaining);
                        newValues.push_back(values[i] * remaining);
                    }
                }

                // 对拆分后的物品做01背包
                for (int i = 0; i < (int)newWeights.size(); i++) {
                    for (int j = capacity; j >= newWeights[i]; j--) {
                        if (maximize) {
                            dp[j] = (((dp[j]) > (dp[j - newWeights[i]] + newValues[i])) ? (dp[j]) : (dp[j - newWeights[i]] + newValues[i]));
                        }
                        else {
                            dp[j] = (((dp[j]) < (dp[j - newWeights[i]] + newValues[i])) ? (dp[j]) : (dp[j - newWeights[i]] + newValues[i]));
                        }
                    }
                }
            }
            else {
                err("Error: Unknown mode '", mode, "'\n");
                return 0;
            }

            // 返回结果
            if (maximize) {
                return *std::max_element(dp.begin(), dp.end());
            }
            else {
                int result = INF;
                for (int v : dp) {
                    if (v < INF) result = (((result) < (v)) ? (result) : (v));
                }
                return result == INF ? 0 : result;
            }
        }
    };

    // 便捷包装函数
    int backpackdp(
        const std::vector<int>& weights,
        const std::vector<int>& values,
        int capacity,
        const std::string& mode = "01",
        const std::vector<int>& counts = {},
        bool maximize = true
    ) {
        return BackpackDP::solve(weights, values, capacity, mode, counts, maximize);
    }

}


