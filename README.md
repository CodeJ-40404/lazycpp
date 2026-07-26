# LazyCpp

> 一个现代 C++ 工具库，让 C++ 开发变得"懒"而高效

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-17%2F20-blue)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)]()

---

## 📖 介绍

**LazyCpp** 是一个头文件驱动的 C++ 工具库，旨在为日常 C++ 开发提供一组**简洁、现代、跨平台**的基础设施。

### 设计哲学

- **懒，但高效** —— 用最少的代码完成最多的事情
- **安全第一，便捷可选** —— 三层命名空间控制，污染由你决定
- **现代 C++ 优先** —— C++17/20 特性贯穿始终
- **零依赖，开箱即用** —— 只需 `#include "lazy.h"`

### 为什么选择 LazyCpp？

| 场景 | 传统 C++ | LazyCpp |
|------|---------|---------|
| 打印容器 | `for (auto& x : v) cout << x << ", ";` | `lazy::container::print(v);` |
| 随机数 | 手写 `std::random_device` + `mt19937` | `lazy::random::rand_int(1, 100);` |
| 字符串分割 | 手写循环或 `getline` | `lazy::string::split(str, ",");` |
| 质数判定 | 自己实现 Miller-Rabin | `lazy::maths::is_prime(n);` |
| 崩溃捕获 | 平台特定的信号处理 | `lazy::debug::init("crash.log");` |

### 快速开始

```cpp
#include "lazy.h"

// 安全模式（推荐）
int main() {
    lazy::io::fast_io();
    lazy::io::log_info("Hello, LazyCpp!");
    
    auto vec = lazy::random::rand_int_array(10, 0, 100);
    lazy::container::print(vec);
    // 输出: [12, 45, 67, 89, 23, 56, 78, 34, 90, 11]
    
    return 0;
}
```

### 使用模式

```cpp
// 1. 🛡️ 安全模式 —— 完全限定
lazy::maths::gcd(12, 18);
lazy::string::to_upper("hello");

// 2. ⚡ 便捷模式 —— 只需 lazy:: 前缀（需定义 LAZY_EXPORT_ALL）
#define LAZY_EXPORT_ALL
#include "lazy.h"
lazy::gcd(12, 18);
lazy::to_upper("hello");

// 3. 🚀 局部裸奔模式 —— 函数内随意（推荐）
void func() {
    LAZY_USE_ALL;
    gcd(12, 18);      // 来自 lazy::maths
    to_upper("hello"); // 来自 lazy::string
    print(vec);       // 来自 lazy::container
}
```

---

## 📦 功能清单

### 1. Core —— 核心基础设施 (`lazy::core`)

提供最常用的类型别名、常量、宏和基础工具。

#### 类型别名

| 别名 | 实际类型 | 用途 |
|------|---------|------|
| `str` | `std::string` | 字符串 |
| `ll` / `ull` | `long long` / `unsigned long long` | 64位整数 |
| `db` / `ld` | `double` / `long double` | 浮点数 |
| `vi` / `vll` / `vstr` | `vector<int/long long/string>` | 常用向量 |
| `vvi` | `vector<vector<int>>` | 二维向量 |
| `pii` / `pll` | `pair<int/long long, ...>` | 键值对 |
| `mii` / `msi` / `umii` | `map/unordered_map` | 映射表 |
| `si` / `usi` | `set/unordered_set` | 集合 |
| `qui` / `pqi` | `queue/priority_queue` | 队列 |
| `ti` / `tll` | `tuple<int/long long, ...>` | 元组 |
| `i128` | `__int128` (GCC/Clang) | 128位整数 |

#### 常量

```cpp
lazy::core::INF      // 1e9
lazy::core::LINF     // 4e18
lazy::core::PI       // 3.14159265358979323846
lazy::core::E        // 2.71828182845904523536
lazy::core::EPS      // 1e-9
lazy::core::MOD      // 1000000007
lazy::core::MOD9     // 998244353
lazy::core::MAXN     // 1000005
```

#### 便捷宏

| 宏 | 功能 |
|----|------|
| `rep(i, n)` | `for (int i = 0; i < n; i++)` |
| `rep1(i, n)` | `for (int i = 1; i <= n; i++)` |
| `rrep(i, n)` | `for (int i = n-1; i >= 0; i--)` |
| `each(x, arr)` | `for (auto& x : arr)` |
| `repeat(times)` | 重复执行 N 次 |
| `all(arr)` | `arr.begin(), arr.end()` |
| `rall(arr)` | `arr.rbegin(), arr.rend()` |
| `sz(arr)` | `(int)arr.size()` |
| `fi` / `se` | `first` / `second` |

#### 输入输出函数

```cpp
lazy::core::read(a, b, c);      // 可变参数输入
lazy::core::out("Hello", 42);   // 不换行输出
lazy::core::outl("Hello", 42);  // 换行输出
```

#### 计时器

```cpp
{
    lazy::core::Timer t("Sort");
    std::sort(vec.begin(), vec.end());
}  // 析构时自动输出: "Sort: 1234 microseconds"

// 函数式计时
auto result = lazy::core::timeit([]() {
    return expensive_operation();
}, "Operation");
```

#### 文件重定向

```cpp
lazy::core::set_input("in.txt");
lazy::core::set_output("out.txt");
lazy::core::set_io("in.txt", "out.txt");
```

#### 其他工具

```cpp
lazy::core::wait(100);  // 等待 100ms
lazy::core::fast_IO();  // 同步流加速
```

---

### 2. Maths —— 数学工具库 (`lazy::maths`)

全面的数学函数、数论工具和线性代数支持。

#### 基础数学

```cpp
lazy::maths::abs_val(-5);      // 5
lazy::maths::sign(-3.14);      // -1
lazy::maths::eq(a, b, 1e-6);   // 浮点数相等比较
lazy::maths::lt(a, b);         // a < b (带容差)
lazy::maths::gt(a, b);         // a > b (带容差)
lazy::maths::floor_div(a, b);  // 向下取整除法
lazy::maths::ceil_div(a, b);   // 向上取整除法
lazy::maths::round_to_int(3.7); // 4
```

#### 最大公约数 & 最小公倍数

```cpp
lazy::maths::gcd(48, 18);       // 6
lazy::maths::lcm(12, 18);       // 36
ll x, y;
lazy::maths::exgcd(30, 18, x, y); // 扩展欧几里得
```

#### 模运算

```cpp
lazy::maths::mod_norm(-5, 3);   // 1 (保证非负)
lazy::maths::mod_pow(2, 10, 100); // 24 (2^10 % 100)
lazy::maths::mod_inv(3, 7);     // 5 (3 * 5 ≡ 1 mod 7)

// 中国剩余定理
vector<ll> a = {2, 3, 1};
vector<ll> m = {3, 5, 7};
lazy::maths::crt(a, m);         // x ≡ 2 mod 3, x ≡ 3 mod 5, x ≡ 1 mod 7
```

#### 素数相关

| 函数 | 说明 |
|------|------|
| `is_prime_traditional(n)` | O(√n) 朴素判定 |
| `miller_rabin(n)` | Miller-Rabin 素性测试 |
| `is_prime(n, "easy"/"m-r")` | 统一接口，默认 Miller-Rabin |
| `sieve(n)` | 埃氏筛，返回 `vector<bool>` |
| `prime_list(n)` | 返回 ≤n 的所有素数 |

```cpp
lazy::maths::is_prime(1000000007);  // true
auto primes = lazy::maths::prime_list(100);
```

#### 因数分解

```cpp
auto factors = lazy::maths::prime_factorize(84);
// [(2, 2), (3, 1), (7, 1)]

lazy::maths::divisor_count(84);     // 12
auto divs = lazy::maths::divisors(84);
// [1, 2, 3, 4, 6, 7, 12, 14, 21, 28, 42, 84]
```

#### 回文数

```cpp
lazy::maths::is_palindrome(12321);      // true
lazy::maths::is_palindrome("racecar");  // true
```

#### 排列组合

```cpp
lazy::maths::factorial(5);          // 120
lazy::maths::perm(10, 3);           // 720
lazy::maths::comb_small(10, 3);     // 120

// DP 表查询
lazy::maths::Comb comb(100);
comb.get(10, 3);                    // 120

// 模意义组合
lazy::maths::CombMod cm(100000, 1000000007);
cm.comb(100000, 50000);             // C(100000, 50000) % MOD
cm.perm(100, 20);                   // P(100, 20) % MOD
```

#### 矩阵运算

```cpp
using Matrix = lazy::maths::Matrix<ll>;

Matrix A({{1, 2}, {3, 4}});
Matrix B({{5, 6}, {7, 8}});

auto C = A + B;
auto D = A * B;
auto E = A.transpose();
auto F = A.pow(10, MOD);  // 矩阵快速幂取模

// 矩阵乘法（优化版）
auto G = lazy::maths::mat_mul_mod(A, B, MOD);
```

#### 斐波那契数列

```cpp
lazy::maths::fib_matrix(50);        // 12586269025
lazy::maths::fib_matrix(50, MOD);   // 取模
lazy::maths::fib_iter(50);          // 传统递推
```

#### 线性代数工具

```cpp
vector<int> a = {1, 2, 3};
vector<int> b = {4, 5, 6};
lazy::maths::dot(a, b);             // 32
lazy::maths::norm(a);               // 3.74165
auto normalized = lazy::maths::normalize(a);
```

#### 进制转换

```cpp
lazy::maths::to_base(255, 16);      // "FF"
lazy::maths::from_base("FF", 16);   // 255
lazy::maths::to_base(42, 2);        // "101010"
```

---

### 3. String —— 字符串工具 (`lazy::string`)

丰富的字符串处理函数，风格类似 Python。

#### 大小写转换

```cpp
lazy::string::to_lower("HELLO");    // "hello"
lazy::string::to_upper("hello");    // "HELLO"
lazy::string::capitalize("hello");  // "Hello"
lazy::string::title_case("hello world"); // "Hello World"
```

#### 去除空白

```cpp
lazy::string::trim("  hello  ");     // "hello"
lazy::string::trim_left("  hello");  // "hello"
lazy::string::trim_right("hello  "); // "hello"
```

#### 分割与连接

```cpp
auto parts = lazy::string::split("a,b,c,d", ",");
// ["a", "b", "c", "d"]

auto parts2 = lazy::string::split("a b\tc\n", " \t\n");
// ["a", "b", "c"]

auto joined = lazy::string::join(parts, " | ");
// "a | b | c | d"

auto parts3 = lazy::string::split_char("a,b,c,d", ',');
// ["a", "b", "c", "d"]
```

#### 查找与替换

```cpp
lazy::string::contains("hello world", "world");  // true
lazy::string::starts_with("hello", "he");        // true
lazy::string::ends_with("hello", "lo");          // true
lazy::string::replace_all("foo bar foo", "foo", "baz"); // "baz bar baz"
lazy::string::replace_first("foo bar foo", "foo", "baz"); // "baz bar foo"
```

#### 其他操作

```cpp
lazy::string::reverse("hello");          // "olleh"
lazy::string::repeats("hi", 3);          // "hihihi"
lazy::string::pad_left("hi", 5, '-');    // "---hi"
lazy::string::pad_right("hi", 5, '-');   // "hi---"
lazy::string::pad_center("hi", 5, '-');  // "-hi--"
```

#### 类型转换

```cpp
lazy::string::to_string(42);            // "42"
lazy::string::to_string(3.14);          // "3.14"
lazy::string::from_string<int>("42");   // 42
lazy::string::bool_to_string(true);     // "true"
```

#### 验证函数

```cpp
lazy::string::is_number("3.14");        // true
lazy::string::is_number("-42");         // true
lazy::string::is_integer("123");        // true
lazy::string::is_integer("3.14");       // false
lazy::string::is_alnum("abc123");       // true
lazy::string::is_alpha("abc");          // true
lazy::string::is_alpha("abc123");       // false
```

#### 格式化（类 Python format）

```cpp
lazy::string::format("Hello {0}! Your score is {1}", "Alice", 95);
// "Hello Alice! Your score is 95"
```

---

### 4. Container —— 容器工具 (`lazy::container`)

STL 容器的增强工具函数。

#### 向量工具

```cpp
vector<int> v = {1, 2, 3, 4, 5};

lazy::container::contains(v, 3);        // true
lazy::container::index_of(v, 3);        // 2
lazy::container::concat(v, {6, 7});     // [1,2,3,4,5,6,7]
lazy::container::slice(v, 1, 4);        // [2,3,4]
lazy::container::remove_first(v, 3);    // v = [1,2,4,5]
lazy::container::remove_all(v, 2);      // v = [1,4,5]
lazy::container::is_sorted(v);          // true
lazy::container::reversed(v);           // [5,4,1]
```

#### Map 工具

```cpp
map<string, int> scores = {{"Alice", 95}, {"Bob", 87}};

lazy::container::get_or_default(scores, "Alice", 0);  // 95
lazy::container::get_or_default(scores, "Charlie", 0); // 0
lazy::container::contains_key(scores, "Bob");          // true
lazy::container::keys(scores);           // ["Alice", "Bob"]
lazy::container::values(scores);         // [95, 87]

auto merged = lazy::container::merge(scores, {{"Charlie", 92}});
```

#### Set 工具

```cpp
set<int> a = {1, 2, 3, 4};
set<int> b = {3, 4, 5, 6};

lazy::container::intersect(a, b);        // {3, 4}
lazy::container::unite(a, b);            // {1, 2, 3, 4, 5, 6}
lazy::container::difference(a, b);       // {1, 2}
lazy::container::symmetric_difference(a, b); // {1, 2, 5, 6}
```

#### 队列/栈工具

```cpp
queue<int> q;
lazy::container::clear_queue(q);  // 清空队列

stack<int> s;
lazy::container::clear_stack(s);  // 清空栈
```

#### 通用容器工具

```cpp
lazy::container::empty(vec);        // 判空
lazy::container::size(vec);         // 获取大小
lazy::container::print(vec);        // "[1, 2, 3, 4, 5]"
lazy::container::print(vec, " | ", "{", "}"); // "{1 | 2 | 3 | 4 | 5}"

auto str = lazy::container::to_string(vec); // "[1, 2, 3, 4, 5]"
```

#### BiMap —— 双向多索引映射

```cpp
lazy::container::BiMap<string, int, string> users;
users.insert("alice", 1001, "Alice");
users.insert("bob", 1002, "Bob");

users.get_by_key1("alice");     // "Alice"
users.get_by_key2(1002);        // "Bob"
users.get_keys_by_value("Alice"); // {"alice", 1001}
users.contains_key1("charlie"); // false
users.contains_value("Bob");    // true
```

---

### 5. Algorithm —— 算法工具 (`lazy::algorithm`)

常用算法实现。

#### 二分查找

```cpp
vector<int> arr = {1, 3, 5, 7, 9};

lazy::algorithm::lower_bound(arr, 5);  // 2 (第一个 >= 5)
lazy::algorithm::upper_bound(arr, 5);  // 3 (第一个 > 5)
```

#### 排序相关

```cpp
vector<int> arr = {1, 2, 3, 4, 5};
lazy::algorithm::reverse(arr);         // arr = [5,4,3,2,1]
lazy::algorithm::maxof(arr);           // 5
lazy::algorithm::minof(arr);           // 1
```

#### 去重

```cpp
vector<int> arr = {1, 2, 2, 3, 3, 3};
auto unique = lazy::algorithm::unique(arr); // [1, 2, 3]
```

#### 前缀和

```cpp
vector<int> arr = {1, 2, 3, 4, 5};
auto pref = lazy::algorithm::prefix_sum(arr); // [0, 1, 3, 6, 10, 15]
lazy::algorithm::range_sum(pref, 1, 3);       // 9 (2+3+4)
```

---

### 6. Random —— 随机数生成 (`lazy::random`)

基于 `mt19937_64` 的完整随机数工具。

#### 整数随机

```cpp
lazy::random::rand_int(1, 100);        // [1, 100]
lazy::random::rand_ll(1e12, 1e13);     // 64位整数
lazy::random::rand_uint(0, 1000);      // 无符号整数
```

#### 浮点数随机

```cpp
lazy::random::rand_double(0.0, 1.0);   // [0, 1]
lazy::random::rand_float(0.0f, 1.0f);  // [0, 1]
lazy::random::rand_uniform();          // [0, 1)
lazy::random::rand_normal(0.0, 1.0);   // 正态分布
```

#### 布尔随机

```cpp
lazy::random::rand_bool();             // 50% 概率 true
lazy::random::rand_bool(0.7);          // 70% 概率 true
```

#### 容器随机

```cpp
vector<int> v = {1, 2, 3, 4, 5};
lazy::random::rand_choice(v);          // 随机元素
lazy::random::shuffle(v);              // 随机打乱
auto sampled = lazy::random::sample(v, 3); // 随机采样 3 个
```

#### 随机字符串

```cpp
lazy::random::rand_string(10);         // "aB3dEfGhIj"
lazy::random::rand_digits(6);          // "483729"
lazy::random::rand_letters(8);         // "AbCdEfGh"
lazy::random::rand_lowercase(5);       // "abcde"
lazy::random::rand_uppercase(5);       // "ABCDE"
lazy::random::rand_hex(8);             // "A3F7B2C8"
```

#### 随机数组

```cpp
auto ints = lazy::random::rand_int_array(10, 0, 100);
auto floats = lazy::random::rand_double_array(5, 0.0, 1.0);
auto perm = lazy::random::rand_permutation(10); // [3, 7, 1, ...]
```

#### 概率分布

```cpp
lazy::random::rand_exponential(2.0);   // 指数分布
lazy::random::rand_poisson(3.5);       // 泊松分布
lazy::random::rand_normal_array(100);  // 正态分布数组
```

#### RNG 单例控制

```cpp
auto& rng = lazy::random::RNG::instance();
rng.reseed();                          // 重新播种
rng.reseed(12345);                     // 固定种子
auto& engine = rng.engine();           // 获取底层 mt19937_64
```

---

### 7. IO —— 输入输出工具 (`lazy::io`)

增强的 IO 操作，支持彩色输出、进度条等。

#### 快速 IO

```cpp
lazy::io::fast_io();  // 加速 cin/cout
```

#### 文件操作

```cpp
auto content = lazy::io::read_file("input.txt");
auto lines = lazy::io::read_lines("input.txt");
lazy::io::write_file("output.txt", "Hello");
lazy::io::append_file("log.txt", "New line\n");
```

#### 彩色输出 (跨平台)

```cpp
// Linux/macOS (ANSI)
lazy::io::print_color(lazy::io::color::RED, "Error!");
lazy::io::println_color(lazy::io::color::GREEN, "Success!");

// Windows (数字颜色)
lazy::io::print_color(lazy::io::RED, "Error!");
lazy::io::println_color(lazy::io::GREEN, "Success!");
```

#### 日志输出 (带颜色)

```cpp
lazy::io::log_info("Starting process...");
lazy::io::log_warn("Memory usage high");
lazy::io::log_error("Connection failed");
lazy::io::log_success("Task completed");
```

#### 进度条

```cpp
lazy::io::ProgressBar bar(100, 50, '=', ' ');
for (int i = 0; i <= 100; i++) {
    bar.update(i);
    // 模拟工作
}
// 输出: [==========================>                   ] 50%
```

#### 分隔线和标题

```cpp
lazy::io::separator('-', 60);
lazy::io::title("Welcome to LazyCpp", '=', 60);
// ================== Welcome to LazyCpp ==================
```

#### 安全输入

```cpp
int age = lazy::io::input<int>("Enter age: ");  // 自动重试直到输入有效
string name = lazy::io::input_line("Enter name: ");
```

---

### 8. Debug —— 调试与崩溃捕获 (`lazy::debug`)

自动捕获程序崩溃并生成详细报告。

#### 快速初始化

```cpp
int main() {
    lazy::debug::init("crash.log");  // 初始化崩溃捕获
    // ... 你的代码 ...
    return 0;
}
```

#### 日志记录

```cpp
lazy::debug::log_info("Application started");
lazy::debug::log_warning("Cache miss", "key: user_123");
lazy::debug::log_error("Database connection failed", "timeout after 5s");
```

#### 配置选项

```cpp
lazy::debug::enable_console_log();   // 同时输出到控制台
lazy::debug::disable_console_log();  // 只输出到文件
lazy::debug::set_log_path("logs/crash.log");
```

#### 崩溃报告示例

当程序崩溃时（如空指针、除零、栈溢出），自动生成报告：

```
========================================
         CRASH REPORT
========================================

[TIME]
Mon Jul 26 14:32:18 2026

[ERROR]
Type: ACCESS_VIOLATION (Code: 0xc0000005)
Address: 0x00007ff6a1b2c3d4

[STACK TRACE]
  #0 main + 0x42 [myapp.exe]
  #1 __scrt_common_main_seh + 0x250 [myapp.exe]
  #2 BaseThreadInitThunk + 0x14 [kernel32.dll]

[SYSTEM INFO]
  C++ Standard: 202002
  Compiler: MSVC 1934
  Platform: Windows x64
  Architecture: x86_64
  Process ID: 12345

========================================
 Tip: Check null pointers, array bounds, memory leaks
========================================
```

#### 高级用法

```cpp
// 自定义配置
lazy::debug::Config cfg;
cfg.log_path = "logs/crash.log";
cfg.enable_console_output = true;
cfg.max_stack_frames = 128;

lazy::debug::Debugger dbg;
dbg.init(cfg);
dbg.log_warning("Custom warning");
```

---

## 📁 目录结构

```
lazy/
├── lazy.h              # 统一入口
├── lazy_config.h       # 配置宏
├── lazy_core.h         # 核心基础设施
├── lazy_maths.h        # 数学工具库
├── lazy_string.h       # 字符串工具
├── lazy_container.h    # 容器工具
├── lazy_algorithm.h    # 算法工具
├── lazy_random.h       # 随机数生成
├── lazy_io.h           # IO 工具
└── lazy_debug.h        # 调试与崩溃捕获
```

---

## 🚀 编译要求

| 编译器 | 最低版本 | 备注 |
|--------|---------|------|
| MSVC | 2019 (16.0) | 需要 C++17 支持 |
| GCC | 7.0+ | 需要 C++17 支持 |
| Clang | 5.0+ | 需要 C++17 支持 |

### 编译选项

```bash
# 安全模式（默认）
g++ -std=c++17 main.cpp -o main

# 便捷模式
g++ -std=c++17 -DLAZY_EXPORT_ALL main.cpp -o main

# 调试模式（崩溃日志）
g++ -std=c++17 -g -O0 main.cpp -o main
```

---

## 📄 License

MIT License —— 随便用，随便改，保留版权声明即可。

---

## ✨ 最后

LazyCpp 是一个"玩具项目"，但它认真对待每一行代码。

**写 C++，懒一点，快一点。** 🚀
