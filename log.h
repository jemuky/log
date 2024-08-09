#pragma once

#include <chrono>
#include <filesystem>
#include <format>
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

enum class LogLevel : int {
    Debug = 0,
    Info = 1,
    Warn = 2,
    Error = 3,
};

struct Log {
    Log& operator=(const Log&) = delete;
    Log(const Log&) = delete;
    Log(Log&&) = delete;

    static void set_log_lev(LogLevel lev);
    static LogLevel get_log_lev();
    // 是否输出文件(默认写入./log.log)
    static void write_file(bool is_write_file);
    // 是否输出控制台(默认输出)
    static void write_console(bool is_write_console);

    static void set_log_file(const std::string& file_path);

    /// @brief  在所有set_xxx后调用，
    // 不调用init不会输出文件，只会输出控制台
    /// @return false时说明文件打不开
    static bool init();

    template <class... Args>
    static void log_print(const char* file, int line, const char* function, const char* color,
                          const char* prefix, const char* format_str, Args&&... args);

private:
    LogLevel __log_lev_;
    bool __is_write_file_;
    bool __is_write_console_;
    std::string __log_file_;
    std::ofstream __ofs_;

    Log();
    static Log& get_instance();
    ~Log();
};

template <class... Args>
void Log::log_print(const char* file, int line, const char* function, const char* color,
                    const char* prefix, const char* format_str, Args&&... args) {
    // 主要字符串拼接
    std::string formatted_header =
        std::format("{:<8}{:%Y-%m-%d %H:%M:%S}  {}:{}({}): ", prefix,
                    std::chrono::current_zone()->to_local(std::chrono::system_clock::now()), file,
                    line, function);
    std::string formatted_message = std::vformat(format_str, std::make_format_args(args...));

    auto ss = formatted_header + formatted_message + "\n";

    auto& log = get_instance();
    if (log.__is_write_console_)
        // console，这里实测 printf 比 cout 速度快，printf每秒8500+条，cout每秒6000+条
        // 去掉console日志，每秒60000+条
        printf("%s%s%s", color, ss.c_str(), __LOG_COLOR_END__);

    // write file
    if (!log.__ofs_.is_open() || !log.__is_write_file_) return;
    log.__ofs_ << ss;
    log.__ofs_.flush();
}

#ifdef _WIN32
#define PARAM_ERR_FUNCTION __FUNCSIG__
#else
#define PARAM_ERR_FUNCTION __PRETTY_FUNCTION__
#endif

#define ERROR(format_str, ...)                                                           \
    if (Log::get_log_lev() <= LogLevel::Error) {                                         \
        Log::log_print(__FILE__, __LINE__, static_cast<const char*>(PARAM_ERR_FUNCTION), \
                       __LOG_COLOR_ERR__, "[ERROR] ", format_str, __VA_ARGS__);          \
    }

#define WARN(format_str, ...)                                                      \
    if (Log::get_log_lev() <= LogLevel::Warn) {                                    \
        Log::log_print(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__), \
                       __LOG_COLOR_WARN__, "[WARN] ", format_str, __VA_ARGS__);    \
    }

#define INFO(format_str, ...)                                                      \
    if (Log::get_log_lev() <= LogLevel::Info) {                                    \
        Log::log_print(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__), \
                       __LOG_COLOR_INFO__, "[INFO] ", format_str, __VA_ARGS__);    \
    }

#define DEBUG(format_str, ...)                                                     \
    if (Log::get_log_lev() <= LogLevel::Debug) {                                   \
        Log::log_print(__FILE__, __LINE__, static_cast<const char*>(__FUNCTION__), \
                       __LOG_COLOR_DEBUG__, "[DEBUG] ", format_str, __VA_ARGS__);  \
    }
