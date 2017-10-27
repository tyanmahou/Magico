#pragma once
#include"utility.hpp"
#include<tuple>


//************************************************************************************************
//
//concept_map
//
//************************************************************************************************

namespace tc
{

	template<class Concept>
	struct concept_map
	{
		using is_default = void;

		template<class T>
		T&& operator =(T&& other)
		{
			return std::forward<T>(other);
		}
	};

	namespace detail
	{

		template<class Concept, class Type, class = void>
		struct concept_mapping_impl : concept_map<void>
		{
			using concept_map<void>::operator=;
		};
		template<class Concept, class Type>
		struct concept_mapping_impl<Concept, Type, void_t<decltype(concept_map<Concept>() = tc::val<Type>())>> :concept_map<Concept>
		{
			using concept_map<Concept>::operator=;
		};

		template<class... Type>
		decltype(auto) ref_make_tuple(Type&&... arg)
		{
			return std::tuple<Type...>(std::forward<Type>(arg)...);
		}

		template<class Concept, class ...Type>
		decltype(auto)  make_mapping_tuple(Type&&... value)
		{
			return ref_make_tuple((concept_mapping_impl<Concept, Type>() = value)...);
		}
	}//namespace detail


	 ///<summary>
	 ///�C���X�^���X�ɃR���Z�v�g�}�b�v��K��������
	 ///</summary>
	template<class Concept, class Type>
	auto concept_mapping(Type&& value)->decltype(detail::concept_mapping_impl<Concept, Type>() = value)
	{

		return detail::concept_mapping_impl<Concept, Type>() = value;
	}
	///<summary>
	///�C���X�^���X�ɃR���Z�v�g�}�b�v��K�������� �Ԃ�ltuple
	///</summary>
	template<template<class...>class Concept, class ...Type>
	decltype(auto) concept_mapping(Type&&... value)
	{
		using C = Concept<std::decay_t<Type>...>;
		return detail::make_mapping_tuple<C>(value...);
	}

	namespace detail
	{
		template<class Concept, class Arg>
		struct concept_mapped
		{
			using type = std::decay_t<decltype(tc::concept_mapping<Concept>(tc::val<Arg&>()))>;
		};

	}
	///<summary>
	///�R���Z�v�g�}�b�v��̌^
	///</summary>
	template<class Concept, class Arg>
	using concept_mapped_t = typename detail::concept_mapped<Concept, Arg>::type;


	///<summary>
	///�R���Z�v�g�}�b�v��K�p��̌^�Ƃ��Ĕ���
	///</summary>
	template<class Type>
	struct as_mapped
	{};

	namespace detail
	{
		template<class Concept, class Arg>
		struct as_mapped_if
		{
			using type = Arg;
		};
		template<class Concept, class Arg>
		struct as_mapped_if<Concept, as_mapped<Arg>>
		{
			using type = concept_mapped_t<Concept, Arg>;
		};
		template<class Concept, class Arg>
		using as_mapped_if_t = typename as_mapped_if<Concept, Arg>::type;

		template<class Arg>
		struct remove_mapped
		{
			using type = Arg;
		};
		template<class Arg>
		struct remove_mapped<as_mapped<Arg>>
		{
			using type = Arg;
		};

		template<class Arg>
		using remove_mapped_t = typename remove_mapped<Arg>::type;
	}

	///<summary>
	///Type��as_mapped�������ꍇ�}�b�s���O��̌^��
	///</summary>
	template<class Type, template<class...>class Concept, class ... Args>
	using as_mapped_if_t = typename tc::detail::as_mapped_if_t<Concept<typename tc::detail::remove_mapped_t<Args>...>, Type>;


}//namespace tc