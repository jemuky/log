# Usage

```cpp
#include "log.h"

int main() {
    Log::set_log_lev(LogLevel::Debug);
    
    DEBUG(111);
    INFO(222);
    WARN(333);
    ERROR(444);
    return 0;
}
```
