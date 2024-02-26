#include "a.h"
#include "b.h"
// #include "../log.h"

int main() {
    Log::set_log_lev(LogLevel::Debug);
    Log::set_log_file("log.log");
    Log::init();
    DEBUG(111);
    INFO(222);
    WARN(333);
    Log::deinit();
    return 0;
}