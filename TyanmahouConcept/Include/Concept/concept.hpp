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
	///require実装クラスをコンセプト(メタ関数)に変換
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
	///require実装クラスをコンセプト(メタ関数)に変換し継承 SubConceptには派生classを与える
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
	///alias形式constraintをコンセプト(メタ関数)に変換
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
	///alias形式constraintをコンセプト(メタ関数)に変換し継承 SubConceptには派生classを与える
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
	///通常のメタ関数をコンセプト(メタ関数)に変換
	///</summary>
	template<template<class...>class Meta, class ...Args>
	struct meta_to_concept : Meta<
		detail::as_mapped_if_t<
		meta_to_concept<Meta, detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};

	///<summary>
	///通常のメタ関数をコンセプト(メタ関数)に変換し継承 SubConceptには派生classを与える
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
	///制約の継承
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
	///Type型が存在するか
	///</summary>
	template<class Type>
	auto associated_type()->decltype(val<Type>());

	///<summary>
	///式が評価可能でRet型か
	///</summary>
	///<param name= "exp">
	///評価する式
	///</param>
	template<class Ret, class Exp>
	auto vailed_expr(Exp&& exp)->tc::require<std::is_same<Ret, Exp>>;


	constexpr detail::void_tester _void;

	template<class T>
	auto vailed_expr(detail::void_tester)->tc::require<std::is_void<T>>;

	///<summary>
	///式がType型に変更可能か
	///</summary>
	///<param name= "exp">
	///評価する式
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
///条件をみたさないとアサート
///</summary>
#define TC_CONCEPT_ASSERT( ... ) static_assert(tc::And<__VA_ARGS__>::value,#__VA_ARGS__ )

///<summary>
///条件をみたさないとアサート(bool)
///</summary>
#define TC_CONCEPT_ASSERT_BOOL( ... ) static_assert(static_cast<bool>(__VA_ARGS__),#__VA_ARGS__ )

///<summary>
///コンセプト生成
///</summary>
#define TC_CONCEPT(name,...)\
struct name : tc::to_concept_ex<name,struct __##name##_c,__VA_ARGS__>{};\
struct __##name##_c