// lazy_random.h
// copyright 2026 lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <random>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <unordered_set>
#include <type_traits>
#include <cmath>

namespace lazy {
    namespace random {

        // ============================================================
        // 随机数生成器
        // ============================================================

        class RNG {
        private:
            std::mt19937_64 gen;

            RNG() {
                gen.seed(get_seed());
            }

            unsigned long long get_seed() {
                return (unsigned long long)std::chrono::steady_clock::now()
                    .time_since_epoch().count();
            }

        public:
            static RNG& instance() {
                static RNG rng;
                return rng;
            }

            std::mt19937_64& engine() {
                return gen;
            }

            void reseed() {
                gen.seed(get_seed());
            }

            void reseed(unsigned long long seed) {
                gen.seed(seed);
            }
        };

        // ============================================================
        // 整数随机（扩展）
        // ============================================================

        
        inline int rand_int(int min, int max) {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(RNG::instance().engine());
        }

        inline long long rand_ll(long long min, long long max) {
            std::uniform_int_distribution<long long> dist(min, max);
            return dist(RNG::instance().engine());
        }

        inline unsigned int rand_uint(unsigned int min, unsigned int max) {
            std::uniform_int_distribution<unsigned int> dist(min, max);
            return dist(RNG::instance().engine());
        }

        // size_t 类型
        inline size_t rand_size(size_t min, size_t max) {
            std::uniform_int_distribution<size_t> dist(min, max);
            return dist(RNG::instance().engine());
        }

        // 带权重的随机整数（权重数组）
        template<typename T>
        int rand_weighted_int(const std::vector<T>& weights) {
            if (weights.empty()) {
                throw std::runtime_error("Weights vector is empty");
            }
            T sum = std::accumulate(weights.begin(), weights.end(), T(0));
            if (sum <= 0) {
                throw std::runtime_error("Sum of weights must be positive");
            }
            std::uniform_real_distribution<double> dist(0.0, (double)sum);
            double r = dist(RNG::instance().engine());
            T cumulative = 0;
            for (size_t i = 0; i < weights.size(); ++i) {
                cumulative += weights[i];
                if (r < cumulative) {
                    return (int)i;
                }
            }
            return (int)weights.size() - 1;
        }

        // 生成不重复的随机整数序列
        std::vector<int> rand_unique_ints(int min, int max, int count) {
            if (count > (max - min + 1)) {
                throw std::runtime_error("Cannot generate more unique numbers than range size");
            }
            std::vector<int> pool(max - min + 1);
            std::iota(pool.begin(), pool.end(), min);
            std::shuffle(pool.begin(), pool.end(), RNG::instance().engine());
            pool.resize(count);
            return pool;
        }

        // ============================================================
        // 浮点数随机（扩展）
        // ============================================================

        
        inline double rand_double(double min, double max) {
            std::uniform_real_distribution<double> dist(min, max);
            return dist(RNG::instance().engine());
        }

        inline float rand_float(float min, float max) {
            std::uniform_real_distribution<float> dist(min, max);
            return dist(RNG::instance().engine());
        }

        inline double rand_uniform() {
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            return dist(RNG::instance().engine());
        }

        inline double rand_normal(double mean = 0.0, double stddev = 1.0) {
            std::normal_distribution<double> dist(mean, stddev);
            return dist(RNG::instance().engine());
        }

        // 对数正态分布
        inline double rand_lognormal(double mean = 0.0, double stddev = 1.0) {
            std::lognormal_distribution<double> dist(mean, stddev);
            return dist(RNG::instance().engine());
        }

        // 伽马分布
        inline double rand_gamma(double alpha, double beta = 1.0) {
            std::gamma_distribution<double> dist(alpha, beta);
            return dist(RNG::instance().engine());
        }

        // 卡方分布
        inline double rand_chi_squared(double df) {
            std::chi_squared_distribution<double> dist(df);
            return dist(RNG::instance().engine());
        }

        // 学生T分布
        inline double rand_student_t(double df) {
            std::student_t_distribution<double> dist(df);
            return dist(RNG::instance().engine());
        }

        // F分布
        inline double rand_fisher_f(double dfn, double dfd) {
            std::fisher_f_distribution<double> dist(dfn, dfd);
            return dist(RNG::instance().engine());
        }

        // ============================================================
        // 布尔随机（扩展）
        // ============================================================

        inline bool rand_bool(double probability = 0.5) {
            return rand_uniform() < probability;
        }

        // 返回 true/false 交替序列（用于测试）
        inline std::vector<bool> rand_bool_sequence(int length, double probability = 0.5) {
            std::vector<bool> result(length);
            for (int i = 0; i < length; ++i) {
                result[i] = rand_bool(probability);
            }
            return result;
        }

        // ============================================================
        // 容器随机（扩展）
        // ============================================================

		// 随机选择容器中的一个元素
        template<typename Container>
        auto rand_choice(const Container& c) -> decltype(c[0]) {
            if (c.empty()) {
                throw std::runtime_error("Cannot choose from empty container");
            }
            int idx = rand_int(0, (int)c.size() - 1);
            return c[idx];
        }

		// 随机选择容器中的多个元素（不放回）
        template<typename Container>
        void shuffle(Container& c) {
            std::shuffle(c.begin(), c.end(), RNG::instance().engine());
        }

		// 随机选择容器中的多个元素（不放回）
        template<typename T>
        std::vector<T> sample(const std::vector<T>& population, int k) {
            if (k > (int)population.size()) {
                throw std::runtime_error("Sample size larger than population");
            }
            std::vector<T> result = population;
            shuffle(result);
            result.resize(k);
            return result;
        }

        // 带权重的随机选择
        template<typename T>
        T rand_weighted_choice(const std::vector<T>& items, const std::vector<double>& weights) {
            if (items.size() != weights.size()) {
                throw std::runtime_error("Items and weights must have same size");
            }
            if (items.empty()) {
                throw std::runtime_error("Cannot choose from empty container");
            }
            int idx = rand_weighted_int(weights);
            return items[idx];
        }

        // 随机选择多个元素（放回）
        template<typename T>
        std::vector<T> sample_with_replacement(const std::vector<T>& population, int k) {
            if (population.empty()) {
                throw std::runtime_error("Population is empty");
            }
            std::vector<T> result;
            result.reserve(k);
            for (int i = 0; i < k; ++i) {
                result.push_back(population[rand_int(0, (int)population.size() - 1)]);
            }
            return result;
        }

        // 随机分区
        template<typename Container>
        std::pair<Container, Container> random_split(Container& c, double ratio = 0.7) {
            if (ratio < 0 || ratio > 1) {
                throw std::runtime_error("Ratio must be between 0 and 1");
            }
            shuffle(c);
            size_t split_idx = (size_t)(c.size() * ratio);
            Container first, second;
            auto it = c.begin();
            for (size_t i = 0; i < split_idx && it != c.end(); ++i, ++it) {
                first.push_back(*it);
            }
            for (; it != c.end(); ++it) {
                second.push_back(*it);
            }
            return { first, second };
        }

        // ============================================================
        // 随机字符串（扩展）
        // ============================================================
        
		// 生成随机字符串
        inline std::string rand_string(int length,
            const std::string& chars =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") {
            std::string result;
            result.reserve(length);
            for (int i = 0; i < length; i++) {
                result += chars[rand_int(0, (int)chars.size() - 1)];
            }
            return result;
        }

		// 生成随机字母数字字符串
        inline std::string rand_digits(int length) {
            return rand_string(length, "0123456789");
        }

		// 生成随机字母字符串
        inline std::string rand_letters(int length) {
            return rand_string(length, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        }

		// 生成随机小写字母字符串
        inline std::string rand_lowercase(int length) {
            return rand_string(length, "abcdefghijklmnopqrstuvwxyz");
        }

		// 生成随机大写字母字符串
        inline std::string rand_uppercase(int length) {
            return rand_string(length, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        }

		//  生成随机十六进制字符串
        inline std::string rand_hex(int length) {
            return rand_string(length, "0123456789ABCDEF");
        }

        // 随机安全密码（包含特殊字符）
        inline std::string rand_password(int length) {
            const std::string chars =
                "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>?";
            return rand_string(length, chars);
        }

        // 随机UUID（v4格式）
        inline std::string rand_uuid() {
            std::string uuid = rand_hex(8) + "-" + rand_hex(4) + "-4" + rand_hex(3) +
                "-" + "89ab"[rand_int(0, 3)] + rand_hex(3) + "-" + rand_hex(12);
            return uuid;
        }

        // 随机单词（从内置词库中选择，这里用字母组合代替）
        inline std::string rand_word(int min_length = 3, int max_length = 8) {
            int length = rand_int(min_length, max_length);
            return rand_lowercase(length);
        }

        // 随机句子
        inline std::string rand_sentence(int word_count = 5, int min_word_len = 3, int max_word_len = 8) {
            std::vector<std::string> words;
            for (int i = 0; i < word_count; ++i) {
                words.push_back(rand_word(min_word_len, max_word_len));
            }
            // 首字母大写
            words[0][0] = std::toupper(words[0][0]);
            std::string sentence = std::accumulate(words.begin(), words.end(), std::string(),
                [](const std::string& a, const std::string& b) -> std::string {
                    return a.empty() ? b : a + " " + b;
                });
            return sentence + ".";
        }

        // ============================================================
        // 随机数组/容器（扩展）
        // ============================================================

		// 生成随机整数数组
        inline std::vector<int> rand_int_array(int size, int min, int max) {
            std::vector<int> result(size);
            for (int i = 0; i < size; i++) {
                result[i] = rand_int(min, max);
            }
            return result;
        }

		// 生成随机浮点数数组
        inline std::vector<double> rand_double_array(int size, double min, double max) {
            std::vector<double> result(size);
            for (int i = 0; i < size; i++) {
                result[i] = rand_double(min, max);
            }
            return result;
        }

		// 生成随机排列（0到n-1的随机顺序）
        inline std::vector<int> rand_permutation(int n) {
            std::vector<int> result(n);
            std::iota(result.begin(), result.end(), 0);
            shuffle(result);
            return result;
        }

        // 生成随机矩阵（二维数组）
        inline std::vector<std::vector<int>> rand_matrix(int rows, int cols, int min, int max) {
            std::vector<std::vector<int>> result(rows, std::vector<int>(cols));
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result[i][j] = rand_int(min, max);
                }
            }
            return result;
        }

        // 生成随机集合
        template<typename T>
        std::set<T> rand_set(int size, const std::vector<T>& pool) {
            if (size > (int)pool.size()) {
                throw std::runtime_error("Cannot generate set larger than pool");
            }
            std::vector<T> shuffled = sample(pool, size);
            return std::set<T>(shuffled.begin(), shuffled.end());
        }

        // ============================================================
        // 分布统计（扩展）
        // ============================================================

		// 生成正态分布数组
        inline std::vector<double> rand_normal_array(int size, double mean = 0.0, double stddev = 1.0) {
            std::vector<double> result(size);
            for (int i = 0; i < size; i++) {
                result[i] = rand_normal(mean, stddev);
            }
            return result;
        }

		// 指数分布
        inline double rand_exponential(double lambda) {
            std::exponential_distribution<double> dist(lambda);
            return dist(RNG::instance().engine());
        }

		// 泊松分布
        inline int rand_poisson(double mean) {
            std::poisson_distribution<int> dist(mean);
            return dist(RNG::instance().engine());
        }

        // 随机游走（布朗运动）
        inline std::vector<double> rand_walk(int steps, double mean = 0.0, double stddev = 1.0) {
            std::vector<double> result(steps + 1, 0.0);
            for (int i = 1; i <= steps; ++i) {
                result[i] = result[i - 1] + rand_normal(mean, stddev);
            }
            return result;
        }

        // 随机排列（全排列中的随机一个）
        template<typename Container>
        Container random_permutation_of(const Container& c) {
            Container result = c;
            shuffle(result);
            return result;
        }

        // 生成随机概率向量（和为1）
        inline std::vector<double> rand_probability_vector(int size) {
            std::vector<double> result(size);
            double sum = 0;
            for (int i = 0; i < size; ++i) {
                result[i] = rand_uniform();
                sum += result[i];
            }
            for (int i = 0; i < size; ++i) {
                result[i] /= sum;
            }
            return result;
        }

        // ============================================================
        // 实用工具
        // ============================================================

        // 随机日期（只返回天数偏移）
        inline int rand_days_offset(int min_days = -365, int max_days = 365) {
            return rand_int(min_days, max_days);
        }

        // 随机枚举值（需要枚举是连续的）
        template<typename Enum>
        Enum rand_enum(Enum min, Enum max) {
            return static_cast<Enum>(rand_int(static_cast<int>(min), static_cast<int>(max)));
        }

        // 打乱并返回新容器（不修改原容器）
        template<typename Container>
        Container shuffled_copy(const Container& c) {
            Container result = c;
            shuffle(result);
            return result;
        }

        // 二项分布
        inline int rand_binomial(int trials, double probability) {
            std::binomial_distribution<int> dist(trials, probability);
            return dist(RNG::instance().engine());
        }

        // 几何分布
        inline int rand_geometric(double probability) {
            std::geometric_distribution<int> dist(probability);
            return dist(RNG::instance().engine());
        }

        // 负二项分布
        inline int rand_negative_binomial(int successes, double probability) {
            std::negative_binomial_distribution<int> dist(successes, probability);
            return dist(RNG::instance().engine());
        }

        // 韦伯分布
        inline double rand_weibull(double a, double b) {
            std::weibull_distribution<double> dist(a, b);
            return dist(RNG::instance().engine());
        }

        // 极值分布
        inline double rand_extreme_value(double a, double b) {
            std::extreme_value_distribution<double> dist(a, b);
            return dist(RNG::instance().engine());
        }

        // ============================================================
        // 随机种子管理
        // ============================================================

        // 使用固定种子（用于可复现测试）
        inline void set_fixed_seed(unsigned long long seed = 123456789) {
            RNG::instance().reseed(seed);
        }

        // 使用随机种子（默认行为）
        inline void set_random_seed() {
            RNG::instance().reseed();
        }

        // ============================================================
        // 简化别名
        // ============================================================

        // 常用的简短别名
        inline int rint(int min, int max) { return rand_int(min, max); }
        inline double rdouble(double min, double max) { return rand_double(min, max); }
        inline double runiform() { return rand_uniform(); }
        inline bool rbool(double p = 0.5) { return rand_bool(p); }
        template<typename T>
        auto rchoice(const T& c) -> decltype(c[0]) { return rand_choice(c); }
        template<typename Container>
        void rshuffle(Container& c) { shuffle(c); }
        inline std::string rstr(int len) { return rand_string(len); }

    } // namespace random

#ifdef LAZY_EXPORT_ALL
    using namespace random;
#endif

} // namespace lazy