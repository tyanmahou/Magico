﻿#pragma once
#include<any>
#include"concept_map.hpp"
#include"require.hpp"

//************************************************************************************************
//
//concept_any
//
//************************************************************************************************

namespace magico
{
	namespace detail
	{
		//実装
		template<class T>
		struct AnyCastImpl
		{
			static T Func(std::any& _any)
			{
				using Type = std::remove_const_t<T>;
				if (auto& type = _any.type(); type == typeid(std::reference_wrapper<Type>))
				{
					return std::any_cast<std::reference_wrapper<Type>>(_any);
				}
				else if (type == typeid(std::reference_wrapper<const Type>))
				{
					return std::any_cast<std::reference_wrapper<const Type>>(_any);
				}
				return std::any_cast<T>(_any);
			}
		};
		//参照
		template<class T>
		struct AnyCastImpl<T&>
		{
			static T& Func(std::any& _any)
			{
				if (_any.type() == typeid(std::reference_wrapper<T>))
				{
					return std::any_cast<std::reference_wrapper<T>>(_any);
				}
				return std::any_cast<T&>(_any);
			}
		};
		//const参照
		template<class T>
		struct AnyCastImpl<const T&>
		{
			static const T& Func(std::any& _any)
			{
				if (auto& type = _any.type(); type == typeid(std::reference_wrapper<T>))
				{
					return std::any_cast<std::reference_wrapper<T>>(_any);
				}
				else if (type == typeid(std::reference_wrapper<const T>))
				{
					return std::any_cast<std::reference_wrapper<const T>>(_any);
				}
				return std::any_cast<T&>(_any);
			}
		};

		//ラップ関数
		template<class T>
		T any_cast_wrapper(std::any& _any)
		{
			return AnyCastImpl<T>::Func(_any);
		}

		//reference_wrapper をはずす
		template<class T>
		struct remove_reference_wrapper 
		{
			using type = T;
		};
		template<class T>
		struct remove_reference_wrapper<std::reference_wrapper<T>>
		{
			using type = T;
		};
		template<class T>
		using remove_reference_wrapper_t = typename remove_reference_wrapper<T>::type;
	}
	///<summary>
	///コンセプトを満たす型
	///</summary>
	template<template<class...>class Concept>
	class concept_any : protected std::any
	{
	public:

		concept_any() = default;

		template<class T, magico::require<Concept<magico::as_mapped<detail::remove_reference_wrapper_t<T>>>> = nullptr>
		concept_any(const T& v) :
			std::any(v)
		{}

		template<class T, magico::require<Concept<magico::as_mapped<detail::remove_reference_wrapper_t<T>>>> = nullptr>
		concept_any& operator=(const T& v)
		{
			return static_cast<concept_any&>(std::any::operator=(v));
		}
		using std::any::emplace;
		using std::any::has_value;
		using std::any::reset;
		using std::any::type;
		void swap(concept_any& any)
		{
			std::any::swap(any);
		}

		template<class T>
		decltype(auto) get()
		{
			return magico::concept_mapping<Concept<std::decay_t<T>>>(get_origin<T>());
		}
		template<class T>
		decltype(auto) get_origin()
		{
			return detail::any_cast_wrapper<T>(*this);
		}
	};

}//namespace magico
