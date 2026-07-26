// lazy_algorithm.h
// copyright 2026 ? lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <vector>
#include <algorithm>
#include <numeric>

namespace lazy {
    namespace algorithm {

        // ============================================================
        // 二分查找
        // ============================================================

        template<typename T>
        int lower_bound(const std::vector<T>& arr, T target) {
            int l = 0, r = arr.size();
            while (l < r) {
                int mid = l + (r - l) / 2;
                if (arr[mid] < target) l = mid + 1;
                else r = mid;
            }
            return l;
        }

        template<typename T>
        int upper_bound(const std::vector<T>& arr, T target) {
            int l = 0, r = arr.size();
            while (l < r) {
                int mid = l + (r - l) / 2;
                if (arr[mid] <= target) l = mid + 1;
                else r = mid;
            }
            return l;
        }

        // ============================================================
        // 反转
        // ============================================================

        template<typename T>
        void reverse(T& arr) {
            std::reverse(arr.begin(), arr.end());
        }

        // ============================================================
        // 最大值/最小值
        // ============================================================

        template<typename T>
        T maxof(const std::vector<T>& arr) {
            return *std::max_element(arr.begin(), arr.end());
        }

        template<typename T>
        T minof(const std::vector<T>& arr) {
            return *std::min_element(arr.begin(), arr.end());
        }

        // ============================================================
        // 去重
        // ============================================================

        template<typename T>
        std::vector<T> unique(const std::vector<T>& arr) {
            std::vector<T> result = arr;
            std::sort(result.begin(), result.end());
            result.erase(std::unique(result.begin(), result.end()), result.end());
            return result;
        }

        // ============================================================
        // 前缀和
        // ============================================================

        template<typename T>
        std::vector<T> prefix_sum(const std::vector<T>& arr) {
            std::vector<T> pref(arr.size() + 1, 0);
            for (size_t i = 0; i < arr.size(); i++) {
                pref[i + 1] = pref[i] + arr[i];
            }
            return pref;
        }

        // 区间和 [l, r] (0-index)
        template<typename T>
        T range_sum(const std::vector<T>& pref, int l, int r) {
            return pref[r + 1] - pref[l];
        }

    } // namespace algorithm

    //using namespace algorithm;
#ifdef LAZY_EXPORT_ALL
    using namespace algorithm;
#endif
} // namespace lazy