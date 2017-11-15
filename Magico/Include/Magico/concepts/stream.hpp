#pragma once
#include"../concept.hpp"
#include<iostream>

namespace magico
{
	namespace concepts
	{

		///<summary>
		///<para>標準入力可能か</para>
		///<para>[ Type ,Elm = char,Traits = std::char_traits[Elm] ]</para>
		///</summary>
		MAGICO_CONCEPT(InputStreamable)
		{
			template<class Type, class Elm = char, class Traits = std::char_traits<Elm>>
			auto require(std::basic_istream<Elm,Traits>& is, Type t)->decltype(
				valid_expr<std::basic_istream<Elm, Traits>&>(is >> t)
				);
		};


		///<summary>
		///<para>標準出力可能か</para>
		///<para>[ Type ,Elm = char,Traits = std::char_traits[Elm]]</para>
		///</summary>
		MAGICO_CONCEPT(OutputStreamable)
		{
			template<class Type, class Elm = char, class Traits = std::char_traits<Elm>>
			auto require(std::basic_ostream<Elm, Traits>& os, Type t)->decltype(
				valid_expr<std::basic_ostream<Elm, Traits>&>(os << t)
				);
		};

		///<summary>
		///<para>標準入出力可能か</para>
		///<para>[ Type ,Elm = char,Traits = std::char_traits[Elm]]</para>
		///</summary>
		MAGICO_CONCEPT(Streamable)
		{
			template<class Type, class Elm = char, class Traits = std::char_traits<Elm>>
			auto require(Type t)->decltype(
				extends<InputStreamable, OutputStreamable>::require<Type, Elm, Traits>()
				);
		};
	}//namespace concept
}//namespace magico