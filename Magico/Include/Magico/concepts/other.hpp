#pragma once
#include"../concept.hpp"
#include"operator.hpp"
#include"basic.hpp"
namespace magico {
	namespace concepts {

		//************************************************************************************************
		//
		//Other
		//
		//************************************************************************************************
		///<summary>
		///条件式conditionalをみたすか concept_map不可
		///</summary>
		template<bool conditional>
		struct Condition : std::bool_constant<conditional>
		{};


		///<summary>
		///TypeがTemplate型であるか
		///</summary>
		template<template <class...>class Template, class Type>
		struct is_template : std::false_type
		{};
		template<template <class...>class Template, class... Args>
		struct is_template<Template, Template<Args...>> : std::true_type
		{};

		template<template <class...>class Template, class Type>
		constexpr bool is_template_v = is_template<Template, Type>::value;


		///<summary>
		///<para>bitマスクであるか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(BitmaskType)
		{
			using ex = extends<HasBitAnd, HasBitOr, HasBitXor, HasComplement,
				HasBitAndAssign, HasBitOrAssign, HasBitXorAssign>;

			template<class Type>
			auto require(Type x)->decltype(
				ex::require<Type>()
				);
		};

		///<summary>
		///<para>数値タイプであるか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(NumericType)
		{
			template<class Type>
			auto require(Type x,const Type& y)->decltype(
				where_bool<int,!std::is_reference_v<Type>&&!std::is_const_v<Type>&&!std::is_volatile_v<Type>>{},
				where_bool<int,Class_v<Type>?
				!HasOverloadedAddressOf_v<Type>&&!Abstract_v<Type>&&
				DefaultConstructible_v<Type>&&CopyConstructible_v<Type>&&Destructible_v<Type>
				:true>{},
				valid_expr<Type&>(x=y),
				valid_expr<Type&>(x=Type())
				);
		};
		///<summary>
		///<para>リテラルタイプであるか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(LiteralType)
		{
			template<class Type>
			auto require(Type x)->decltype(
				extends<std::is_literal_type>::require<Type>()
				);
		};

	}//namespace concept
}//namespace magico
