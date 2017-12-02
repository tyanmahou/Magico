#pragma once
#include<type_traits>


//************************************************************************************************
//
//utility
//
//************************************************************************************************

namespace magico
{


	template<class... Test>
	using void_t = void;

	namespace detail
	{

		template<class, template<class...>class Constraint, class ...Args>
		struct is_detected_impl :std::false_type
		{};

		template<template<class...>class Constraint, class ...Args>
		struct is_detected_impl<void_t<Constraint<Args...>>, Constraint, Args...> :std::true_type
		{};
	}

	//-- TODO std::is_detected

	template<template<class...>class Constraint, class ...Args>
	using is_detected = detail::is_detected_impl <void, Constraint, Args...>;



	///<summary>
	///仮のインスタンス作成(declvalのヘルパ)
	///</summary>
	template<class Type>
	auto val()->decltype(std::declval<Type>());

	///<summary>
	///メタ関数 論理和
	///</summary>
	template<class ... Concpets>
	using Or = std::disjunction<Concpets...>;
	///<summary>
	///メタ関数 否定
	///</summary>
	template < class Concept >
	using Not = std::negation<Concept>;
	///<summary>
	///メタ関数 論理積
	///</summary>
	template <class ... Concpets>
	using And = std::conjunction<Concpets...>;

}//namesapce magico
