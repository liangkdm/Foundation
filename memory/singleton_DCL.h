#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <mutex>
namespace Foundation {
/// \class template<class T>CSingleton
/// \brief 单例模板类,
/// \see
template <class T> class CSingleton {
protected:
    /// 构造函数
    CSingleton() = default;
    /// 拷贝复制函数
    CSingleton(const CSingleton &singleton) = default;
    /// 赋值函数
    CSingleton &operator=(const CSingleton &singleton) = default;
    /// 析构函数
    ~CSingleton() = default;

public:
    /// 获得单例实例指针
    /// return 单例实例指针
    static T *GetInstance() {
        if (nullptr == s_pInstance) {
            lock_guard<mutex> lock{m_mutex};
            if (nullptr == s_pInstance) {

                s_pInstance = new T();

                return s_pInstance;
            }
        }

        return s_pInstance;
    }
    /// 删除单例实例
    static void RealseInstance() {
        if (nullptr != s_pInstance) {
            delete s_pInstance;
            s_pInstance = nullptr;
        }
    }

public:
    /// \class CDeleter
    /// \brief 负责单例的资源初始化和释放
    /// \see
    class CDeleter {
    public:
        CDeleter() {
            CSingleton<T>::GetInstance();
        }
        ~CDeleter() {
            CSingleton<T>::RealseInstance();
        }
    };

private:
    /// 单例实例指针
    static T *s_pInstance;
    /// 删除器
    static CDeleter m_Deleter;
    static mutex m_mutex;
};

template <class T> T *CSingleton<T>::s_pInstance = nullptr;

template <class T> typename CSingleton<T>::CDeleter CSingleton<T>::m_Deleter;

template <class T> T *CSingleton<T>::m_mutex;

} // namespace Foundation

#endif //_SINGLETON_H
