// lazy.h
// copyright 2026 ? lazycpp MIT License
//
// 
// 使用方式：
//   1. 安全模式：#include "lazy.h"
//      使用 lazy::xxx::func() 完全限定
//
//   2. 便捷模式（小心使用）：
//      #define LAZY_EXPORT_ALL
//      #include "lazy.h"
//      然后可以使用 lazy::func()（无需子命名空间）
//
//   3. 局部便捷模式（推荐）：
//      #include "lazy.h"
//      void func() {
//          LAZY_USE_ALL;  // 仅在当前作用域导出
//          // 可以使用所有功能而无需前缀
//      }

#define _CRT_SECURE_NO_WARNINGS
#pragma once

// ========== 配置 ==========
#include "lazy_config.h"

// ========== 所有子模块 ==========
#include "lazy_core.h"       // lazy::core
#include "lazy_maths.h"      // lazy::maths
#include "lazy_debug.h"      // lazy::debug
#include "lazy_algorithm.h"  // lazy::algorithm
#include "lazy_container.h"  // lazy::container
#include "lazy_string.h"     // lazy::string
#include "lazy_io.h"         // lazy::io
#include "lazy_random.h"     // lazy::random

// ========== 便捷宏 ==========

// 快速 IO（不污染命名空间）
#define LAZY_FAST_IO() \
    lazy::io::fast_io()

// 完整初始化
#define LAZY_INIT() \
    lazy::io::fast_io(); \
    lazy::debug::init("crash.log")

// ========== 局部导出宏（推荐使用） ==========

// 在局部作用域中使用，将所有子命名空间导出到当前作用域
// 用法：在函数或代码块内使用 LAZY_USE_ALL
#define LAZY_USE_ALL \
    using namespace lazy::core; \
    using namespace lazy::maths; \
    using namespace lazy::container; \
    using namespace lazy::string; \
    using namespace lazy::io; \
    using namespace lazy::random; \
    using namespace lazy::algorithm; \
    using namespace lazy::debug

// ========== 全局导出（仅当 LAZY_EXPORT_ALL=1） ==========

#ifdef LAZY_EXPORT_ALL
    // ?? 警告：将子命名空间导出到 lazy 命名空间
    // 这会污染 lazy 命名空间，但不会污染全局命名空间
    // 用户仍然需要 using namespace lazy 才能获得所有符号

namespace lazy {
    // 从 core 导出
    using namespace core;
    using namespace maths;
    using namespace container;
    using namespace string;
    using namespace io;
    using namespace random;
    using namespace algorithm;
    using namespace debug;
}

// 如果你想让所有符号直接在全局可用（极度危险！），可以额外定义：
// #define LAZY_GLOBAL_EXPORT
#ifdef LAZY_GLOBAL_EXPORT
#pragma message("?? WARNING: LAZY_GLOBAL_EXPORT - global namespace pollution!")
using namespace lazy;
#endif

#endif