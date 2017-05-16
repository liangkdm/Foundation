#ifndef __ANY_H__
#define __ANY_H__
#include "TypeConf.h"
#include <iostream>

namespace Foundation
{

	/// \class Any
	/// \brief �����������ݴ洢��
	class Any
	{
	private:
		/// \class PlaceHolder
		/// \brief �������������Ļ���
		class PlaceHolder
		{
		public:
			/// ����������
			virtual ~PlaceHolder(){};
		public:
			virtual const std::type_info & type() const = 0;
			/// ��¡����
			virtual PlaceHolder *clone() const = 0;
		};

		/// \class Holder 
		/// \brief ������������
		template<typename ValueType>
		class Holder:public PlaceHolder
		{
		public:
			/// ���캯�������ô洢���ݵĿ������캯�����и�ֵ
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
			/// ��ֹ��ֵʹ��	
			/// ��Ҫ�Ǳ��⵱ValueTypeΪholderͬ��������ʱ����ĵݹ����
			Holder & operator=(const Holder &);
		public:
			/// �洢������
			ValueType m_value;
		};
	public:
		/// Ĭ�Ϲ��캯��
		Any()
			:m_Content(WP_NULL)
		{

		}
		/// ģ�幹�캯��������������������
		template<typename ValueType>
		Any(const ValueType & value)
			:m_Content(new Holder<ValueType>(value))
		{
		}

		/// Any a(1);
		/// Any b(a);
		/// ����ͻ�ҵ�����Ϊb�����������Any���ͣ���m_Content��Any���ͣ�b�ڵ�����������ʱ����delete m_Content��䣬
		/// ������ֻ����m_Content��������������Ϊ�����m_Content��Any���ͣ�����Any������������������ݹ���á�
		/// ��ˣ������Ҫ����Any����һ������Any���Ͳ����Ĺ��캯����
		/// ����������,ͨ�����ڵķ������������������µ���������
		/// ��ֹǳ�������������
		Any(const Any&othre)
			:m_Content(othre.m_Content?othre.m_Content->clone():WP_NULL)
		{

		}
		/// ��������
		~Any()
		{
			delete m_Content;
		}
	public:
		/// ��������
		Any & swap(Any & rhs)
		{
			std::swap(m_Content, rhs.m_Content);
			return *this;
		}
		/// ��ֵ����
		Any &operator= (const Any &other)
		{
			//����=�����������ǹ���һ��Any����swap��������ָ���ԭ����Ҫ�ͷŵ��Ǹ�ָ��͵�����ʱ����Any(val)��  
			//�������غ���ʱ�����Զ����������������������ͷ�����ڴ棬��������ڴ�й¶�� 
			Any(other).swap(*this);
			return *this;
		}
		/// ģ�帳ֵ����
		template<typename ValueType>
		Any & operator=(const ValueType &value)
		{
			//����=�����������ǹ���һ��Any����swap��������ָ���ԭ����Ҫ�ͷŵ��Ǹ�ָ��͵�����ʱ����Any(val)��  
			//�������غ���ʱ�����Զ����������������������ͷ�����ڴ棬��������ڴ�й¶�� 
			Any(value).swap(*this);
			return *this;
		}
		/// �ж��Ƿ�Ϊ��
		bool Empty() const
		{
			return (WP_NULL == m_Content)?true:false;
		}
		/// ���
		void Clear()
		{
			Any().swap(*this);
		}
		/// ������ݵ�������Ϣ
		const std::type_info & type() const
		{
			return m_Content ? m_Content->type() : typeid(void);
		}
	private:
		template <typename ValueType>
		friend ValueType* AnyCast(Any*);

		template <typename ValueType>
		friend ValueType* UnsafeAnyCast(Any*);

		/// ��������ָ��
		PlaceHolder *m_Content;
	};

	//���ر�׼�Ľ�������
	inline void swap(Any & lhs,Any & rhs)
	{
		lhs.swap(rhs);
	}

	template <typename ValueType>
	ValueType* AnyCast(Any* operand)
	{
		//����RTTI���а�ȫ���
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