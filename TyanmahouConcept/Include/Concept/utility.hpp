#pragma once
#include<type_traits>


//************************************************************************************************
//
//utility
//
//************************************************************************************************

namespace tc
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
	///���̃C���X�^���X�쐬(declval�̃w���p)
	///</summary>
	template<class Type>
	auto val()->decltype(std::declval<Type>());

	///<summary>
	///���^�֐� �_���a
	///</summary>
	template<class Head, class... Tail>
	struct Or :std::conditional_t <  Head::value, std::true_type, Or < Tail... > >
	{};
	template<class Concpet>
	struct Or<Concpet> :std::bool_constant<Concpet::value>
	{};
	///<summary>
	///���^�֐� �ے�
	///</summary>
	template < class Concpet >
	struct Not : std::bool_constant <!Concpet::value >
	{};
	///<summary>
	///���^�֐� �_����
	///</summary>
	template <class ... Args>
	struct And :Not < Or  < Not < Args >... > >
	{};

}//namesapce tc