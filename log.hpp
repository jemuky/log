#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>

// windows颜色
//         前景    背景
// Default ESC[39m ESC[49m
// Black ESC[30m ESC[40m
// Dark red ESC[31m ESC[41m
// Dark green ESC[32m ESC[42m
// Dark yellow (Orange-ish) ESC[33m ESC[43m
// Dark blue ESC[34m ESC[44m
// Dark magenta ESC[35m ESC[45m
// Dark cyan ESC[36m ESC[46m
// Light gray ESC[37m ESC[47m
// Dark gray ESC[90m ESC[100m
// Red ESC[91m ESC[101m
// Green ESC[92m ESC[101m
// Orange ESC[93m ESC[103m
// Blue ESC[94m ESC[104m
// Magenta ESC[95m ESC[105m
// Cyan ESC[96m ESC[106m
// White ESC[97m ESC[107m

// linux颜色
// 字颜色范围:30----37
// 30:黑色
// 31:深红
// 32:绿色
// 33:黄色
// 34:蓝色
// 35:紫色
// 36:深绿
// 37:白色
#ifdef _WIN32
#define __LOG_COLOR_END__ "\u001b[39m"
#define __LOG_COLOR_ERR__ "\u001b[91m"
#define __LOG_COLOR_WARN__ "\u001b[33m"
#define __LOG_COLOR_INFO__ "\u001b[94m"
#define __LOG_COLOR_DEBUG__ "\u001b[95m"
#elif __linux__
#define __LOG_COLOR_END__ "\033[0m"
#define __LOG_COLOR_ERR__ "\033[31m"
#define __LOG_COLOR_WARN__ "\033[33m"
#define __LOG_COLOR_INFO__ "\033[34m"
#define __LOG_COLOR_DEBUG__ "\033[30m"
#else
#define __LOG_COLOR_END__ ""
#define __LOG_COLOR_ERR__ ""
#define __LOG_COLOR_WARN__ ""
#define __LOG_COLOR_INFO__ ""
#define __LOG_COLOR_DEBUG__ ""
#endif

enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warn = 2,
    Error = 3,
};

/// @brief 写文件时，若多级目录没创建，会失败
struct Log {
    LogLevel log_lev_ = LogLevel::Debug;
    bool is_write_file_ = true;
    std::string log_file_ = "./log.log";
    std::ofstream of;

    static std::mutex mtx_;
    static Log* log_;

    Log& operator=(const Log&) = delete;

    static Log* get_instance() {
        if (log_ == nullptr) {
            std::lock_guard<std::mutex> lock(mtx_);
            if (log_ == nullptr) {
                log_ = new Log();
                log_->of.open(log_->log_file_, std::ios::app);
                if (!log_->of.is_open()) {
                    log_->is_write_file_ = false;
                    delete log_;
                }
            }
        }
        return log_;
    }

    static void set_log_lev(LogLevel lev) { get_instance()->log_lev_ = lev; }
    static LogLevel get_log_lev() { return get_instance()->log_lev_; }
    static void write_file(bool is_write_file) { get_instance()->is_write_file_ = is_write_file; }

    /// 设置文件时不能设置未创建好的多级目录
    static void set_log_file(const std::string& file_path) {
        std::lock_guard<std::mutex> lock(Log::mtx_);
        auto log = get_instance();
        log->log_file_ = file_path;
        log->of.close();
        log->of.open(log->log_file_, std::ios::app);
        if (!log->of.is_open()) {
            log->is_write_file_ = false;
            std::cout << __LOG_COLOR_ERR__ << std::strerror(errno) << __LOG_COLOR_END__
                      << std::endl;
            delete log_;
        }
    }

    template <class... Args>
    static void log_print(const char* file, int line, const char* function, const char* color,
                          const char* prefix, const Args&... args) {
        // 时间
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        // 获取微秒部分
        auto microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
        // 将微秒部分转换为字符串
        std::string microseconds_str = std::to_string(microseconds);

        // 主要字符串拼接
        std::stringstream ss;
        ss << std::boolalpha << std::left << std::setw(8) << prefix
           << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << "."
           << microseconds_str.substr(0, 5) << "  " << file << "(" << function << ")"
           << ":" << line << ": ";
        auto print_func = [&](auto i) { ss << i; };
        std::initializer_list<int>{(print_func(args), 0)...};
        ss << "\n";
        ss.flush();

        // console
        std::cout << color << ss.str() << __LOG_COLOR_END__;

        // write file
        std::lock_guard<std::mutex> lock(Log::mtx_);
        auto log = get_instance();
        if (!log->is_write_file_) return;
        log->of << ss.str();
        log->of.flush();
    }
};

std::mutex Log::mtx_;
Log* Log::log_ = nullptr;

#ifdef _WIN32
#define PARAM_ERR_FUNCTION __FUNCSIG__
#else
#define PARAM_ERR_FUNCTION __PRETTY_FUNCTION__
#endif

#define ERROR(...)                                                                       \
    if (static_cast<int>(Log::get_log_lev()) <= static_cast<int>(LogLevel::Error)) {     \
        Log::log_print(__FILE__, __LINE__, static_cast<const char*>(PARAM_ERR_FUNCTION), \
                       __LOG_COLOR_ERR__, "[ERROR] ", __VA_ARGS__);                      \
    }

#define WARN(...)                                                                   \
    if (static_cast<int>(Log::get_log_lev()) <= static_cast<int>(LogLevel::Warn)) { \
        Log::log_print(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__),  \
                       __LOG_COLOR_WARN__, "[WARN] ", __VA_ARGS__);                 \
    }

#define INFO(...)                                                                   \
    if (static_cast<int>(Log::get_log_lev()) <= static_cast<int>(LogLevel::Info)) { \
        Log::log_print(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__),  \
                       __LOG_COLOR_INFO__, "[INFO] ", __VA_ARGS__);                 \
    }

#define DEBUG(...)                                                                   \
    if (static_cast<int>(Log::get_log_lev()) <= static_cast<int>(LogLevel::Debug)) { \
        Log::log_print(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__),   \
                       __LOG_COLOR_DEBUG__, "[DEBUG] ", __VA_ARGS__);                \
    }
