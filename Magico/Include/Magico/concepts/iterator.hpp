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
		///<para>イテレーターかどうか</para>
		///<para>[ It ]</para>
		///</summary>
		MAGICO_CONCEPT(Iterator)
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
		///<para>入力イテレーターかどうか</para>
		///<para>[ It ]</para>
		///</summary>
		MAGICO_CONCEPT(InputIterator)
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
		///<para>出力イテレーターかどうか</para>
		///<para>[ It ]</para>
		///</summary>
		MAGICO_CONCEPT(OutputIterator)
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
		///<para>前方イテレーターかどうか</para>
		///<para>[ It ]</para>
		///</summary>
		MAGICO_CONCEPT(ForwardIterator)
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
		///<para>双方向イテレーターかどうか</para>
		///<para>[ It ]</para>
		///</summary>
		MAGICO_CONCEPT(BidirectionalIterator)
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
		///<para>ランダムアクセスイテレーターかどうか</para>
		///<para>[ It ]</para>
		///</summary>
		MAGICO_CONCEPT(RandomAccessIterator)
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
		///<para>イテレーターの値型がスワップ可能か</para>
		///<para>[It]</para>
		///</summary>
		MAGICO_CONCEPT(ValueSwappable)
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
		///<para>イテレーターをもつか</para>
		///<para>[Type]</para>
		///</summary>
		MAGICO_CONCEPT(HasIterator)
		{

			template<class Type>
			auto require()->decltype(
				magico::extends<Iterator>::require<typename Type::iterator>()
				);
		};

		///<summary>
		///<para>レンジかどうか</para>
		///<para>[Type]</para>
		///</summary>
		MAGICO_CONCEPT(Range)
		{
			template<class Type>
			auto require(Type&& range)->decltype(
				magico::extends<Iterator>::require(std::begin(range)),
				magico::extends<Iterator>::require(std::end(range))
				);
		};

	}//namesapace concepts
}//namespace magico