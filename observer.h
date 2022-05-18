#include <functional>
#include <iostream>
#include <list>
#include <map>

class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable operator=(const NonCopyable &) = delete;
};

template <typename F>
class Events : public NonCopyable {
public:
    Events() {
    }
    ~Events() {
    }

    // 注册观察者，支持右值引用
    int Connect(F &&f) {
        return Assign(f);
    }
    // 普通注册观察者函数
    int Connect(const F &f) {
        return Assign(f);
    }
    // 移除观察者
    void Disconnect(int key) {
        connections_.erase(key);
    }
    // 通知
    template <typename... Args> 
    void Notify(Args &&...args) {
        for (auto &iter : connections_) {
            iter.second(std::forward<Args>(args)...);
        }
    }

private:
    template <typename F> 
    int Assign(F &f) {
        int k = observer_++;
        connections_.emplace(k, std::forward<F>(f));
        return k;
    }

private:
    int observer_ = 0;
    std::map<int, F> connections_;
};

// struct T {
//     int a, b;
//     void print(int a, int b) {
//         std::cout << a << ", " << b << std::endl;
//     }
// };
//
// void print(int a, int b) {
//     std::cout << a << ", " << b << std::endl;
// }

/*
int main() {
    Events<std::function<void(int, int)>> event;

    // 第一种注册方式
    auto key = event.Connect(print);

    // 第二种 lambda 方式
    T t;
    auto lambdaKey = event.Connect([ &t ](int a, int b) {
        t.a = a;
        t.b = b;
        std::cout << t.a << ", " << t.b << std::endl;
    });

    // 第三种 bind 一个 function 的方式
    auto functionKey = event.Connect(std::bind(&T::print, &t, std::placeholders::_1, std::placeholders::_2));

    int a = 10, b = 20;
    event.Notify(a, b);
}
*/
