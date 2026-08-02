#define NOMINMA
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define LAZY_EXPORT_ALL
#include "lazy.h"

// again!!!
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// ============================================================
// 辅助函数：清屏和暂停
// ============================================================
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void wait_for_enter(const std::string& msg = "按 Enter 键继续...") {
    lazy::io::log_info(msg);
    std::cin.get();
}

int main() {
    // ============================================================
    // 1. 初始化
    // ============================================================
    LAZY_INIT();
    clear_screen();

    // ============================================================
    // 2. 核心功能 (lazy::core)
    // ============================================================
    lazy::io::title("LAZYCPP 功能演示", '=', 60);
    lazy::outl();
    lazy::io::log_info("版本: ", LAZY_VERSION_MAJOR, ".", LAZY_VERSION_MINOR, ".", LAZY_VERSION_PATCH);
    lazy::outl();

    lazy::vi numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    lazy::vstr names = { "张三", "李四", "王五" };
    lazy::msi age_map = { {"张三", 25}, {"李四", 30}, {"王五", 35} };

    lazy::outl("核心类型演示:");
    lazy::outl("  数字数组: ");
    for (auto x : numbers) lazy::out(x, " ");
    lazy::outl();
    lazy::outl("  姓名列表: ", lazy::string::join(names, ", "));
    lazy::outl("  张三的年龄: ", age_map["张三"]);
    lazy::outl();

    lazy::outl("常量:");
    lazy::outl("  PI = ", lazy::core::PI);
    lazy::outl("  INF = ", lazy::core::INF);
    lazy::outl("  MOD = ", lazy::core::MOD);
    lazy::outl();

    lazy::outl("计时器演示:");
    {
        lazy::core::Timer t("循环计时");
        int sum = 0;
        for (int i = 0; i < 1000000; i++) sum += i;
        lazy::outl("  累加和 = ", sum);
    }
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 3. 数学功能 (lazy::maths)
    // ============================================================
    clear_screen();
    lazy::io::title("数学功能演示", '=', 60);
    lazy::outl();

    lazy::io::log_info("基础数学:");
    lazy::outl("  GCD(48, 18) = ", lazy::maths::gcd(48, 18));
    lazy::outl("  LCM(12, 18) = ", lazy::maths::lcm(12, 18));
    lazy::outl("  2^10 = ", lazy::maths::mod_pow(2, 10, 1000000007));
    lazy::outl("  sqrt(2) = ", std::sqrt(2.0));
    lazy::outl();

    lazy::io::log_info("模运算:");
    lazy::outl("  2^10 mod 100 = ", lazy::maths::mod_pow(2, 10, 100));
    lazy::outl("  3^5 mod 7 = ", lazy::maths::mod_pow(3, 5, 7));
    lazy::outl("  5 mod 3 = ", lazy::maths::mod_norm(5, 3));
    lazy::outl("  -5 mod 3 = ", lazy::maths::mod_norm(-5, 3));
    lazy::outl();

    lazy::io::log_info("素数:");
    lazy::outl("  17 是素数? ", lazy::maths::is_prime(17));
    lazy::outl("  100 是素数? ", lazy::maths::is_prime(100));
    lazy::outl("  20 以内的素数: ");
    auto primes = lazy::maths::prime_list(20);
    for (auto p : primes) lazy::out(p, " ");
    lazy::outl();
    lazy::outl();

    lazy::io::log_info("因数分解 84:");
    auto factors = lazy::maths::prime_factorize(84);
    for (auto [p, e] : factors) {
        lazy::outl("  ", p, "^", e);
    }
    lazy::outl("  约数个数: ", lazy::maths::divisor_count(84));
    auto divs = lazy::maths::divisors(84);
    lazy::outl("  所有约数: ");
    for (auto d : divs) lazy::out(d, " ");
    lazy::outl();
    lazy::outl();

    lazy::io::log_info("组合数学:");
    lazy::outl("  C(10, 3) = ", lazy::maths::comb_small(10, 3));
    lazy::outl("  P(10, 3) = ", lazy::maths::perm(10, 3));
    lazy::outl("  5! = ", lazy::maths::factorial(5));

    lazy::maths::CombMod comb_mod(20);
    lazy::outl("  C(10, 3) mod 1e9+7 = ", comb_mod.comb(10, 3));
    lazy::outl();

    lazy::io::log_info("斐波那契数列:");
    lazy::outl("  F(10) = ", lazy::maths::fib_iter(10));
    lazy::outl("  F(20) = ", lazy::maths::fib_matrix(20));
    lazy::outl("  F(20) mod 100 = ", lazy::maths::fib_matrix(20, 100));
    lazy::outl();

    lazy::io::log_info("进制转换:");
    lazy::outl("  255 转十六进制 = ", lazy::maths::to_base(255, 16));
    lazy::outl("  FF 转十进制 = ", lazy::maths::from_base("FF", 16));
    lazy::outl("  42 转二进制 = ", lazy::maths::to_base(42, 2));
    lazy::outl();

    lazy::io::log_info("矩阵运算:");
    lazy::maths::Matrix<int> A({ {1, 2}, {3, 4} });
    lazy::maths::Matrix<int> B({ {5, 6}, {7, 8} });
    lazy::outl("  A = ");
    A.print();
    lazy::outl("  B = ");
    B.print();
    auto C = A * B;
    lazy::outl("  A * B = ");
    C.print();
    lazy::outl("  A^3 = ");
    auto Apow = A.pow(3);
    Apow.print();
    lazy::outl();

    lazy::io::log_info("线性代数:");
    std::vector<double> vec1 = { 1.0, 2.0, 3.0 };
    std::vector<double> vec2 = { 4.0, 5.0, 6.0 };
    lazy::outl("  dot(vec1, vec2) = ", lazy::maths::dot(vec1, vec2));
    lazy::outl("  norm(vec1) = ", lazy::maths::norm(vec1));
    lazy::outl();

    lazy::io::log_info("随机数:");
    lazy::outl("  [1, 10] 随机整数: ", lazy::maths::randint(1, 10));
    lazy::outl("  [0, 1] 随机浮点数: ", lazy::maths::randdouble(0, 1));
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 4. 算法功能 (lazy::algorithm)
    // ============================================================
    clear_screen();
    lazy::io::title("算法功能演示", '=', 60);
    lazy::outl();

    std::vector<int> arr = { 5, 2, 8, 1, 9, 3, 7, 4, 6 };
    lazy::io::log_info("原始数组:");
    for (auto x : arr) lazy::out(x, " ");
    lazy::outl();

    auto arr_copy = arr;
    lazy::algorithm::quick_sort(arr_copy);
    lazy::io::log_success("快速排序结果:");
    for (auto x : arr_copy) lazy::out(x, " ");
    lazy::outl();

    lazy::io::log_info("二分查找:");
    lazy::outl("  lower_bound(5) = ", lazy::algorithm::lower_bound(arr_copy, 5));
    lazy::outl("  upper_bound(5) = ", lazy::algorithm::upper_bound(arr_copy, 5));
    lazy::outl("  first_equal(5) = ", lazy::algorithm::first_equal(arr_copy, 5));
    lazy::outl("  last_equal(5) = ", lazy::algorithm::last_equal(arr_copy, 5));
    lazy::outl("  linear_search(9) = ", lazy::algorithm::linear_search(arr_copy, 9));
    lazy::outl();

    std::vector<int> kadane_arr = { -2, 1, -3, 4, -1, 2, 1, -5, 4 };
    lazy::io::log_info("Kadane 最大子数组和:");
    lazy::outl("  最大子数组和 = ", lazy::algorithm::max_subarray_sum(kadane_arr));
    auto [l, r] = lazy::algorithm::max_subarray_range(kadane_arr);
    lazy::outl("  区间: [", l, ", ", r, "]");
    lazy::outl();

    std::vector<int> lis_arr = { 10, 22, 9, 33, 21, 50, 41, 60 };
    auto lis = lazy::algorithm::longest_increasing_subsequence(lis_arr);
    lazy::io::log_info("最长递增子序列 (LIS):");
    for (auto x : lis) lazy::out(x, " ");
    lazy::outl();
    lazy::outl("  LIS 长度 = ", lazy::algorithm::lis_length(lis_arr));
    lazy::outl();

    std::vector<int> lcs_a = { 1, 2, 3, 4, 5 };
    std::vector<int> lcs_b = { 2, 3, 4, 6, 7 };
    auto lcs = lazy::algorithm::longest_common_subsequence(lcs_a, lcs_b);
    lazy::io::log_info("最长公共子序列 (LCS):");
    lazy::outl("  {1,2,3,4,5} 与 {2,3,4,6,7} 的 LCS:");
    for (auto x : lcs) lazy::out(x, " ");
    lazy::outl();
    lazy::outl();

    std::vector<int> window_arr = { 1, 3, -1, -3, 5, 3, 6, 7 };
    auto max_window = lazy::algorithm::sliding_window_max(window_arr, 3);
    lazy::io::log_info("滑动窗口最大值 (k=3):");
    for (auto x : max_window) lazy::out(x, " ");
    lazy::outl();
    lazy::outl();

    std::vector<int> perm_arr = { 1, 2, 3 };
    auto perms = lazy::algorithm::all_permutations(perm_arr);
    lazy::io::log_info("{1,2,3} 的所有排列:");
    for (const auto& p : perms) {
        for (auto x : p) lazy::out(x, " ");
        lazy::outl();
    }
    lazy::outl();

    auto combos = lazy::algorithm::all_combinations(perm_arr, 2);
    lazy::io::log_info("{1,2,3} 的所有组合 (大小 2):");
    for (const auto& c : combos) {
        for (auto x : c) lazy::out(x, " ");
        lazy::outl();
    }
    lazy::outl();

    std::vector<int> set_a = { 1, 2, 3, 4, 5 };
    std::vector<int> set_b = { 4, 5, 6, 7, 8 };
    auto intersection = lazy::algorithm::set_intersection(set_a, set_b);
    auto union_set = lazy::algorithm::set_union(set_a, set_b);
    lazy::io::log_info("集合操作:");
    lazy::outl("  A = {1,2,3,4,5}, B = {4,5,6,7,8}");
    lazy::outl("  交集: ");
    for (auto x : intersection) lazy::out(x, " ");
    lazy::outl();
    lazy::outl("  并集: ");
    for (auto x : union_set) lazy::out(x, " ");
    lazy::outl();
    lazy::outl("  A 是 B 的子集? ", lazy::algorithm::is_subset(set_a, set_b));
    lazy::outl();

    std::unordered_map<int, std::vector<int>> graph = {
        {1, {2, 3}},
        {2, {4, 5}},
        {3, {6}},
        {4, {}},
        {5, {}},
        {6, {}}
    };
    auto bfs_result = lazy::algorithm::bfs(graph, 1);
    lazy::io::log_info("从 1 开始的 BFS:");
    for (auto x : bfs_result) lazy::out(x, " ");
    lazy::outl();
    lazy::outl();

    std::vector<int> two_sum_arr = { 2, 7, 11, 15 };
    auto pair = lazy::algorithm::two_sum(two_sum_arr, 9);
    lazy::io::log_info("两数之和 (目标=9): 索引 (", pair.first, ", ", pair.second, ")");
    lazy::outl();

    lazy::io::log_info("位运算:");
    lazy::outl("  popcount(0b1010) = ", lazy::algorithm::popcount((unsigned int)0b1010));
    lazy::outl("  is_power_of_two(16) = ", lazy::algorithm::is_power_of_two(16));
    lazy::outl("  lowbit(12) = ", lazy::algorithm::lowbit(12));
    lazy::outl("  lowbit_pos(12) = ", lazy::algorithm::lowbit_pos((unsigned int)12));
    lazy::outl("  highbit_pos(12) = ", lazy::algorithm::highbit_pos((unsigned int)12));
    lazy::outl();

    std::vector<double> stats_arr = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
    lazy::io::log_info("统计量:");
    lazy::outl("  平均值 = ", lazy::algorithm::mean(stats_arr));
    lazy::outl("  中位数 = ", lazy::algorithm::median(stats_arr));
    lazy::outl("  方差 = ", lazy::algorithm::variance(stats_arr));
    lazy::outl("  标准差 = ", lazy::algorithm::stddev(stats_arr));
    lazy::outl();

    lazy::io::log_info("KMP 字符串搜索:");
    auto kmp_result = lazy::algorithm::kmp_search("ABABDABACDABABCABAB", "ABABCABAB");
    lazy::outl("  模式串出现位置: ");
    for (auto pos : kmp_result) lazy::out(pos, " ");
    lazy::outl();
    lazy::outl("  是否为子串? ", lazy::algorithm::is_substring("hello world", "world"));
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 5. 字符串功能 (lazy::string)
    // ============================================================
    clear_screen();
    lazy::io::title("字符串功能演示", '=', 60);
    lazy::outl();

    std::string str = "  你好，世界！  ";
    lazy::io::log_info("原始字符串: \"", str, "\"");
    lazy::outl("  trim: \"", lazy::string::trim(str), "\"");
    lazy::outl("  转小写: \"", lazy::string::to_lower(str), "\"");
    lazy::outl("  转大写: \"", lazy::string::to_upper(str), "\"");
    lazy::outl("  首字母大写: \"", lazy::string::capitalize("hello"), "\"");
    lazy::outl("  标题格式: \"", lazy::string::title_case("hello world"), "\"");
    lazy::outl();

    std::string csv = "苹果,香蕉,橙子,葡萄";
    auto parts = lazy::string::split(csv, ",");
    lazy::io::log_info("分割 \"" + csv + "\":");
    for (const auto& p : parts) lazy::outl("  ", p);
    lazy::outl("  连接: \"", lazy::string::join(parts, " | "), "\"");
    lazy::outl();

    lazy::outl("  包含 \"世界\": ", lazy::string::contains(str, "世界"));
    lazy::outl("  以 \"你好\" 开头: ", lazy::string::starts_with(str, "你好"));
    lazy::outl("  以 \"世界！\" 结尾: ", lazy::string::ends_with(str, "世界！"));
    lazy::outl("  替换全部: ", lazy::string::replace_all("你好 你好 你好", "你好", "嗨"));
    lazy::outl();

    lazy::io::log_info("验证:");
    lazy::outl("  is_number(\"123.45\") = ", lazy::string::is_number("123.45"));
    lazy::outl("  is_number(\"abc\") = ", lazy::string::is_number("abc"));
    lazy::outl("  is_integer(\"-123\") = ", lazy::string::is_integer("-123"));
    lazy::outl("  is_alpha(\"Hello\") = ", lazy::string::is_alpha("Hello"));
    lazy::outl("  is_alnum(\"Hello123\") = ", lazy::string::is_alnum("Hello123"));
    lazy::outl();

    lazy::outl("  格式化: ", lazy::string::format("你好 {0}，年龄 {1}", "小明", 18));
    lazy::outl();

    lazy::io::log_info("填充:");
    lazy::outl("  pad_left: \"", lazy::string::pad_left("42", 5, '0'), "\"");
    lazy::outl("  pad_right: \"", lazy::string::pad_right("42", 5, '0'), "\"");
    lazy::outl("  pad_center: \"", lazy::string::pad_center("42", 5, ' '), "\"");
    lazy::outl();

    lazy::outl("  随机字符串: ", lazy::string::random_string(10));
    lazy::outl("  随机数字: ", lazy::string::random_string(8, "0123456789"));
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 6. 容器功能 (lazy::container)
    // ============================================================
    clear_screen();
    lazy::io::title("容器功能演示", '=', 60);
    lazy::outl();

    std::vector<int> vec = { 1, 2, 3, 4, 5, 3, 6, 7, 3 };
    lazy::io::log_info("向量:");
    for (auto x : vec) lazy::out(x, " ");
    lazy::outl();
    lazy::outl("  contains(5): ", lazy::container::contains(vec, 5));
    lazy::outl("  index_of(3): ", lazy::container::index_of(vec, 3));
    lazy::outl("  remove_first(3): ", lazy::container::remove_first(vec, 3));
    lazy::outl("  删除第一个 3 后: ");
    for (auto x : vec) lazy::out(x, " ");
    lazy::outl();
    lazy::outl("  slice(2, 5): ");
    auto sliced = lazy::container::slice(vec, 2, 5);
    for (auto x : sliced) lazy::out(x, " ");
    lazy::outl();
    lazy::outl("  is_sorted: ", lazy::container::is_sorted(vec));
    lazy::outl();

    std::map<std::string, int> m = { {"a", 1}, {"b", 2}, {"c", 3} };
    lazy::io::log_info("Map:");
    for (const auto& [k, v] : m) lazy::out(k, ":", v, " ");
    lazy::outl();
    lazy::outl("  get_or_default(\"b\", 0): ", lazy::container::get_or_default<std::string, int>(m, "b", 0));
    lazy::outl("  get_or_default(\"d\", 0): ", lazy::container::get_or_default<std::string, int>(m, "d", 0));
    lazy::outl("  contains_key(\"c\"): ", lazy::container::contains_key<std::string, int>(m, "c"));
    lazy::outl("  所有键: ");
    for (auto k : lazy::container::keys(m)) lazy::out(k, " ");
    lazy::outl();
    lazy::outl("  所有值: ");
    for (auto v : lazy::container::values(m)) lazy::out(v, " ");
    lazy::outl();
    lazy::outl();

    std::set<int> s1 = { 1, 2, 3, 4, 5 };
    std::set<int> s2 = { 4, 5, 6, 7, 8 };
    auto inter = lazy::container::intersect(s1, s2);
    auto uni = lazy::container::unite(s1, s2);
    lazy::io::log_info("集合操作:");
    lazy::outl("  交集: ");
    for (auto x : inter) lazy::out(x, " ");
    lazy::outl();
    lazy::outl("  并集: ");
    for (auto x : uni) lazy::out(x, " ");
    lazy::outl();
    lazy::outl();

    lazy::container::BiMap<std::string, int, std::string> bimap;
    bimap.insert("user1", 1001, "张三");
    bimap.insert("user2", 1002, "李四");
    lazy::io::log_info("双向映射 (BiMap):");
    lazy::outl("  get_by_key1(\"user1\"): ", bimap.get_by_key1("user1"));
    lazy::outl("  get_by_key2(1002): ", bimap.get_by_key2(1002));
    auto keys = bimap.get_keys_by_value("张三");
    lazy::outl("  get_keys_by_value(\"张三\"): (", keys.first, ", ", keys.second, ")");
    lazy::outl("  大小: ", bimap.size());
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 7. IO 功能 (lazy::io) - 彩色输出演示
    // ============================================================
    clear_screen();
    lazy::io::title("彩色输出演示", '=', 60);
    lazy::outl();

    lazy::io::log_info("这是一条信息日志");
    lazy::io::log_warn("这是一条警告日志");
    lazy::io::log_error("这是一条错误日志");
    lazy::io::log_success("这是一条成功日志");
    lazy::outl();

    // 使用 print_color 自定义颜色
    lazy::io::separator('-', 50);
    lazy::outl();

#ifdef _WIN32
    lazy::io::print_color(lazy::io::BLUE, "蓝色文字 ");
    lazy::io::print_color(lazy::io::GREEN, "绿色文字 ");
    lazy::io::print_color(lazy::io::RED, "红色文字 ");
    lazy::io::print_color(lazy::io::YELLOW, "黄色文字 ");
    lazy::io::println_color(lazy::io::WHITE, "白色文字");
#else
    lazy::io::print_color(lazy::io::color::BLUE, "蓝色文字 ");
    lazy::io::print_color(lazy::io::color::GREEN, "绿色文字 ");
    lazy::io::print_color(lazy::io::color::RED, "红色文字 ");
    lazy::io::print_color(lazy::io::color::YELLOW, "黄色文字 ");
    lazy::io::println_color(lazy::io::color::WHITE, "白色文字");
#endif
    lazy::outl();

    lazy::io::title("进度条演示", '-', 50);
    lazy::io::ProgressBar pb(100, 40);
    for (int i = 0; i <= 100; i += 5) {
        pb.update(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    lazy::outl();

    lazy::io::title("文件操作演示", '-', 50);
    try {
        lazy::io::write_file("test.txt", "你好，Lazy 库！\n这是一个测试文件。");
        auto content = lazy::io::read_file("test.txt");
        lazy::io::log_success("文件内容: ", content);

        auto lines = lazy::io::read_lines("test.txt");
        lazy::io::log_info("按行读取:");
        for (const auto& line : lines) lazy::outl("  ", line);
    }
    catch (const std::exception& e) {
        lazy::io::log_error("文件操作错误: ", e.what());
    }
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 8. 随机功能 (lazy::random)
    // ============================================================
    clear_screen();
    lazy::io::title("随机功能演示", '=', 60);
    lazy::outl();

    lazy::io::log_info("随机数生成:");
    lazy::outl("  rand_int(1, 100): ", lazy::random::rand_int(1, 100));
    lazy::outl("  rand_ll(1e9, 1e10): ", lazy::random::rand_ll(1000000000LL, 10000000000LL));
    lazy::outl("  rand_double(0, 1): ", lazy::random::rand_double(0.0, 1.0));
    lazy::outl("  rand_uniform(): ", lazy::random::rand_uniform());
    lazy::outl("  rand_normal(): ", lazy::random::rand_normal());
    lazy::outl("  rand_bool(): ", lazy::random::rand_bool());
    lazy::outl();

    lazy::io::log_info("随机字符串:");
    lazy::outl("  rand_string(12): ", lazy::random::rand_string(12));
    lazy::outl("  rand_digits(8): ", lazy::random::rand_digits(8));
    lazy::outl("  rand_hex(16): ", lazy::random::rand_hex(16));
    lazy::outl("  rand_lowercase(10): ", lazy::random::rand_lowercase(10));
    lazy::outl("  rand_uppercase(10): ", lazy::random::rand_uppercase(10));
    lazy::outl();

    lazy::io::log_info("随机数组:");
    auto rand_arr = lazy::random::rand_int_array(10, 1, 100);
    lazy::outl("  rand_int_array(10): ");
    for (auto x : rand_arr) lazy::out(x, " ");
    lazy::outl();

    lazy::io::log_info("随机采样:");
    std::vector<int> pop = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    auto sample = lazy::random::sample(pop, 5);
    lazy::outl("  sample(5) from {1..10}: ");
    for (auto x : sample) lazy::out(x, " ");
    lazy::outl();
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 9. 结构体功能 (lazy::structs)
    // ============================================================
    clear_screen();
    lazy::io::title("结构体功能演示", '=', 60);
    lazy::outl();

    lazy::structs::RangeI range(1, 10);
    lazy::io::log_info("区间 [1, 10]:");
    lazy::outl("  contains(5): ", range.contains(5));
    lazy::outl("  contains(15): ", range.contains(15));
    lazy::outl("  长度: ", range.length());
    lazy::outl("  to_string: ", range.to_string());
    lazy::outl();

    lazy::structs::Point2Di p1(1, 2), p2(4, 6);
    lazy::io::log_info("二维点:");
    lazy::outl("  p1 = ", p1.to_string(), ", p2 = ", p2.to_string());
    lazy::outl("  p1 + p2 = ", (p1 + p2).to_string());
    lazy::outl("  距离 = ", p1.distance_to(p2));
    lazy::outl("  距离平方 = ", p1.squared_distance(p2));
    lazy::outl();

    lazy::structs::RectI rect(0, 0, 10, 10);
    lazy::structs::Point2Di point(5, 5);
    lazy::io::log_info("矩形: ", rect.to_string());
    lazy::outl("  contains(5,5): ", rect.contains(point));
    lazy::outl("  面积: ", rect.area());
    lazy::outl("  中心: (", rect.center_x(), ", ", rect.center_y(), ")");
    lazy::outl();

    lazy::structs::Version version1(1, 2, 3);
    lazy::structs::Version version2(1, 2, 4);
    lazy::io::log_info("版本号:");
    lazy::outl("  version1 = ", version1.to_string());
    lazy::outl("  version2 = ", version2.to_string());
    lazy::outl("  version1 < version2: ", version1 < version2);
    lazy::outl("  version1 == version2: ", version1 == version2);
    lazy::outl("  解析 '2.0.1-alpha+123': ", lazy::structs::Version::parse("2.0.1-alpha+123").to_string());
    lazy::outl();

    auto result_ok = lazy::structs::Result<int>::ok(42);
    auto result_err = lazy::structs::Result<int>::err("出错了");
    lazy::io::log_info("Result 类型:");
    lazy::outl("  is_ok: ", result_ok.is_ok(), ", value: ", result_ok.value());
    lazy::outl("  is_err: ", result_err.is_err(), ", error: ", result_err.error());
    auto mapped = result_ok.map([](int x) { return x * 2; });
    lazy::outl("  map(*2): ", mapped.value());
    lazy::outl();

    auto root = std::make_shared<lazy::structs::TreeNode<std::string>>("根节点");
    auto child1 = std::make_shared<lazy::structs::TreeNode<std::string>>("子节点1");
    auto child2 = std::make_shared<lazy::structs::TreeNode<std::string>>("子节点2");
    auto child1_1 = std::make_shared<lazy::structs::TreeNode<std::string>>("子节点1-1");

    root->add_child(child1);
    root->add_child(child2);
    child1->add_child(child1_1);

    lazy::io::log_info("树结构:");
    root->print_pretty();
    lazy::outl("  节点数: ", root->node_count());
    lazy::outl("  高度: ", root->height());
    lazy::outl("  是根节点: ", root->is_root());
    lazy::outl("  是叶节点: ", root->is_leaf());
    lazy::outl("  to_string: ", root->to_string());
    lazy::outl("  查找 '子节点1-1': ", root->find("子节点1-1") ? "找到" : "未找到");
    lazy::outl();

    lazy::structs::Edge<int> edge(1, 2);
    lazy::structs::WeightedEdge<int> wedge(1, 2, 3.5);
    lazy::io::log_info("边:");
    lazy::outl("  边: (", edge.from, " -> ", edge.to, ")");
    lazy::outl("  带权边: (", wedge.from, " -> ", wedge.to, ", weight=", wedge.weight, ")");
    lazy::outl();

    lazy::structs::PageInfo page(2, 10, 95);
    lazy::io::log_info("分页信息: ", page.to_string());
    lazy::outl("  总页数: ", page.total_pages());
    lazy::outl("  偏移量: ", page.offset());
    lazy::outl("  有上一页: ", page.has_prev());
    lazy::outl("  有下一页: ", page.has_next());
    lazy::outl();

    lazy::structs::Error err(404, "未找到", "HTTP 处理器");
    err.details = "请求的资源不存在";
    lazy::io::log_error("错误信息: ", err.to_string());
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 10. 调试功能 (lazy::debug)
    // ============================================================
    clear_screen();
    lazy::io::title("调试功能演示", '=', 60);
    lazy::outl();

    lazy::debug::log_info("这是一条调试信息日志");
    lazy::debug::log_warning("这是一条调试警告", "附加详情");
    lazy::debug::log_error("这是一条调试错误", "错误详情");

    lazy::debug::enable_console_log();
    lazy::debug::log_info("控制台日志已启用");

    lazy::io::log_info("调试已初始化: ", lazy::debug::is_initialized());
    lazy::outl();

    wait_for_enter();

    // ============================================================
    // 11. 综合示例
    // ============================================================
    clear_screen();
    lazy::io::title("综合示例", '=', 60);
    lazy::outl();

    lazy::io::log_info("生成随机数据...");
    std::vector<int> data = lazy::random::rand_int_array(20, 1, 100);
    lazy::io::log_info("随机数据: ");
    for (auto x : data) lazy::out(x, " ");
    lazy::outl();

    auto data_mean = lazy::algorithm::mean(data);
    auto data_median = lazy::algorithm::median(data);
    auto data_stddev = lazy::algorithm::stddev(data);
    lazy::io::log_info("统计信息:");
    lazy::outl("  平均值 = ", data_mean);
    lazy::outl("  中位数 = ", data_median);
    lazy::outl("  标准差 = ", data_stddev);

    lazy::algorithm::quick_sort(data);
    lazy::io::log_success("排序后数据: ");
    for (auto x : data) lazy::out(x, " ");
    lazy::outl();

    int target = data[data.size() / 2];
    lazy::io::log_info("目标值: ", target);
    lazy::outl("  lower_bound: ", lazy::algorithm::lower_bound(data, target));
    lazy::outl("  upper_bound: ", lazy::algorithm::upper_bound(data, target));

    auto max_sum = lazy::algorithm::max_subarray_sum(data);
    lazy::outl("  最大子数组和 = ", max_sum);

    std::string data_str = lazy::container::to_string(data);
    lazy::outl("  数据转字符串: ", data_str);
    lazy::outl("  去除空格后: \"", lazy::string::trim("  " + data_str + "  "), "\"");

    lazy::outl();
    lazy::io::title("演示完成！", '=', 60);
    lazy::io::log_success("LazyCpp 所有功能已演示完毕！");
    lazy::io::log_info("请检查 crash.log 查看调试输出。");

    // 最后等待，让用户看到结果
    lazy::outl();
    lazy::io::log_info("按 Enter 键退出...");
    std::cin.get();

    return 0;
}