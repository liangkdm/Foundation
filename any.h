#ifndef __ANY_H__
#define __ANY_H__
#include "TypeConf.h"
#include <iostream>

namespace Foundation
{

	/// \class Any
	/// \brief 任意类型数据存储类
	class Any
	{
	private:
		/// \class PlaceHolder
		/// \brief 泛型数据容器的基类
		class PlaceHolder
		{
		public:
			/// 虚析构函数
			virtual ~PlaceHolder(){};
		public:
			virtual const std::type_info & type() const = 0;
			/// 克隆函数
			virtual PlaceHolder *clone() const = 0;
		};

		/// \class Holder 
		/// \brief 泛型数据容器
		template<typename ValueType>
		class Holder:public PlaceHolder
		{
		public:
			/// 构造函数，调用存储数据的拷贝构造函数进行赋值
			Holder(const ValueType & value)
				:m_value(value)
			{
			}
		public:
			const std::type_info & type() const
			{
				return typeid(ValueType);
			}
			PlaceHolder *clone() const
			{
				return new Holder(m_value);	
			}
		private: 
			/// 禁止左值使用	
			/// 主要是避免当ValueType为holder同类型类型时无穷的递归调用
			Holder & operator=(const Holder &);
		public:
			/// 存储的数据
			ValueType m_value;
		};
	public:
		/// 默认构造函数
		Any()
			:m_Content(WP_NULL)
		{

		}
		/// 模板构造函数，接受任意数据类型
		template<typename ValueType>
		Any(const ValueType & value)
			:m_Content(new Holder<ValueType>(value))
		{
		}

		/// Any a(1);
		/// Any b(a);
		/// 程序就会挂掉，因为b变量保存的是Any类型，即m_Content是Any类型，b在调用析构函数时调用delete m_Content语句，
		/// 该语句又会调用m_Content的析构函数，因为这里的m_Content是Any类型，所以Any析构函数就陷入无穷递归调用。
		/// 因此，这里，需要定义Any另外一个接受Any类型参数的构造函数：
		/// 拷贝构函数,通过存在的泛型数据容器来创建新的数据容器
		/// 防止浅拷贝的情况发生
		Any(const Any&othre)
			:m_Content(othre.m_Content?othre.m_Content->clone():WP_NULL)
		{

		}
		/// 析构函数
		~Any()
		{
			delete m_Content;
		}
	public:
		/// 交换函数
		Any & swap(Any & rhs)
		{
			std::swap(m_Content, rhs.m_Content);
			return *this;
		}
		/// 赋值函数
		Any &operator= (const Any &other)
		{
			//重载=操作符，先是构造一个Any对象，swap交换对象指针后，原来需要释放的那个指针就到了临时对象Any(val)中  
			//函数返回后，临时变量自动析构，调用了析构函数释放这块内存，不会造成内存泄露。 
			Any(other).swap(*this);
			return *this;
		}
		/// 模板赋值函数
		template<typename ValueType>
		Any & operator=(const ValueType &value)
		{
			//重载=操作符，先是构造一个Any对象，swap交换对象指针后，原来需要释放的那个指针就到了临时对象Any(val)中  
			//函数返回后，临时变量自动析构，调用了析构函数释放这块内存，不会造成内存泄露。 
			Any(value).swap(*this);
			return *this;
		}
		/// 判断是否为空
		bool Empty() const
		{
			return (WP_NULL == m_Content)?true:false;
		}
		/// 清除
		void Clear()
		{
			Any().swap(*this);
		}
		/// 获得数据的类型信息
		const std::type_info & type() const
		{
			return m_Content ? m_Content->type() : typeid(void);
		}
	private:
		template <typename ValueType>
		friend ValueType* AnyCast(Any*);

		template <typename ValueType>
		friend ValueType* UnsafeAnyCast(Any*);

		/// 数据容器指针
		PlaceHolder *m_Content;
	};

	//隐藏标准的交换函数
	inline void swap(Any & lhs,Any & rhs)
	{
		lhs.swap(rhs);
	}

	template <typename ValueType>
	ValueType* AnyCast(Any* operand)
	{
		//运用RTTI进行安全检测
		return (operand && operand->type() == typeid(ValueType)) ? &static_cast<Any::Holder<ValueType> *>(operand->m_Content)->m_value : 0;
	}

	template <typename ValueType>
	const ValueType* AnyCast(const Any* operand)
	{
		return AnyCast<ValueType>(const_cast<Any*>(operand));
	}

	template <typename ValueType>
	ValueType AnyCast(const Any& operand)
	{
		ValueType* result = AnyCast<ValueType>(const_cast<Any*>(&operand));
		if (!result) 
		{
			throw std::bad_cast();
		}
		return *result;
	}


	template <typename ValueType>
	ValueType AnyCast(Any& operand)
	{
		ValueType* result = AnyCast<ValueType>(&operand);
		if (!result) 
		{
			return ValueType();
		}
		return *result;
	}

	template <typename ValueType>
	const ValueType& RefAnyCast(const Any & operand)
	{
		ValueType* result = AnyCast<ValueType>(const_cast<Any*>(&operand));
		if (!result) 
		{
			throw bad_cast();
		}
		return *result;
	}

	template <typename ValueType>
	ValueType& RefAnyCast(Any& operand)
	{
		ValueType* result = AnyCast<ValueType>(&operand);
		if (!result) 
		{
			throw bad_cast();
		}
		return *result;
	}


	template <typename ValueType>
	ValueType* UnsafeAnyCast(Any* operand)
	{
		return &static_cast<Any::Holder<ValueType>*>(operand->m_Content)->m_value;
	}


	template <typename ValueType>
	const ValueType* UnsafeAnyCast(const Any* operand)
	{
		return AnyCast<ValueType>(const_cast<Any*>(operand));
	}


}

#endif//__ANY_H__