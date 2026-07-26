// lazy_debug.h
// copyright 2026 lazycpp MIT License
#define _CRT_SECURE_NO_WARNINGS
#pragma once

// ========== 基础 ==========
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <cstdlib>

// ========== 工具 ==========
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>

// ========== 类型支持 ==========
#include <cstdint>
#include <climits>
#include <type_traits>

// ========== 异常 ==========
#include <exception>
#include <stdexcept>

// ========== 平台相关 ==========
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <signal.h>
#include <excpt.h>
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")
#else
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ucontext.h>
#endif
namespace lazy {
    namespace debug {

        // ========== 日志级别 ==========
        enum LogLevel {
            LOG_INFO = 0,
            LOG_WARNING = 1,
            LOG_ERROR = 2,
            LOG_FATAL = 3
        };

        // ========== 配置 ==========
        struct Config {
            std::string log_path = "crash.log";
            bool enable_console_output = false;
            int max_stack_frames = 64;
        };

        // ========== 日志记录器 ==========
        class Logger {
        private:
            Config config_;
            std::ofstream log_file_;
            bool is_open_ = false;
            bool header_written_ = false;

            void write_header() {
                if (header_written_) return;

                // 检查文件是否为空，避免重复写入头部
                if (is_open_) {
                    log_file_.seekp(0, std::ios::end);
                    if (log_file_.tellp() > 0) {
                        // 文件已有内容，不写入头部
                        header_written_ = true;
                        return;
                    }
                }

                std::ostringstream oss;
                oss << "========================================\n";
                oss << "      Lazy Debug Crash Logger\n";
                oss << "      copyright 2026 lazycpp\n";
                oss << "========================================\n";
                oss << " Log started at: " << get_current_time() << "\n";
                oss << "========================================\n\n";

                if (config_.enable_console_output) {
                    std::cerr << oss.str();
                }

                if (is_open_) {
                    log_file_ << oss.str();
                    log_file_.flush();
                }

                header_written_ = true;
            }

            std::string get_current_time() {
                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                std::string time_str = std::ctime(&time_t);
                if (!time_str.empty()) {
                    time_str.pop_back(); // 移除换行符
                }
                return time_str;
            }

            std::string get_level_string(int level) {
                switch (level) {
                case LOG_INFO: return "INFO";
                case LOG_WARNING: return "WARNING";
                case LOG_ERROR: return "ERROR";
                case LOG_FATAL: return "FATAL";
                default: return "UNKNOWN";
                }
            }

            std::string format_log_entry(int level, const std::string& msg, const std::string& detail) {
                std::ostringstream oss;

                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                auto tm = *std::localtime(&time_t);

                oss << "[" << get_level_string(level) << "] "
                    << std::setw(4) << std::setfill('0') << (tm.tm_year + 1900) << "-"
                    << std::setw(2) << std::setfill('0') << (tm.tm_mon + 1) << "-"
                    << std::setw(2) << std::setfill('0') << tm.tm_mday << " "
                    << std::setw(2) << std::setfill('0') << tm.tm_hour << ":"
                    << std::setw(2) << std::setfill('0') << tm.tm_min << ":"
                    << std::setw(2) << std::setfill('0') << tm.tm_sec
                    << " : " << msg;

                if (!detail.empty()) {
                    oss << "\n  -> " << detail;
                }

                oss << "\n";
                return oss.str();
            }

        public:
            Logger() = default;

            bool init(const Config& cfg) {
                config_ = cfg;

                log_file_.open(config_.log_path, std::ios::app);
                is_open_ = log_file_.is_open();

                if (!is_open_) {
                    std::cerr << "[Lazy Debug] Failed to open log: " << config_.log_path << "\n";
                    return false;
                }

                write_header();
                return true;
            }

            void log(int level, const std::string& msg, const std::string& detail = "") {
                if (!header_written_) {
                    write_header();
                }

                std::string entry = format_log_entry(level, msg, detail);

                if (config_.enable_console_output) {
                    std::cerr << entry;
                }

                if (is_open_) {
                    log_file_ << entry;
                    log_file_.flush();
                }
            }

            void write_crash_report(const std::string& report) {
                if (!header_written_) {
                    write_header();
                }

                // 添加崩溃报告分隔线和时间戳
                std::string time = get_current_time();
                std::string separator = "\n--- CRASH REPORT [" + time + "] ---\n";

                if (config_.enable_console_output) {
                    std::cerr << separator << report;
                }

                if (is_open_) {
                    log_file_ << separator << report;
                    log_file_.flush();
                }
            }

            void close() {
                if (is_open_) {
                    log_file_ << "\n========================================\n";
                    log_file_ << " Log closed at: " << get_current_time() << "\n";
                    log_file_ << "========================================\n";
                    log_file_.flush();
                    log_file_.close();
                }
                is_open_ = false;
                header_written_ = false;
            }

            ~Logger() {
                close();
            }
        };

        // ========== 错误信息收集器 ==========
        class CrashInfo {
        public:
            std::string timestamp;
            std::string error_type;
            int error_code = 0;
            std::string fault_address;
            std::vector<std::string> stack_trace;
            std::string additional_info;

            std::string format() const {
                std::ostringstream oss;

                oss << "\n========================================\n";
                oss << "         CRASH REPORT\n";
                oss << "========================================\n\n";

                oss << "[TIME]\n";
                oss << timestamp << "\n\n";

                oss << "[ERROR]\n";
                oss << "Type: " << error_type << " (Code: " << error_code << ")\n";
                if (!fault_address.empty()) {
                    oss << "Address: " << fault_address << "\n";
                }
                oss << "\n";

                oss << "[STACK TRACE]\n";
                if (stack_trace.empty()) {
                    oss << "  (No stack trace available)\n";
                }
                else {
                    for (size_t i = 0; i < stack_trace.size(); i++) {
                        oss << "  #" << i << " " << stack_trace[i] << "\n";
                    }
                }
                oss << "\n";

                if (!additional_info.empty()) {
                    oss << "[SYSTEM INFO]\n";
                    oss << additional_info << "\n";
                }

                oss << "\n========================================\n";
                oss << " Tip: Check null pointers, array bounds, memory leaks\n";
                oss << "========================================\n\n";

                return oss.str();
            }

            std::string format_summary() const {
                std::ostringstream oss;
                oss << "[" << timestamp << "] " << error_type;
                if (!fault_address.empty()) {
                    oss << " at " << fault_address;
                }
                return oss.str();
            }
        };

        // ========== 前向声明 ==========
        class Debugger;

        // ========== 全局指针 ==========
        static Debugger* g_debugger_instance = nullptr;

        // ========== Debugger ==========
        class Debugger {
        private:
            Logger logger_;
            Config config_;
            bool initialized_ = false;

    #if defined(_WIN32) || defined(_WIN64)
            static LONG WINAPI exception_handler(EXCEPTION_POINTERS* ex_info) {
                if (g_debugger_instance) {
                    g_debugger_instance->handle_exception(ex_info);
                }
                return EXCEPTION_EXECUTE_HANDLER;
            }

            void handle_exception(EXCEPTION_POINTERS* ex_info) {
                CrashInfo crash;

                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                crash.timestamp = std::ctime(&time_t);
                if (!crash.timestamp.empty()) {
                    crash.timestamp.pop_back();
                }

                DWORD code = ex_info->ExceptionRecord->ExceptionCode;
                crash.error_code = code;
                crash.error_type = get_exception_name(code);

                std::ostringstream oss;
                oss << "0x" << std::hex << (uintptr_t)ex_info->ExceptionRecord->ExceptionAddress;
                crash.fault_address = oss.str();

                crash.stack_trace = get_stack_trace_win(ex_info->ContextRecord);
                crash.additional_info = get_additional_info();

                logger_.log(LOG_FATAL, crash.format_summary());
                logger_.write_crash_report(crash.format());
                logger_.close();

                // 程序即将退出，刷新所有缓冲区
                std::cerr.flush();
            }

            std::string get_exception_name(DWORD code) {
                switch (code) {
                case EXCEPTION_ACCESS_VIOLATION:     return "ACCESS_VIOLATION";
                case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:return "ARRAY_BOUNDS_EXCEEDED";
                case EXCEPTION_DATATYPE_MISALIGNMENT:return "DATATYPE_MISALIGNMENT";
                case EXCEPTION_FLT_DIVIDE_BY_ZERO:   return "FLT_DIVIDE_BY_ZERO";
                case EXCEPTION_INT_DIVIDE_BY_ZERO:   return "INT_DIVIDE_BY_ZERO";
                case EXCEPTION_STACK_OVERFLOW:       return "STACK_OVERFLOW";
                case EXCEPTION_ILLEGAL_INSTRUCTION:  return "ILLEGAL_INSTRUCTION";
                default:                             return "Unknown Exception";
                }
            }

            std::vector<std::string> get_stack_trace_win(CONTEXT* context) {
                std::vector<std::string> result;

                HANDLE process = GetCurrentProcess();
                SymInitialize(process, NULL, TRUE);

                STACKFRAME64 stack_frame = { 0 };

    #if defined(_WIN64)
                stack_frame.AddrPC.Offset = context->Rip;
                stack_frame.AddrPC.Mode = AddrModeFlat;
                stack_frame.AddrFrame.Offset = context->Rbp;
                stack_frame.AddrFrame.Mode = AddrModeFlat;
                stack_frame.AddrStack.Offset = context->Rsp;
                stack_frame.AddrStack.Mode = AddrModeFlat;
    #else
                stack_frame.AddrPC.Offset = context->Eip;
                stack_frame.AddrPC.Mode = AddrModeFlat;
                stack_frame.AddrFrame.Offset = context->Ebp;
                stack_frame.AddrFrame.Mode = AddrModeFlat;
                stack_frame.AddrStack.Offset = context->Esp;
                stack_frame.AddrStack.Mode = AddrModeFlat;
    #endif

                HANDLE thread = GetCurrentThread();

    #if defined(_WIN64)
                DWORD machine_type = IMAGE_FILE_MACHINE_AMD64;
    #else
                DWORD machine_type = IMAGE_FILE_MACHINE_I386;
    #endif

                for (int i = 0; i < 64; i++) {
                    if (!StackWalk64(machine_type, process, thread,
                        &stack_frame, context, NULL,
                        SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
                        break;
                    }

                    if (stack_frame.AddrPC.Offset == 0) break;

                    char symbol_buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
                    SYMBOL_INFO* symbol = (SYMBOL_INFO*)symbol_buffer;
                    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
                    symbol->MaxNameLen = MAX_SYM_NAME;

                    DWORD64 displacement = 0;
                    std::ostringstream oss;

                    if (SymFromAddr(process, stack_frame.AddrPC.Offset, &displacement, symbol)) {
                        oss << symbol->Name << " + 0x" << std::hex << displacement;
                    }
                    else {
                        oss << "0x" << std::hex << stack_frame.AddrPC.Offset;
                    }

                    // 添加模块信息
                    IMAGEHLP_MODULE64 module_info = { 0 };
                    module_info.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
                    if (SymGetModuleInfo64(process, stack_frame.AddrPC.Offset, &module_info)) {
                        oss << " [" << module_info.ModuleName << "]";
                    }

                    result.push_back(oss.str());
                }

                SymCleanup(process);
                return result;
            }

    #endif

    #ifndef _WIN32
            static void signal_handler(int sig, siginfo_t* info, void* context) {
                if (g_debugger_instance) {
                    g_debugger_instance->handle_signal(sig, info, context);
                }
                // 恢复默认信号处理并重新触发
                signal(sig, SIG_DFL);
                raise(sig);
            }

            void handle_signal(int sig, siginfo_t* info, void* context) {
                CrashInfo crash;

                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                crash.timestamp = std::ctime(&time_t);
                if (!crash.timestamp.empty()) {
                    crash.timestamp.pop_back();
                }

                crash.error_code = sig;
                crash.error_type = get_signal_name(sig);

                if (info) {
                    std::ostringstream oss;
                    oss << "0x" << std::hex << (uintptr_t)info->si_addr;
                    crash.fault_address = oss.str();
                }

                crash.stack_trace = get_stack_trace_linux();
                crash.additional_info = get_additional_info();

                logger_.log(LOG_FATAL, crash.format_summary());
                logger_.write_crash_report(crash.format());
                logger_.close();

                std::cerr.flush();
            }

            std::string get_signal_name(int sig) {
                switch (sig) {
                case SIGSEGV: return "SIGSEGV";
                case SIGFPE:  return "SIGFPE";
                case SIGABRT: return "SIGABRT";
                case SIGILL:  return "SIGILL";
                case SIGBUS:  return "SIGBUS";
                default:      return "Unknown Signal";
                }
            }

            std::vector<std::string> get_stack_trace_linux() {
                std::vector<std::string> result;
                void* buffer[64];
                int nptrs = backtrace(buffer, 64);
                char** symbols = backtrace_symbols(buffer, nptrs);

                if (symbols) {
                    for (int i = 0; i < nptrs; i++) {
                        result.push_back(symbols[i]);
                    }
                    free(symbols);
                }
                return result;
            }

            void register_signals() {
                struct sigaction sa;
                sa.sa_sigaction = signal_handler;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = SA_SIGINFO;

                sigaction(SIGSEGV, &sa, nullptr);
                sigaction(SIGFPE, &sa, nullptr);
                sigaction(SIGABRT, &sa, nullptr);
                sigaction(SIGILL, &sa, nullptr);
                sigaction(SIGBUS, &sa, nullptr);
            }
    #endif

            std::string get_additional_info() {
                std::ostringstream oss;

    #ifdef __cplusplus
                oss << "  C++ Standard: " << __cplusplus << "\n";
    #endif

    #ifdef _MSC_VER
                oss << "  Compiler: MSVC " << _MSC_VER << "\n";
    #endif

    #ifdef __GNUC__
                oss << "  Compiler: GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << "\n";
    #endif

    #ifdef __clang__
                oss << "  Compiler: Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << "\n";
    #endif

    #if defined(_WIN64)
                oss << "  Platform: Windows x64\n";
    #elif defined(_WIN32)
                oss << "  Platform: Windows x86\n";
    #elif __linux__
                oss << "  Platform: Linux\n";
    #elif __APPLE__
                oss << "  Platform: macOS\n";
    #endif

    #ifdef __x86_64__
                oss << "  Architecture: x86_64\n";
    #elif __i386__
                oss << "  Architecture: x86\n";
    #endif

                // 添加进程ID
    #if defined(_WIN32) || defined(_WIN64)
                oss << "  Process ID: " << GetCurrentProcessId() << "\n";
    #else
                oss << "  Process ID: " << getpid() << "\n";
    #endif

                return oss.str();
            }

        public:
            Debugger() {
                g_debugger_instance = this;
            }

            ~Debugger() {
                if (initialized_) {
                    logger_.log(LOG_INFO, "Lazy Debug Shutdown");
                    logger_.close();
                }
                g_debugger_instance = nullptr;
            }

            bool init(const Config& cfg = Config()) {
                // 防止重复初始化
                if (initialized_) {
                    // 如果已经初始化，只更新配置
                    config_ = cfg;
                    logger_.log(LOG_INFO, "Configuration updated");
                    return true;
                }

                config_ = cfg;

                if (!logger_.init(config_)) {
                    std::cerr << "[Lazy Debug] Failed to open log: " << config_.log_path << "\n";
                    return false;
                }

    #if defined(_WIN32) || defined(_WIN64)
                SetUnhandledExceptionFilter(exception_handler);
    #else
                register_signals();
    #endif

                initialized_ = true;

                std::ostringstream msg;
                msg << "Lazy Debug initialized (PID: ";
    #if defined(_WIN32) || defined(_WIN64)
                msg << GetCurrentProcessId();
    #else
                msg << getpid();
    #endif
                msg << ")";
                logger_.log(LOG_INFO, msg.str());

                return true;
            }

            void enable_console_output(bool enable = true) {
                config_.enable_console_output = enable;
                logger_.log(LOG_INFO, "Console output " + std::string(enable ? "enabled" : "disabled"));
            }

            void set_log_path(const std::string& path) {
                config_.log_path = path;
                logger_.log(LOG_INFO, "Log path set to: " + path);
            }

            void log_message(const std::string& msg) {
                logger_.log(LOG_INFO, msg);
            }

            void log_warning(const std::string& msg, const std::string& detail = "") {
                logger_.log(LOG_WARNING, msg, detail);
            }

            void log_error(const std::string& msg, const std::string& detail = "") {
                logger_.log(LOG_ERROR, msg, detail);
            }

            bool is_initialized() const {
                return initialized_;
            }
        };

        // ========== 全局函数 ==========

        inline void init(const std::string& log_path = "crash.log") {
            static Debugger debugger;
            Config cfg;
            cfg.log_path = log_path;
            cfg.enable_console_output = false;
            debugger.init(cfg);
        }

        inline void enable_console_log() {
            if (g_debugger_instance) {
                g_debugger_instance->enable_console_output(true);
            }
        }

        inline void disable_console_log() {
            if (g_debugger_instance) {
                g_debugger_instance->enable_console_output(false);
            }
        }

        inline void set_log_path(const std::string& path) {
            if (g_debugger_instance) {
                g_debugger_instance->set_log_path(path);
            }
        }

        inline void log_info(const std::string& msg) {
            if (g_debugger_instance) {
                g_debugger_instance->log_message(msg);
            }
        }

        inline void log_warning(const std::string& msg, const std::string& detail = "") {
            if (g_debugger_instance) {
                g_debugger_instance->log_warning(msg, detail);
            }
        }

        inline void log_error(const std::string& msg, const std::string& detail = "") {
            if (g_debugger_instance) {
                g_debugger_instance->log_error(msg, detail);
            }
        }

        inline bool is_initialized() {
            return g_debugger_instance && g_debugger_instance->is_initialized();
        }

    } // namespace debug
#ifdef LAZY_EXPORT_ALL
	using namespace debug;
#endif
} // namespace lazy