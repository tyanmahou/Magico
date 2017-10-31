#pragma once
#include"../concept.hpp"

namespace magico {
	namespace concepts
	{
		//************************************************************************************************
		//
		//Operator
		//
		//************************************************************************************************



#define MAGICO_HAS_UNARY_OPERATOR(className,symbol)\
template<class Type> MAGICO_CONCEPT(className,Type)\
{\
	template<class Type>\
	auto require(Type&& t)->decltype(symbol t);\
}

		///<summary>
		/// operator ! をもつか
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasLogicalNot, !);


		///<summary>
		/// 単項operator + をもつか
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasUnaryPlus, +);

		///<summary>
		/// 単項operator - をもつか
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasNegate, -);

		///<summary>
		/// 前置インクリメント可能か
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasPreIncrement, ++);

		///<summary>
		/// 前置デクリメント可能か
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasPreDecrement, --);

		///<summary>
		/// 後置インクリメント可能か
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(HasPostIncrement, Type)
		{
			template<class Type>
			auto require(Type&& t)->decltype(t++);
		};

		///<summary>
		/// 後置デクリメント可能か
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(HasPostDecrement, Type)
		{
			template<class Type>
			auto require(Type&& t)->decltype(t--);
		};
		///<summary>
		/// 単項operator ~ をもつか
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasComplement, ~);

		///<summary>
		/// アドレス取得可能か
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasAddressOf, &);

		///<summary>
		/// 関接演算可能か
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasIndirect, *);


#undef MAGICO_HAS_UNARY_OPERATOR

#define MAGICO_HAS_BINARY_OPERATOR(className,symbol)\
template<class Left,class Right=Left> \
MAGICO_CONCEPT(className,Left,Right)\
{\
	template<class Left,class Right>\
	auto require(Left&& l,Right&& r)->decltype(l symbol r);\
}

		///<summary>
		/// 代入演算可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasAssign, =);

		///<summary>
		/// 加算可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasPlus, +);

		///<summary>
		/// 減算可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasMinus, -);

		///<summary>
		/// 乗算可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasMultiply, *);

		///<summary>
		/// 除算可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasDivide, / );

		///<summary>
		/// 剰余可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasModulus, %);

		///<summary>
		/// 加算代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasPlusAssign, +=);

		///<summary>
		/// 減算代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasMinusAssign, -=);

		///<summary>
		/// 乗算代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasMultiplyAssign, *=);

		///<summary>
		/// 除算代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasDivideAssign, /=);

		///<summary>
		/// 剰余代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasModulusAssign, %=);

		///<summary>
		/// 左シフト可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLeftShift, << );

		///<summary>
		/// 右シフト可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasRightShift, >> );

		///<summary>
		/// 左シフト代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLeftShiftAssign, <<=);

		///<summary>
		/// 右シフト可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasRightShiftAssign, >>=);

		///<summary>
		/// ビットごとのAND演算可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitAnd, &);

		///<summary>
		/// ビットごとのOR演算可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitOr, | );

		///<summary>
		/// ビットごとの排他的OR演算可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitXor, ^);

		///<summary>
		/// ビットごとのAND演算代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitAndAssign, &=);

		///<summary>
		/// ビットごとのOR演算代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitOrAssign, |=);

		///<summary>
		/// ビットごとの排他的OR演算代入可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitXorAssign, ^=);

		///<summary>
		/// 論理積可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLogicalAnd, &&);

		///<summary>
		/// 論理和可能か
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLogicalOr, || );

		///<summary>
		/// 小なり比較演算 をもつか
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLess, <);

		///<summary>
		/// 小なり=比較演算 をもつか
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLessEqual, <= );

		///<summary>
		/// 大なり比較演算をもつか
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasGreater, >);

		///<summary>
		/// 大なり=比較演算をもつか
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasGreaterEqual, >= );

		///<summary>
		/// operator == をもつか
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasEqualTo, == );

		///<summary>
		/// operator != をもつか
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasNotEqualTo, != );

		///<summary>
		/// operator , をもつか
		///</summary>
		template<class Left, class Right = Left>
		MAGICO_CONCEPT(HasComma, Left, Right)
		{
			template<class Left, class Right>
			auto require(Left&& l, Right&& r)->decltype(l, r);
		};

		///<summary>
		/// operator [] をもつか
		///</summary>
		template<class Type, class Key = std::size_t>
		MAGICO_CONCEPT(HasSubscript, Type, Key)
		{
			template<class Type, class Key>
			auto require(Type t, Key key)->decltype(t[key]);
		};

#undef MAGICO_HAS_BINARY_OPERATOR


	}//namesapace concepts
}//namespace magico