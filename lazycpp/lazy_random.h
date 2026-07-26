// lazy_random.h
// copyright 2026 ? lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <random>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>

namespace lazy {
    namespace random {

        // ============================================================
        // 随机数生成器（单例）
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
        // 整数随机
        // ============================================================

        // 随机整数 [min, max]
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

        // ============================================================
        // 浮点数随机
        // ============================================================

        // 随机浮点数 [min, max]
        inline double rand_double(double min, double max) {
            std::uniform_real_distribution<double> dist(min, max);
            return dist(RNG::instance().engine());
        }

        inline float rand_float(float min, float max) {
            std::uniform_real_distribution<float> dist(min, max);
            return dist(RNG::instance().engine());
        }

        // 随机 [0, 1) 浮点数
        inline double rand_uniform() {
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            return dist(RNG::instance().engine());
        }

        // 正态分布
        inline double rand_normal(double mean = 0.0, double stddev = 1.0) {
            std::normal_distribution<double> dist(mean, stddev);
            return dist(RNG::instance().engine());
        }

        // ============================================================
        // 布尔随机
        // ============================================================

        inline bool rand_bool(double probability = 0.5) {
            return rand_uniform() < probability;
        }

        // ============================================================
        // 容器随机
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

        // 随机打乱容器
        template<typename Container>
        void shuffle(Container& c) {
            std::shuffle(c.begin(), c.end(), RNG::instance().engine());
        }

        // 随机采样（不放回）
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

        // ============================================================
        // 随机字符串
        // ============================================================

        // 随机字符串（指定字符集）
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

        // 随机数字字符串
        inline std::string rand_digits(int length) {
            return rand_string(length, "0123456789");
        }

        // 随机字母字符串
        inline std::string rand_letters(int length) {
            return rand_string(length, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        }

        // 随机小写字母字符串
        inline std::string rand_lowercase(int length) {
            return rand_string(length, "abcdefghijklmnopqrstuvwxyz");
        }

        // 随机大写字母字符串
        inline std::string rand_uppercase(int length) {
            return rand_string(length, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        }

        // 随机十六进制字符串
        inline std::string rand_hex(int length) {
            return rand_string(length, "0123456789ABCDEF");
        }

        // ============================================================
        // 随机分布生成器
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

        // 生成随机排列（0 到 n-1）
        inline std::vector<int> rand_permutation(int n) {
            std::vector<int> result(n);
            std::iota(result.begin(), result.end(), 0);
            shuffle(result);
            return result;
        }

        // ============================================================
        // 分布统计
        // ============================================================

        // 生成符合正态分布的随机数数组
        inline std::vector<double> rand_normal_array(int size, double mean = 0.0, double stddev = 1.0) {
            std::vector<double> result(size);
            for (int i = 0; i < size; i++) {
                result[i] = rand_normal(mean, stddev);
            }
            return result;
        }

        // 生成指数分布
        inline double rand_exponential(double lambda) {
            std::exponential_distribution<double> dist(lambda);
            return dist(RNG::instance().engine());
        }

        // 生成泊松分布
        inline int rand_poisson(double mean) {
            std::poisson_distribution<int> dist(mean);
            return dist(RNG::instance().engine());
        }

    } // namespace random

    //using namespace random;
    //不规范
#ifdef LAZY_EXPORT_ALL
    using namespace random;
#endif
} // namespace lazy