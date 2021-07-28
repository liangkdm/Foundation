
/*
C++11规定了local static在多线程条件下的初始化行为，要求编译器保证了内部静态变量的线程安全性。
在C++11标准下，《Effective C++》提出了一种更优雅的单例模式实现，使用函数内的 local static对象。
这样，只有当第一次访问getInstance()方法时才创建实例。这种方法也被称为Meyers' Singleton。
C++0x之后该实现是线程安全的，C++0x之前仍需加锁
*/
namespace Foundation {
/// \class template<class T>CSingleton
/// \brief 单例模板类,
/// \see
template <class T>
class Singleton {
private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

public:
    static T &getInstance() {
        static T instance;
        return instance;
    }
};

} // namespace Foundation
