# Usage

```cpp
#include "log.h"

int main() {
    // 设置日志等级
    Log::set_log_lev(LogLevel::Debug);
    // 设置日志文件，分隔符使用/或\\都可以
    Log::set_log_file("abc/tmp/def\\log.log");
    // 当init失败后只是不能输出到文件中，可以继续执行
    if (!Log::init()) {
        return -1;
    }
    DEBUG(111);
    INFO(222);
    WARN(333);
    // 可以不调用，等系统自动清理
    Log::deinit();
    return 0;
}
```
