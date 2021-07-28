
/*
C++11�涨��local static�ڶ��߳������µĳ�ʼ����Ϊ��Ҫ���������֤���ڲ���̬�������̰߳�ȫ�ԡ�
��C++11��׼�£���Effective C++�������һ�ָ����ŵĵ���ģʽʵ�֣�ʹ�ú����ڵ� local static����
������ֻ�е���һ�η���getInstance()����ʱ�Ŵ���ʵ�������ַ���Ҳ����ΪMeyers' Singleton��
C++0x֮���ʵ�����̰߳�ȫ�ģ�C++0x֮ǰ�������
*/
namespace Foundation {
/// \class template<class T>CSingleton
/// \brief ����ģ����,
/// \see
template <class T>
class Singleton {
private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton &) = default;
    Singleton &operator=(const Singleton &) = default;

public:
    static T &getInstance() {
        static T instance;
        return instance;
    }
};

} // namespace Foundation