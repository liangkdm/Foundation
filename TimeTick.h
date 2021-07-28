
#ifndef TIMECOUNT_H
#define TIMECOUNT_H

#include <atomic>
#include <chrono>

//计时类
class TimeCount {
public:
    TimeCount()
        : m_begin(std::chrono::high_resolution_clock::now()) {
    }
    void reset() {
        m_begin = std::chrono::high_resolution_clock::now();
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> get_starttime() const {
        return m_begin;
    }

    int64_t get_starttimestamp() const {
        auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(m_begin.time_since_epoch());
        return tmp.count();
    }

    //默认输出毫秒
    int64_t elapsed() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }
    //微秒
    int64_t elapsed_micro() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }
    //纳秒
    int64_t elapsed_nano() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }
    //秒
    int64_t elapsed_seconds() const {
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }
    //分
    int64_t elapsed_minutes() const {
        return std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }
    //时
    int64_t elapsed_hours() const {
        return std::chrono::duration_cast<std::chrono::hours>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;
};

#endif // TIMECOUNT_H
