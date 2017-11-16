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
MAGICO_CONCEPT(className)\
{\
private:\
	template<class Type,class Ret>\
	auto require_(Type& t)->decltype(magico::valid_expr<Ret>( symbol t )); \
	template<class Type>\
	auto require_(Type& t)->decltype(symbol t); \
public:\
	template<class Type,class ...Ret>\
	auto require(Type& t)->decltype(this->require_<Type,Ret...>(t));\
}

		///<summary>
		///<para>operator ! をもつか</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasLogicalNot, !);


		///<summary>
		///<para>単項operator + をもつか</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasUnaryPlus, +);

		///<summary>
		///<para>単項operator - をもつか</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasNegate, -);

		///<summary>
		///<para>前置インクリメント可能か</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasPreIncrement, ++);

		///<summary>
		///<para>前置デクリメント可能か</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasPreDecrement, --);

		///<summary>
		///<para>後置インクリメント可能か</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_CONCEPT(HasPostIncrement)
		{
		private:
			template<class Type, class Ret>
			auto require_(Type& t)->decltype(magico::valid_expr<Ret>(t++)); 
			template<class Type>
			auto require_(Type& t)->decltype(t++); 
		public:
			template<class Type, class ...Ret>
			auto require(Type& t)->decltype(this->require_<Type, Ret...>(t));
		};

		///<summary>
		///<para>後置デクリメント可能か</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_CONCEPT(HasPostDecrement)
		{
		private:
			template<class Type, class Ret>
			auto require_(Type& t)->decltype(magico::valid_expr<Ret>(t--));
			template<class Type>
			auto require_(Type& t)->decltype(t--);
		public:
			template<class Type, class ...Ret>
			   auto require(Type& t)->decltype(this->require_<Type, Ret...>(t));
		};
		///<summary>
		///<para>単項operator ~ をもつか</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasComplement, ~);

		///<summary>
		///<para>アドレス取得可能か</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasAddressOf, &);

		///<summary>
		///<para>オーバーロードされた&演算可能か</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_CONCEPT(HasOverloadedAddressOf)
		{
		private:
			template<class Type, class Ret>
			auto require_(Type& t)->decltype(magico::valid_expr<Ret>(t.operator&()));
			template<class Type>
			auto require_(Type& t)->decltype(t.operator&());
		public:
			template<class Type, class ...Ret>
			auto require(Type& t)->decltype(this->require_<Type, Ret...>(t));
		};

		///<summary>
		///<para>関接演算可能か</para>
		///<para>[ Type ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_UNARY_OPERATOR(HasIndirect, *);


#undef MAGICO_HAS_UNARY_OPERATOR

#define MAGICO_HAS_BINARY_OPERATOR(className,symbol)\
MAGICO_CONCEPT(className)\
{\
private:\
		template<class Left, class Right, class Ret>\
		auto require_(Left& a, Right& b)->decltype(magico::valid_expr<Ret>(a symbol b));\
		template<class Left, class Right = Left>\
		auto require_(Left& a, Right& b)->decltype( a symbol b);\
public:\
	template<class Left, class Right = Left, class... Ret>\
	auto require(Left&a, Right& b)->decltype(this->require_<Left, Right, Ret...>(a, b));\
}

		///<summary>
		///<para>代入演算可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasAssign, =);

		///<summary>
		///<para>加算可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasPlus, +);

		///<summary>
		///<para> 減算可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasMinus, -);

		///<summary>
		///<para>乗算可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasMultiply, *);

		///<summary>
		///<para> 除算可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasDivide, / );

		///<summary>
		///<para> 剰余可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasModulus, %);

		///<summary>
		///<para> 加算代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasPlusAssign, +=);

		///<summary>
		///<para> 減算代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasMinusAssign, -=);

		///<summary>
		///<para> 乗算代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasMultiplyAssign, *=);

		///<summary>
		///<para> 除算代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasDivideAssign, /=);

		///<summary>
		///<para> 剰余代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasModulusAssign, %=);

		///<summary>
		///<para> 左シフト可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLeftShift, << );

		///<summary>
		///<para> 右シフト可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasRightShift, >> );

		///<summary>
		///<para> 左シフト代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLeftShiftAssign, <<=);

		///<summary>
		///<para> 右シフト可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasRightShiftAssign, >>=);

		///<summary>
		///<para> ビットごとのAND演算可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitAnd, &);

		///<summary>
		///<para>ビットごとのOR演算可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitOr, | );

		///<summary>
		///<para> ビットごとの排他的OR演算可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitXor, ^);

		///<summary>
		///<para> ビットごとのAND演算代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitAndAssign, &=);

		///<summary>
		///<para> ビットごとのOR演算代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitOrAssign, |=);

		///<summary>
		///<para> ビットごとの排他的OR演算代入可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasBitXorAssign, ^=);

		///<summary>
		///<para> 論理積可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLogicalAnd, &&);

		///<summary>
		///<para> 論理和可能か</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLogicalOr, || );

		///<summary>
		///<para>小なり比較演算 をもつか</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLess, < );

		///<summary>
		///<para> 小なり=比較演算をもつか</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasLessEqual, <= );

		///<summary>
		///<para> 大なり比較演算をもつか</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasGreater, > );

		///<summary>
		///<para> 大なり=比較演算をもつか</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasGreaterEqual, >= );

		///<summary>
		///<para> operator == をもつか</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasEqualTo, == );

		///<summary>
		///<para> operator != をもつか</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_HAS_BINARY_OPERATOR(HasNotEqualTo, != );

		///<summary>
		///<para> operator , をもつか</para>
		///<para>[ Left,Right = Left ,Ret = _ ]</para>
		///</summary>
		MAGICO_CONCEPT(HasOverloadedComma)
		{
		private:
			template<class Left, class Right, class Ret>
			auto require_(Left& a, Right& b)->decltype(
				magico::valid_expr<Ret>(a.operator,(b))
				);
			template<class Left, class Right = Left>
			auto require_(Left& a, Right& b)->decltype(
				a.operator,(b)
				);
		public:
			template<class Left, class Right = Left, class... Ret>
			auto require(Left& a, Right& b)->decltype(
				this->require_<Left, Right, Ret...>(a, b)
				);
		};

		///<summary>
		///<para>operator [] をもつか</para>
		///<para>[ Type, Key = size_t ,Ret = _ ]</para>
		///</summary>
		MAGICO_CONCEPT(HasSubscript)
		{
		private:
			template<class Type, class Key, class Ret>
			auto require_(Type& a, Key& b)->decltype(
				magico::valid_expr<Ret>(a[b])
				);
			template<class Type, class Key = std::size_t>
			auto require_(Type& a, Key& b)->decltype(
				a[b]
				);
		public:
			template<class Type, class Key = std::size_t, class... Ret>
			auto require(Type&a, Key& b)->decltype(
				this->require_<Type, Key, Ret...>(a, b)
				);
		};

#undef MAGICO_HAS_BINARY_OPERATOR


	}//namesapace concepts
}//namespace magico