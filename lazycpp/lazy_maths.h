// lazy_maths.h
// copyright 2026 ? lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

// ========== 基础 ==========
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>
#include <climits>
#include <limits>
#include <type_traits>
#include <stdexcept>
#include <numeric>
#include <random>
#include <chrono>

namespace lazy {
    namespace maths {

        // ============================================================
        // 1. 类型别名
        // ============================================================
        using ll = long long;
        using ull = unsigned long long;
        using db = double;
        using ld = long double;

        // ============================================================
        // 2. 常量
        // ============================================================
        const db PI = 3.141592653589793238462643383279502884L;
        const db E = 2.718281828459045235360287471352662497L;
        const db GOLDEN_RATIO = 1.618033988749894848204586834365638117L;
        const db SQRT2 = 1.414213562373095048801688724209698078L;
        const db SQRT3 = 1.732050807568877293527446341505872366L;
        const db EPS = 1e-9;
        const db EPSILON = 1e-12;
        const int INF_INT = 1e9;
        const ll INF_LL = 4e18;
        const int MOD = 1000000007;
        const int MOD9 = 998244353;

        // ============================================================
        // 3. 基础数学函数
        // ============================================================

        // 绝对值（支持整数和浮点数）
        template<typename T>
        inline T abs_val(T x) {
            return x < 0 ? -x : x;
        }

        // 符号函数：-1, 0, 1
        template<typename T>
        inline int sign(T x) {
            return (x > 0) - (x < 0);
        }

        // 浮点数比较
        template<typename T>
        inline bool eq(T a, T b, T eps = EPS) {
            return abs_val(a - b) <= eps;
        }

        template<typename T>
        inline bool lt(T a, T b, T eps = EPS) {
            return a < b - eps;
        }

        template<typename T>
        inline bool gt(T a, T b, T eps = EPS) {
            return a > b + eps;
        }

        template<typename T>
        inline bool le(T a, T b, T eps = EPS) {
            return a <= b + eps;
        }

        template<typename T>
        inline bool ge(T a, T b, T eps = EPS) {
            return a >= b - eps;
        }

        // 取整
        inline int floor_div(int a, int b) {
            if (b < 0) { a = -a; b = -b; }
            return a >= 0 ? a / b : -((-a + b - 1) / b);
        }

        inline ll floor_div(ll a, ll b) {
            if (b < 0) { a = -a; b = -b; }
            return a >= 0 ? a / b : -((-a + b - 1) / b);
        }

        inline int ceil_div(int a, int b) {
            if (b < 0) { a = -a; b = -b; }
            return a >= 0 ? (a + b - 1) / b : -((-a) / b);
        }

        inline ll ceil_div(ll a, ll b) {
            if (b < 0) { a = -a; b = -b; }
            return a >= 0 ? (a + b - 1) / b : -((-a) / b);
        }

        // 四舍五入
        template<typename T>
        inline int round_to_int(T x) {
            return (int)(x + (x >= 0 ? 0.5 : -0.5));
        }

        // ============================================================
        // 4. 最大公约数 & 最小公倍数
        // ============================================================

        template<typename T>
        inline T gcd(T a, T b) {
            a = abs_val(a);
            b = abs_val(b);
            while (b != 0) {
                T t = a % b;
                a = b;
                b = t;
            }
            return a;
        }

        template<typename T>
        inline T lcm(T a, T b) {
            if (a == 0 || b == 0) return 0;
            return a / gcd(a, b) * b;
        }

        // 扩展欧几里得算法：返回 gcd，并计算 ax + by = gcd 的一组解
        template<typename T>
        inline T exgcd(T a, T b, T& x, T& y) {
            if (b == 0) {
                x = 1;
                y = 0;
                return a;
            }
            T x1, y1;
            T g = exgcd(b, a % b, x1, y1);
            x = y1;
            y = x1 - (a / b) * y1;
            return g;
        }

        // ============================================================
        // 5. 模运算
        // ============================================================

        // 取模（保证非负）
        template<typename T>
        inline T mod_norm(T a, T m) {
            T r = a % m;
            return r < 0 ? r + m : r;
        }

        // 快速幂取模
        template<typename T>
        inline T mod_pow(T base, T exp, T mod) {
            T result = 1 % mod;
            base %= mod;
            while (exp > 0) {
                if (exp & 1) {
                    result = (result * base) % mod;
                }
                base = (base * base) % mod;
                exp >>= 1;
            }
            return result;
        }

        // 模逆元（需要 gcd(a, m) = 1）
        template<typename T>
        inline T mod_inv(T a, T m) {
            T x, y;
            T g = exgcd(a, m, x, y);
            if (g != 1) {
                throw std::runtime_error("mod_inv: inverse does not exist");
            }
            return mod_norm(x, m);
        }

        // 中国剩余定理（CRT）：x ≡ a_i (mod m_i)
        template<typename T>
        inline T crt(const std::vector<T>& a, const std::vector<T>& m) {
            if (a.size() != m.size() || a.empty()) {
                throw std::invalid_argument("CRT: empty or mismatched input");
            }

            T x = 0;
            T M = 1;
            for (size_t i = 0; i < m.size(); i++) {
                T mi = m[i];
                T ai = mod_norm(a[i], mi);
                T ti = mod_inv(M % mi, mi);
                x += ai * M * ti;
                M *= mi;
                x %= M;
            }
            return x % M;
        }

        // ============================================================
        // 6. 素数相关
        // ============================================================

        // 传统 O(√n) 判定
        inline bool is_prime_traditional(ull n) {
            if (n < 2) return false;
            if (n == 2 || n == 3) return true;
            if (n % 2 == 0 || n % 3 == 0) return false;

            ull limit = (ull)std::sqrt(n);
            for (ull i = 5; i <= limit; i += 6) {
                if (n % i == 0 || n % (i + 2) == 0) return false;
            }
            return true;
        }

        // Miller-Rabin 素性测试
        inline bool miller_rabin(ull n) {
            if (n < 2) return false;
            for (ull p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
                if (n % p == 0) return n == p;
            }

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
                    x = (x * x) % n;
                    if (x == n - 1) {
                        composite = false;
                        break;
                    }
                }
                if (composite) return false;
            }
            return true;
        }

        // 统一接口
        inline bool is_prime(ull n, const std::string& mode = "m-r") {
            if (mode == "easy") {
                return is_prime_traditional(n);
            }
            else if (mode == "m-r") {
                return miller_rabin(n);
            }
            else {
                throw std::invalid_argument("is_prime: unknown mode '" + mode + "'");
            }
        }

        // 埃氏筛
        inline std::vector<bool> sieve(int n) {
            std::vector<bool> is_prime_vec(n + 1, true);
            if (n >= 0) is_prime_vec[0] = false;
            if (n >= 1) is_prime_vec[1] = false;

            for (int i = 2; i * i <= n; i++) {
                if (is_prime_vec[i]) {
                    for (int j = i * i; j <= n; j += i) {
                        is_prime_vec[j] = false;
                    }
                }
            }
            return is_prime_vec;
        }

        // 质数列表（埃氏筛）
        inline std::vector<int> prime_list(int n) {
            std::vector<bool> is_prime_vec = sieve(n);
            std::vector<int> primes;
            for (int i = 2; i <= n; i++) {
                if (is_prime_vec[i]) primes.push_back(i);
            }
            return primes;
        }

        // ============================================================
        // 7. 因数分解
        // ============================================================

        // 质因数分解（返回 {质因数, 指数}）
        inline std::vector<std::pair<ull, int>> prime_factorize(ull n) {
            std::vector<std::pair<ull, int>> factors;
            for (ull p : {2ULL, 3ULL, 5ULL}) {
                if (n % p == 0) {
                    int cnt = 0;
                    while (n % p == 0) { n /= p; cnt++; }
                    factors.push_back({ p, cnt });
                }
            }

            for (ull i = 7; i * i <= n; i += 30) {
                for (ull j : {0ULL, 4ULL, 6ULL, 10ULL, 12ULL, 16ULL, 22ULL, 24ULL}) {
                    ull p = i + j;
                    if (p * p > n) break;
                    if (n % p == 0) {
                        int cnt = 0;
                        while (n % p == 0) { n /= p; cnt++; }
                        factors.push_back({ p, cnt });
                    }
                }
            }
            if (n > 1) factors.push_back({ n, 1 });
            return factors;
        }

        // 约数个数
        inline ull divisor_count(ull n) {
            auto factors = prime_factorize(n);
            ull count = 1;
            for (auto& [p, e] : factors) {
                count *= (e + 1);
            }
            return count;
        }

        // 所有约数
        inline std::vector<ull> divisors(ull n) {
            std::vector<ull> result;
            for (ull i = 1; i * i <= n; i++) {
                if (n % i == 0) {
                    result.push_back(i);
                    if (i * i != n) result.push_back(n / i);
                }
            }
            std::sort(result.begin(), result.end());
            return result;
        }

        // ============================================================
        // 8. 回文数
        // ============================================================

        inline bool is_palindrome(ull n) {
            if (n < 10) return true;
            if (n % 10 == 0) return false;

            ull rev = 0;
            while (n > rev) {
                rev = rev * 10 + n % 10;
                n /= 10;
            }
            return n == rev || n == rev / 10;
        }

        inline bool is_palindrome(const std::string& s) {
            int l = 0, r = (int)s.length() - 1;
            while (l < r) {
                if (s[l] != s[r]) return false;
                l++; r--;
            }
            return true;
        }

        // ============================================================
        // 9. 排列组合
        // ============================================================

        // 阶乘
        template<typename T>
        inline T factorial(T n) {
            T result = 1;
            for (T i = 2; i <= n; i++) {
                result *= i;
            }
            return result;
        }

        // 排列数 P(n, k)
        template<typename T>
        inline T perm(T n, T k) {
            if (k < 0 || k > n) return 0;
            T result = 1;
            for (T i = n - k + 1; i <= n; i++) {
                result *= i;
            }
            return result;
        }

        // 组合数 C(n, k)（使用递推，适用于 n 较小）
        template<typename T>
        inline T comb_small(T n, T k) {
            if (k < 0 || k > n) return 0;
            if (k > n - k) k = n - k;
            T result = 1;
            for (T i = 1; i <= k; i++) {
                result = result * (n - k + i) / i;
            }
            return result;
        }

        // 组合数（使用 DP 表，适用于多次查询）
        class Comb {
        private:
            std::vector<std::vector<ll>> C;
            int max_n;
        public:
            Comb(int n) : max_n(n) {
                C.resize(n + 1, std::vector<ll>(n + 1, 0));
                for (int i = 0; i <= n; i++) {
                    C[i][0] = C[i][i] = 1;
                    for (int j = 1; j < i; j++) {
                        C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
                    }
                }
            }

            ll get(int n, int k) const {
                if (k < 0 || k > n || n > max_n) return 0;
                return C[n][k];
            }
        };

        // ============================================================
        // 10. 组合数学（模意义）
        // ============================================================

        class CombMod {
        private:
            std::vector<ll> fact, inv_fact;
            ll mod;

            ll mod_pow_ll(ll a, ll e) {
                ll res = 1;
                while (e > 0) {
                    if (e & 1) res = (res * a) % mod;
                    a = (a * a) % mod;
                    e >>= 1;
                }
                return res;
            }

        public:
            CombMod(int n, ll m = MOD) : mod(m) {
                fact.resize(n + 1);
                inv_fact.resize(n + 1);
                fact[0] = 1;
                for (int i = 1; i <= n; i++) {
                    fact[i] = (fact[i - 1] * i) % mod;
                }
                inv_fact[n] = mod_pow_ll(fact[n], mod - 2);
                for (int i = n; i >= 1; i--) {
                    inv_fact[i - 1] = (inv_fact[i] * i) % mod;
                }
            }

            ll comb(int n, int k) const {
                if (k < 0 || k > n) return 0;
                return fact[n] * inv_fact[k] % mod * inv_fact[n - k] % mod;
            }

            ll perm(int n, int k) const {
                if (k < 0 || k > n) return 0;
                return fact[n] * inv_fact[n - k] % mod;
            }

            ll factorial(int n) const {
                if (n < 0) return 0;
                return fact[n];
            }
        };

        // ============================================================
        // 11. 矩阵
        // ============================================================

        template<typename T>
        struct Matrix {
            int rows, cols;
            std::vector<std::vector<T>> data;

            Matrix() : rows(0), cols(0) {}

            Matrix(int r, int c, T val = 0) : rows(r), cols(c) {
                data.assign(r, std::vector<T>(c, val));
            }

            Matrix(const std::vector<std::vector<T>>& mat) {
                data = mat;
                rows = mat.size();
                cols = rows > 0 ? mat[0].size() : 0;
            }

            static Matrix identity(int n) {
                Matrix I(n, n, 0);
                for (int i = 0; i < n; i++) I.data[i][i] = 1;
                return I;
            }

            Matrix<T> operator+(const Matrix<T>& other) const {
                if (rows != other.rows || cols != other.cols) {
                    throw std::invalid_argument("Matrix dimensions mismatch");
                }
                Matrix result(rows, cols);
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        result.data[i][j] = data[i][j] + other.data[i][j];
                    }
                }
                return result;
            }

            Matrix<T> operator*(const Matrix<T>& other) const {
                if (cols != other.rows) {
                    throw std::invalid_argument("Matrix dimensions mismatch");
                }
                Matrix result(rows, other.cols, 0);
                for (int i = 0; i < rows; i++) {
                    for (int k = 0; k < cols; k++) {
                        if (data[i][k] == 0) continue;
                        for (int j = 0; j < other.cols; j++) {
                            result.data[i][j] += data[i][k] * other.data[k][j];
                        }
                    }
                }
                return result;
            }

            Matrix<T> operator%(T mod) const {
                Matrix result(rows, cols);
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        result.data[i][j] = data[i][j] % mod;
                    }
                }
                return result;
            }

            // 矩阵快速幂
            Matrix<T> pow(int exp, T mod = 0) const {
                if (rows != cols) {
                    throw std::invalid_argument("Matrix must be square for pow");
                }
                Matrix result = identity(rows);
                Matrix base = *this;

                while (exp > 0) {
                    if (exp & 1) {
                        result = (mod > 0) ? ((result * base) % mod) : (result * base);
                    }
                    base = (mod > 0) ? ((base * base) % mod) : (base * base);
                    exp >>= 1;
                }
                return result;
            }

            // 转置
            Matrix<T> transpose() const {
                Matrix result(cols, rows);
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        result.data[j][i] = data[i][j];
                    }
                }
                return result;
            }

            // 打印
            void print() const {
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        std::cout << data[i][j] << " ";
                    }
                    std::cout << "\n";
                }
            }
        };

        // 矩阵乘法（模版特化，模意义）
        template<typename T>
        inline Matrix<T> mat_mul_mod(const Matrix<T>& A, const Matrix<T>& B, T mod) {
            if (A.cols != B.rows) {
                throw std::invalid_argument("Matrix dimensions mismatch");
            }
            Matrix<T> result(A.rows, B.cols, 0);
            for (int i = 0; i < A.rows; i++) {
                for (int k = 0; k < A.cols; k++) {
                    if (A.data[i][k] == 0) continue;
                    for (int j = 0; j < B.cols; j++) {
                        result.data[i][j] = (result.data[i][j] + A.data[i][k] * B.data[k][j]) % mod;
                    }
                }
            }
            return result;
        }

        // ============================================================
        // 12. 斐波那契数列
        // ============================================================

        // 使用矩阵快速幂计算第 n 个斐波那契数（模意义）
        template<typename T>
        inline T fib_matrix(T n, T mod = 0) {
            if (n == 0) return 0;
            if (n == 1) return 1;

            Matrix<T> F({ {1, 1}, {1, 0} });
            auto result = F.pow((int)n - 1, mod);
            return mod > 0 ? result.data[0][0] % mod : result.data[0][0];
        }

        // 传统递推
        template<typename T>
        inline T fib_iter(T n) {
            if (n == 0) return 0;
            if (n == 1) return 1;
            T a = 0, b = 1;
            for (T i = 2; i <= n; i++) {
                T c = a + b;
                a = b;
                b = c;
            }
            return b;
        }

        // ============================================================
        // 13. 随机数
        // ============================================================

		// lazy_random有更好的实现，建议使用lazy_random
        inline int randint(int min, int max) {
            static std::mt19937 gen(
                (unsigned)std::chrono::steady_clock::now().time_since_epoch().count()
            );
            std::uniform_int_distribution<> dis(min, max);
            return dis(gen);
        }

        inline double randdouble(double min, double max) {
            static std::mt19937 gen(
                (unsigned)std::chrono::steady_clock::now().time_since_epoch().count()
            );
            std::uniform_real_distribution<> dis(min, max);
            return dis(gen);
        }

        // ============================================================
        // 14. 线性代数工具
        // ============================================================

        // 向量点积
        template<typename T>
        inline T dot(const std::vector<T>& a, const std::vector<T>& b) {
            if (a.size() != b.size()) {
                throw std::invalid_argument("Vector dimensions mismatch");
            }
            T result = 0;
            for (size_t i = 0; i < a.size(); i++) {
                result += a[i] * b[i];
            }
            return result;
        }

        // 向量二范数
        template<typename T>
        inline double norm(const std::vector<T>& v) {
            T sum = 0;
            for (const auto& x : v) {
                sum += x * x;
            }
            return std::sqrt((double)sum);
        }

        // 向量归一化
        template<typename T>
        inline std::vector<double> normalize(const std::vector<T>& v) {
            double n = norm(v);
            if (n < EPS) return std::vector<double>(v.size(), 0);
            std::vector<double> result(v.size());
            for (size_t i = 0; i < v.size(); i++) {
                result[i] = (double)v[i] / n;
            }
            return result;
        }

        // ============================================================
        // 15. 进制转换
        // ============================================================

        // 十进制转任意进制（2-36）
        inline std::string to_base(ull n, int base) {
            if (base < 2 || base > 36) {
                throw std::invalid_argument("Base must be between 2 and 36");
            }
            if (n == 0) return "0";

            const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            std::string result;
            while (n > 0) {
                result = digits[n % base] + result;
                n /= base;
            }
            return result;
        }

        // 任意进制转十进制
        inline ull from_base(const std::string& s, int base) {
            if (base < 2 || base > 36) {
                throw std::invalid_argument("Base must be between 2 and 36");
            }
            ull result = 0;
            for (char c : s) {
                int digit;
                if (c >= '0' && c <= '9') {
                    digit = c - '0';
                }
                else if (c >= 'A' && c <= 'Z') {
                    digit = c - 'A' + 10;
                }
                else if (c >= 'a' && c <= 'z') {
                    digit = c - 'a' + 10;
                }
                else {
                    throw std::invalid_argument("Invalid character in input");
                }
                if (digit >= base) {
                    throw std::invalid_argument("Digit out of range for base");
                }
                result = result * base + digit;
            }
            return result;
        }
#ifdef LAZY_EXPORT_ALL
        using namespace maths;
#endif
    } // namespace maths
}//namespace lazy