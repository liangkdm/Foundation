#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <mutex>
namespace Foundation {
/// \class template<class T>CSingleton
/// \brief ����ģ����,
/// \see
template <class T> class CSingleton {
protected:
    /// ���캯��
    CSingleton() = default;
    /// �������ƺ���
    CSingleton(const CSingleton &singleton) = default;
    /// ��ֵ����
    CSingleton &operator=(const CSingleton &singleton) = default;
    /// ��������
    ~CSingleton() = default;

public:
    /// ��õ���ʵ��ָ��
    /// return ����ʵ��ָ��
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
    /// ɾ������ʵ��
    static void RealseInstance() {
        if (nullptr != s_pInstance) {
            delete s_pInstance;
            s_pInstance = nullptr;
        }
    }

public:
    /// \class CDeleter
    /// \brief ����������Դ��ʼ�����ͷ�
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
    /// ����ʵ��ָ��
    static T *s_pInstance;
    /// ɾ����
    static CDeleter m_Deleter;
    static mutex m_mutex;
};

template <class T> T *CSingleton<T>::s_pInstance = nullptr;

template <class T> typename CSingleton<T>::CDeleter CSingleton<T>::m_Deleter;

template <class T> T *CSingleton<T>::m_mutex;

} // namespace Foundation

#endif //_SINGLETON_H
