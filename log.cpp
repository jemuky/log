#include "log.h"

Log::Log() {
    __log_lev_ = LogLevel::Debug;
    __is_write_file_ = true;
    __is_write_console_ = true;
    __log_file_ = "./log.log";
}

Log& Log::get_instance() {
    static Log __log_;
    return __log_;
}

void Log::set_log_lev(LogLevel lev) { get_instance().__log_lev_ = lev; }
LogLevel Log::get_log_lev() { return get_instance().__log_lev_; }
void Log::write_file(bool is_write_file) { get_instance().__is_write_file_ = is_write_file; }
void Log::write_console(bool is_write_console) {
    get_instance().__is_write_console_ = is_write_console;
}

void Log::set_log_file(const std::string& file_path) { get_instance().__log_file_ = file_path; }

bool Log::init() {
    // 获取文件夹与文件的分割
    auto index = get_instance().__log_file_.find_last_of("\\/");

    std::string log_file_path, log_path;
    if (index == std::string::npos) {
        log_file_path = get_instance().__log_file_;
    } else {
        log_file_path = get_instance().__log_file_.substr(index + 1);
        log_path = get_instance().__log_file_.substr(0, index);
    }

    // 创建文件夹
    if (log_path.size() > 0) {
        namespace fs = std::filesystem;
        // (不存在 或者 不是文件夹) 且 创建文件夹失败
        if ((!fs::exists(log_path) || !fs::is_directory(log_path)) &&
            !fs::create_directories(log_path)) {
            get_instance().__is_write_file_ = false;
            ERROR("CreateDirectory failed, {}", std::strerror(errno));
            return false;
        }
    }

    // 打开文件
    get_instance().__ofs_.open(get_instance().__log_file_, std::ios::app);
    if (!get_instance().__ofs_.is_open()) {
        get_instance().__is_write_file_ = false;
        ERROR("{}", std::strerror(errno));
        return false;
    }
    return true;
}
