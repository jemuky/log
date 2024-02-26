#include "log.h"

std::mutex Log::mtx_;
Log* Log::log_ = nullptr;

Log::Log() {}

Log* Log::get_instance() {
    if (log_ == nullptr) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (log_ == nullptr) {
            log_ = new Log();
        }
    }
    return log_;
}

void Log::set_log_lev(LogLevel lev) { get_instance()->log_lev_ = lev; }
LogLevel Log::get_log_lev() { return get_instance()->log_lev_; }
void Log::write_file(bool is_write_file) { get_instance()->is_write_file_ = is_write_file; }

void Log::set_log_file(const std::string& file_path) {
    std::lock_guard<std::mutex> lock(Log::mtx_);
    get_instance()->log_file_ = file_path;
}

bool Log::init() {
    log_->of.open(log_->log_file_, std::ios::app);
    if (!log_->of.is_open()) {
        log_->is_write_file_ = false;
        ERROR(std::strerror(errno));
        delete log_;
        return false;
    }
    return true;
}

void Log::deinit() {
    if (log_) {
        delete log_;
        log_ = nullptr;
    }
}
