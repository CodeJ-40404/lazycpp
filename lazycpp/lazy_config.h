// lazy_config.h
// copyright 2026 ? lazycpp MIT License
// 所有配置都在这里，便于管理

#pragma once

// ============================================================
// 导出控制宏
// ============================================================

// 定义这个宏可以将所有子命名空间导出到 lazy::
// 使用方法：
//   方式1：在包含 lazy.h 前 #define LAZY_EXPORT_ALL
//   方式2：编译时 -DLAZY_EXPORT_ALL
//   方式3：定义 LAZY_USE_ALL 仅在当前作用域使用

// 默认不导出（安全模式）
#ifndef LAZY_EXPORT_ALL
#define LAZY_EXPORT_ALL 0
#endif

// ============================================================
// 编译器检测
// ============================================================

#ifdef _WIN32
#define LAZY_WINDOWS
#endif

#ifdef __linux__
#define LAZY_LINUX
#endif

#ifdef __APPLE__
#define LAZY_MACOS
#endif

#ifdef __GNUC__
#define LAZY_GCC
#endif

#ifdef __clang__
#define LAZY_CLANG
#endif

#ifdef _MSC_VER
#define LAZY_MSVC
#endif

// ============================================================
// 版本信息
// ============================================================

#define LAZY_VERSION_MAJOR 1
#define LAZY_VERSION_MINOR 0
#define LAZY_VERSION_PATCH 0

// ============================================================
// 编译警告（提醒用户）
// ============================================================

#ifdef LAZY_EXPORT_ALL
#pragma message("??  LAZY_EXPORT_ALL is enabled - namespace pollution may occur")
#pragma message("   Use LAZY_USE_ALL for local scope instead if possible")
#pragma message("   Define LAZY_EXPORT_ALL=0 to disable")
#endif