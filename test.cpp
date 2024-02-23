#include "log.hpp"

int main() {
    Log::set_log_lev(LogLevel::Debug);
    Log::set_log_file("a.log");
    DEBUG(111);
    INFO(222);
    WARN(333);
    return 0;
}