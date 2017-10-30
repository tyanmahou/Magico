#pragma once
#include"utility.hpp"

//************************************************************************************************
//
//requires
//
//************************************************************************************************
namespace tc
{

	///<summary>
	///Conceptがすべて条件を満たす場合　nullptr型になる
	///</summary>
	template < class... Concept >
	using require = std::enable_if_t<And<Concept...>::value, std::nullptr_t >;

	///<summary>
	///制約チェック
	///[Ret]：返り値の型　[Concept] 制約
	///</summary>
	template<class Ret, class ...Concept>
	using where = std::enable_if_t<
		std::is_same<tc::require<Concept...>, std::nullptr_t>::value,
		Ret>;

	///<summary>
	///制約チェック　Ret：返り値の型　Exp bool値
	///</summary>
	template<class Ret, bool Exp>
	using where_bool = std::enable_if_t<Exp, Ret>;

}//namesapce tc
