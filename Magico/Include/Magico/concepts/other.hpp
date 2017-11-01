#pragma once
#include"../concept.hpp"

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

	}//namespace concept
}//namespace magico
