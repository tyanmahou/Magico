#pragma once
#include"basic.hpp"
#include<istream>
#include<ostream>
#include<random>
namespace magico {
	namespace concepts {

		//************************************************************************************************
		//
		//Random Number Generation
		//
		//************************************************************************************************

		///<summary>
		///<para>32ビット符号なし整数値のシーケンスを生成するオブジェクトか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(SeedSequence)
		{
			using InputIterator = std::istreambuf_iterator<int>;
			using RandomIterator = int*;
			using OutputIterator = std::ostreambuf_iterator<int>;
			template<class T>
			using Result = typename T::result_type;


			template<class Type>
			auto require(Type s, const Type&cs,
				std::initializer_list<Result<Type>> il, InputIterator init, OutputIterator outit, RandomIterator rit)->decltype(
					Type(), Type(il), Type(init, init),
					valid_expr<void>((s.generate(rit, rit), is_void)),
					valid_expr<std::size_t>(cs.size()),
					valid_expr<void>((cs.param(outit), is_void))
					);
		};

		///<summary>
		///<para>一様分布の符号なし整数を生成するか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(UniformRandomBitGenerator)
		{
			template<class T>
			using Result = typename T::result_type;

			template<class Type>
			auto require(Type g)->decltype(
				valid_expr<Result<Type>>(Type::min()),
				valid_expr<Result<Type>>(Type::max()),
				valid_expr<Result<Type>>(g())
				);
		};

		///<summary>
		///<para>与えられた数学的確率密度関数に従って分布した乱数を返すか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(RandomNumberDistribution)
		{

			using Generator = std::random_device;
			using Is = std::basic_istream<int>;
			using Os = std::basic_ostream<int>;

			template<class T>
			using Result = typename T::result_type;
			template<class T>
			using Param = typename T::param_type;

			template<class Type>
			auto require(Type d, Param<Type> p, Generator g, Is is, Os os)->decltype(
				magico::extends<CopyConstructible, CopyAssignable>::require<Type>(),
				magico::extends<CopyConstructible, CopyAssignable, EqualityComparable>::require<Param<Type>>(),
				Type(), Type(p),
				valid_expr<void>((d.reset(), is_void)),
				valid_expr<Param<Type>>(d.param()),
				valid_expr<void>((d.param(p), is_void)),
				valid_expr<Result<Type>>(d(g)),
				valid_expr<Result<Type>>(d(g, p)),
				valid_expr<Result<Type>>(d.max()),
				valid_expr<Result<Type>>(d.min()),
				valid_expr<bool>(d == d), valid_expr<bool>(d != d),
				valid_expr<Os&>(os << d),
				valid_expr<Is&>(is >> d)
				);
		};

	}//namespace concepts
}//namespace magico
