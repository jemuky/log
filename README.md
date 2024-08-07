# Usage

```cpp
#include "log.h"

int main() {
    Log::set_log_lev(LogLevel::Debug);
    // 这里只是演示支持各种斜杠，也可以abc/tmp/def/log.log
    Log::set_log_file("abc/tmp/def\\log.log");
    if (!Log::init()) {
        return -1;
    }
    Log::write_console(false);
    DEBUG("{}", 111);
    INFO("{}", 222);
    WARN("{}", 333);

    Struct s("abc", 123);
    ERROR("{}", s);
    return 0;
}
```
