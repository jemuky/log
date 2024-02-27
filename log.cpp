#include "log.h"

std::mutex Log::__mtx_;
Log* Log::__log_ = nullptr;

Log::Log() {
    __log_lev_ = LogLevel::Debug;
    __is_write_file_ = true;
    __log_file_ = "./log.log";
}

Log* Log::get_instance() {
    if (__log_ == nullptr) {
        std::lock_guard<std::mutex> lock(__mtx_);
        if (__log_ == nullptr) {
            __log_ = new Log();
        }
    }
    return __log_;
}

void Log::set_log_lev(LogLevel lev) { get_instance()->__log_lev_ = lev; }
LogLevel Log::get_log_lev() { return get_instance()->__log_lev_; }
void Log::write_file(bool is_write_file) { get_instance()->__is_write_file_ = is_write_file; }

void Log::set_log_file(const std::string& file_path) {
    std::lock_guard<std::mutex> lock(Log::__mtx_);
    get_instance()->__log_file_ = file_path;
}

bool Log::init() {
    __log_->__ofs_.open(__log_->__log_file_, std::ios::app);
    if (!__log_->__ofs_.is_open()) {
        __log_->__is_write_file_ = false;
        ERROR(std::strerror(errno));
        delete __log_;
        return false;
    }
    return true;
}

void Log::deinit() {
    if (__log_) {
        delete __log_;
        __log_ = nullptr;
    }
}
