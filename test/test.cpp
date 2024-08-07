#include "a.h"
#include "b.h"
#include "struct.h"

// 测试吞吐量
void test_throughput();

int main() {
    Log::set_log_lev(LogLevel::Debug);
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

    test_throughput();
    return 0;
}

void log_messages(int thread_id, std::atomic<int>& total_msgs, int messages_num) {
    for (int i = 0; i < messages_num; ++i) {
        ERROR("Thread {}: Log message {}", thread_id, i);
        total_msgs++;
    }
}

void test_throughput() {
    const int num_threads = 10;       // 线程数
    const int msgs_per_thread = 1000; // 每个线程写入的日志数量
    std::atomic<int> total_msgs(0);
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();
    // 启动多个线程
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(log_messages, i, std::ref(total_msgs), msgs_per_thread);
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    // 计算吞吐量
    double throughput = total_msgs / duration.count(); // 消息数 / 时间（秒）

    INFO("duration: {} seconds, throughput: {} messages per second", duration.count(), throughput);
}