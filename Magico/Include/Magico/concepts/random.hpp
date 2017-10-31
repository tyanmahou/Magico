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
		///32ビット符号なし整数値のシーケンスを生成するオブジェクトか
		///</summary>
		template<class Type> MAGICO_CONCEPT(SeedSequence, Type)
		{
			using InputIterator = std::istreambuf_iterator<int>;
			using RandomIterator = int*;
			using OutputIterator = std::ostreambuf_iterator<int>;
			template<class Type,
				class Result = typename Type::result_type
			>
				auto require(Type s, const Type&cs,
					std::initializer_list<Result> il, InputIterator init, OutputIterator outit, RandomIterator rit)->decltype(
						Type(), Type(il), Type(init, init),
						valid_expr<void>((s.generate(rit, rit), is_void)),
						valid_expr<std::size_t>(cs.size()),
						valid_expr<void>((cs.param(outit), is_void))
						);
		};

		///<summary>
		///一様分布の符号なし整数を生成するか
		///</summary>
		template<class Type> MAGICO_CONCEPT(UniformRandomBitGenerator, Type)
		{
			template<class Type,
				class Result = typename Type::result_type
			>
				auto require(Type g)->decltype(
					valid_expr<Result>(Type::min()),
					valid_expr<Result>(Type::max()),
					valid_expr<Result>(g())
					);
		};

		///<summary>
		///与えられた数学的確率密度関数に従って分布した乱数を返すか
		///</summary>
		template<class Type> MAGICO_CONCEPT(RandomNumberDistribution, Type)
		{

			using Generator = std::random_device;
			using Is = std::basic_istream<int>;
			using Os = std::basic_ostream<int>;

			template<class Type,
				class Result = typename Type::result_type,
				class Param = typename Type::param_type
			>
				auto require(Type d, Param p, Generator g, Is is, Os os)->decltype(
					magico::extends<CopyConstructible, CopyAssignable>::require<Type>(),
					magico::extends<CopyConstructible, CopyAssignable, EqualityComparable>::require<Param>(),
					Type(), Type(p),
					valid_expr<void>((d.reset(), is_void)),
					valid_expr<Param>(d.param()),
					valid_expr<void>((d.param(p), is_void)),
					valid_expr<Result>(d(g)),
					valid_expr<Result>(d(g, p)),
					valid_expr<Result>(d.max()),
					valid_expr<Result>(d.min()),
					valid_expr<bool>(d == d), valid_expr<bool>(d != d),
					valid_expr<Os&>(os<<d),
					valid_expr<Is&>(is>>d)
					);
		};

	}//namespace concepts
}//namespace magico
