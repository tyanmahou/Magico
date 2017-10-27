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
	///Concept�����ׂď����𖞂�����
	///</summary>
	template < class... Concept >
	using require = std::enable_if_t<And<Concept...>::value, std::nullptr_t >;

	///<summary>
	///����`�F�b�N�@Ret�F�Ԃ�l�̌^�@Concept ����
	///</summary>
	template<class Ret, class ...Concept>
	using where = std::enable_if_t<
		std::is_same<tc::require<Concept...>, std::nullptr_t>::value,
		Ret>;

	///<summary>
	///����`�F�b�N�@Ret�F�Ԃ�l�̌^�@Exp bool�l
	///</summary>
	template<class Ret, bool Exp>
	using where_bool = std::enable_if_t<Exp, Ret>;

}//namesapce tc
