#pragma once
#include"concept_map.hpp"
#include"require.hpp"

//************************************************************************************************
//
//constraint
//
//************************************************************************************************

namespace tc
{

	namespace detail
	{
		template<class Concept, class ... Args>
		using ConceptCheck = decltype(&Concept::template require<Args...>);

		struct void_tester
		{
			template<typename T>
			friend void operator,(T &&, void_tester);
		};
	}


	///<summary>
	///require�����N���X���R���Z�v�g(���^�֐�)�ɕϊ�
	///</summary>
	template<class Constraint, class ...Args>
	struct to_concept : is_detected<
		detail::ConceptCheck,
		Constraint,
		as_mapped_if_t<
		Args,
		to_concept,
		Constraint,
		Args...
		>...
	>
	{};
	///<summary>
	///require�����N���X���R���Z�v�g(���^�֐�)�ɕϊ����p�� SubConcept�ɂ͔h��class��^����
	///</summary>
	template<template<class...>class SubConcept, class Constraint, class ...Args>
	struct to_concept_ex : is_detected<
		detail::ConceptCheck,
		Constraint,
		detail::as_mapped_if_t<
		SubConcept<detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};
	///<summary>
	///alias�`��constraint���R���Z�v�g(���^�֐�)�ɕϊ�
	///</summary>
	template<template<class...>class Constraint, class ...Args>
	struct alias_to_concept : is_detected<
		Constraint,
		detail::as_mapped_if_t<
		alias_to_concept<Constraint, detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};
	///<summary>
	///alias�`��constraint���R���Z�v�g(���^�֐�)�ɕϊ����p�� SubConcept�ɂ͔h��class��^����
	///</summary>
	template<template<class...>class SubConcept, template<class...>class Constraint, class ...Args>
	struct alias_to_concept_ex : is_detected<
		Constraint,
		detail::as_mapped_if_t<
		SubConcept<detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};

	///<summary>
	///�ʏ�̃��^�֐����R���Z�v�g(���^�֐�)�ɕϊ�
	///</summary>
	template<template<class...>class Meta, class ...Args>
	struct meta_to_concept : Meta<
		detail::as_mapped_if_t<
		meta_to_concept<Meta, detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};

	///<summary>
	///�ʏ�̃��^�֐����R���Z�v�g(���^�֐�)�ɕϊ����p�� SubConcept�ɂ͔h��class��^����
	///</summary>
	template<template<class...>class SubConcept, template<class...>class Meta, class ...Args>
	struct meta_to_concept_ex : Meta<
		detail::as_mapped_if_t<
		SubConcept<detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};

	/**************************************************************

	require helper

	***************************************************************/

	///<summary>
	///����̌p��
	///</summary>
	template<template<class...>class ...Concepts>
	struct extends
	{
		template<class... Args>
		static auto require(Args&&...)->tc::require<Concepts<Args...>...>;
		template<class... Args>
		static auto require()->tc::require<Concepts<Args...>...>;

	};

	///<summary>
	///Type�^�����݂��邩
	///</summary>
	template<class Type>
	auto associated_type()->decltype(val<Type>());

	///<summary>
	///�����]���\��Ret�^��
	///</summary>
	///<param name= "exp">
	///�]�����鎮
	///</param>
	template<class Ret, class Exp>
	auto vailed_expr(Exp&& exp)->tc::require<std::is_same<Ret, Exp>>;


	constexpr detail::void_tester _void;

	template<class T>
	auto vailed_expr(detail::void_tester)->tc::require<std::is_void<T>>;

	///<summary>
	///����Type�^�ɕύX�\��
	///</summary>
	///<param name= "exp">
	///�]�����鎮
	///</param>
	template<class Type, class Exp>
	auto convertible_expr(Exp&& exp)->tc::require<std::is_convertible<Exp, Type>>;

}//namespace tc
//************************************************************************************************
//
//macro
//
//************************************************************************************************

///<summary>
///�������݂����Ȃ��ƃA�T�[�g
///</summary>
#define TC_CONCEPT_ASSERT( ... ) static_assert(tc::And<__VA_ARGS__>::value,#__VA_ARGS__ )

///<summary>
///�������݂����Ȃ��ƃA�T�[�g(bool)
///</summary>
#define TC_CONCEPT_ASSERT_BOOL( ... ) static_assert(static_cast<bool>(__VA_ARGS__),#__VA_ARGS__ )

///<summary>
///�R���Z�v�g����
///</summary>
#define TC_CONCEPT(name,...)\
struct name : tc::to_concept_ex<name,struct __##name##_c,__VA_ARGS__>{};\
struct __##name##_c