#include "a.h"
#include "b.h"
// #include "../log.h"

int main() {
    Log::set_log_lev(LogLevel::Debug);
    Log::set_log_file("log.log");
    DEBUG(111);
    INFO(222);
    WARN(333);
    return 0;
}