#pragma once
#include"basic.hpp"
#include"operator.hpp"
namespace magico {
	namespace concepts
	{
		//************************************************************************************************
		//
		//Iterator
		//
		//************************************************************************************************

		///<summary>
		///イテレーターかどうか
		///</summary>
		template<class It> MAGICO_CONCEPT(Iterator, It)
		{
		private:
			using ex = magico::extends<
				CopyConstructible,
				CopyAssignable,
				Destructible,
				Swappable,
				HasIndirect
			>;
		public:
			template<class It>
			auto require(It it, typename std::iterator_traits<It>::value_type v)->decltype(
				ex::require<It>(),
				valid_expr<It&>(++it)
				);

		};


		///<summary>
		///入力イテレーターかどうか
		///</summary>
		template<class It> MAGICO_CONCEPT(InputIterator, It)
		{
		private:
			using ex = magico::extends<
				Iterator,
				EqualityComparable
			>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				magico::valid_expr<bool>(it != it),
				magico::valid_expr<typename std::iterator_traits<It>::value_type>(*it),
				magico::valid_expr<typename std::iterator_traits<It>::value_type>(*it++),
				magico::valid_expr<typename std::iterator_traits<It>::reference>(*it)
				);
		};


		///<summary>
		///出力イテレーターかどうか
		///</summary>
		template<class It> MAGICO_CONCEPT(OutputIterator, It)
		{
		private:
			using ex = magico::extends<Iterator>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				magico::valid_expr<const It&>(it++),
				magico::valid_expr<It&>(++it),
				*it = *it,
				*it++ = *it
				);
		};


		///<summary>
		///前方イテレーターかどうか
		///</summary>
		template<class It> MAGICO_CONCEPT(ForwardIterator, It)
		{
		private:
			using ex = magico::extends<InputIterator, DefaultConstructible>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				magico::valid_expr<It>(it++),
				magico::valid_expr<typename std::iterator_traits<It>::reference>(*it++)
				);
		};


		///<summary>
		///双方向イテレーターかどうか
		///</summary>
		template<class It> MAGICO_CONCEPT(BidirectionalIterator, It)
		{
		private:
			using ex = magico::extends<ForwardIterator>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				magico::valid_expr<const It&>(it--),
				magico::valid_expr<It&>(--it),
				magico::valid_expr<typename std::iterator_traits<It>::reference>(*it--)
				);
		};

		///<summary>
		///ランダムアクセスイテレーターかどうか
		///</summary>
		template<class It> MAGICO_CONCEPT(RandomAccessIterator, It)
		{
		private:
			using ex = magico::extends<
				BidirectionalIterator,
				LessThanComparable
			>;
		public:
			template<class It>
			auto require(It&& it, typename std::iterator_traits<It>::difference_type n)->decltype(
				ex::require<It>(),
				magico::valid_expr<bool>(it <= it),
				magico::valid_expr<bool>(it > it),
				magico::valid_expr<bool>(it >= it),
				magico::valid_expr<It&>(it += n),
				magico::valid_expr<It>(it + n), magico::valid_expr<It>(n + it),
				magico::valid_expr<It&>(it -= n),
				magico::valid_expr<It>(it - n),
				magico::valid_expr<typename std::iterator_traits<It>::reference>(it[n])
				);

		};

		///<summary>
		///イテレーターの値型がスワップ可能か
		///</summary>
		template<class It> MAGICO_CONCEPT(ValueSwappable, It)
		{
		private:
			using ex = magico::extends<Iterator>;
		public:
			template<class It>
			auto require(It it)->decltype(
				ex::require<It>(),
				magico::extends<Swappable>::require<typename std::iterator_traits<It>::value_type>()
				);
		};

		///<summary>
		///イテレーターをもつか
		///</summary>
		template<class Type> MAGICO_CONCEPT(HasIterator, Type)
		{

			template<class Type>
			auto require()->decltype(
				magico::extends<Iterator>::require<typename Type::iterator>()
				);
		};

		///<summary>
		///レンジかどうか
		///</summary>
		template<class Type> MAGICO_CONCEPT(Range, Type)
		{
			template<class Type>
			auto require(Type&& range)->decltype(
				magico::extends<Iterator>::require(std::begin(range)),
				magico::extends<Iterator>::require(std::end(range))
				);
		};

	}//namesapace concepts
}//namespace magico