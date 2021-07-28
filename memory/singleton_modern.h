
#include <memory>
namespace Foundation {
/// \class template<class T>CSingleton
/// \brief 单例模板类
/// \see
template <class T> class Singleton {
private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

    static std::unique_ptr<std::once_flag> oc;

    using TSingletonPtr = std::unique_ptr<T>;
    static TSingletonPtr instance;

public:
    static const TSingletonPtr &getInstance() {
        std::call_once(*oc, [ & ]() {
            instance = std::move(std::make_unique<T>());

            atexit(Singleton<T>::releaseInstance);
        });
        return instance;
    }

private:
    static void releaseInstance() {
        instance.reset();
        oc.reset(new std::once_flag);
    }
};

template <typename T> std::unique_ptr<T> Singleton<T>::instance = nullptr;
template <typename T> std::unique_ptr<std::once_flag> Singleton<T>::oc = std::make_unique<std::once_flag>();

} // namespace Foundation
