#pragma once
#include"iterator.hpp"

namespace magico {
	namespace concepts {
		//************************************************************************************************
		//
		//Container
		//
		//************************************************************************************************


		///<summary>
		///<para>コンテナかどうか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(Container)
		{
			template<class X,
				class _value_t = typename X::value_type,
				class _ref = typename X::reference,
				class _cref = typename X::const_reference,
				class _itr = typename X::iterator,
				class _citr = typename X::const_iterator,
				class _dif = typename X::difference_type,
				class _size = typename X::size_type
			>
				struct GetType
			{
				using itr = _itr;
				using citr = _citr;
				using size = _size;
			};

			using ex = magico::extends<DefaultConstructible, CopyConstructible,
				Destructible, EqualityComparable, CopyAssignable>;

			template<class X>
			auto require(X&& x)->decltype(
				ex::require<X>(),
				magico::valid_expr<bool>(x != x),
				magico::valid_expr<typename GetType<X>::itr>(x.begin()),
				magico::valid_expr<typename GetType<X>::itr>(x.end()),
				magico::valid_expr<typename GetType<X>::citr>(x.cbegin()),
				magico::valid_expr<typename GetType<X>::citr>(x.cend()),
				magico::valid_expr<void>((x.swap(x), is_void)),
				magico::valid_expr<void>((std::swap(x, x), is_void)),
				magico::valid_expr<typename GetType<X>::size>(x.size()),
				magico::valid_expr<typename GetType<X>::size>(x.max_size()),
				magico::valid_expr<bool>(x.empty())
				);
		};

		///<summary>
		///<para>前方イテレーターをもつコンテナかどうか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(ForwardContainer)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<ForwardIterator>::require<typename X::iterator>()
				);
		};


		///<summary>
		///<para>双方向イテレーターをもつコンテナかどうか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(BidirectionalContainer)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<BidirectionalIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///<para>ランダムアクセスイテレーターをもつコンテナかどうか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(RandomAccessContainer)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<RandomAccessIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///<para>リバースイテレーターをもつコンテナかどうか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(ReversibleContainer)
		{

			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<BidirectionalIterator>::require<typename X::iterator>(),
				magico::valid_expr<typename X::reverse_iterator>(x.rbegin()),
				magico::valid_expr<typename X::reverse_iterator>(x.rend()),
				magico::valid_expr<typename X::const_reverse_iterator>(x.crbegin()),
				magico::valid_expr<typename X::const_reverse_iterator>(x.crend())
				);
		};


		///<summary>
		///<para>コンテナXに対して、要素型をデフォルトで挿入可能か</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(DefaultInsertable)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<DefaultConstructible>::require<typename X::value_type>()
				);
		};

		///<summary>
		///<para>コンテナXに対して、その要素型のコピー挿入可能か</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(CopyInsertable)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<CopyConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///<para>コンテナXに対して、その要素型の右辺値をムーブ挿入可能か</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(MoveInsertable)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<MoveConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///<para>コンテナXに対して、要素型のコンストラクタ引数列Argsから直接構築可能か</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(EmplaceConstructible)
		{
			template<class X, class... Args>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<Constructible>::require<typename X::value_type, Args...>()
				);
		};

		///<summary>
		///<para>コンテナXに対して、要素型の破棄が可能か</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(Erasable)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<Destructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///<para>アロケーターを認識するコンテナか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(AllocatorAwareContainer)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<CopyAssignable, MoveAssignable, CopyInsertable, MoveInsertable>::require<X>(),
				magico::extends<DefaultConstructible>::require<std::allocator<typename X::value_type>>(),
				magico::extends<Constructible>::require<X, std::allocator<typename X::value_type>>(),
				magico::extends<Constructible>::require<X, std::add_lvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
				magico::extends<Constructible>::require<X, std::add_rvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
				magico::valid_expr<typename X::allocator_type>(x.get_allocator()),
				magico::valid_expr<void>((x.swap(x), is_void))
				);
		};
		///<summary>
		///<para>線形保管するコンテナか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(SequenceContainer)
		{
			struct Tmp
			{
				//アロケーターの型
				template<class X, class = void>
				struct GetAllocator
				{
					using type = std::allocator<typename X::value_type>;
				};
				template<class X>
				struct GetAllocator<X, magico::void_t<typename X::allocator_type>>
				{
					using type = typename X::allocator_type;
				};

				template<class X,
					class A = typename GetAllocator<X>::type,
					class Value = typename X::value_type,
					class Size = typename X::size_type,
					class It = typename X::iterator,
					class CIt = typename X::const_iterator,
					class... Args
				>
					auto require(X a, Value t, Size n, It it, CIt cit, std::initializer_list<Value> il, Args&&... args)->decltype(
						magico::extends<Container>::require<X>(),
						X(n, t), X(it, it), X(il),
						magico::valid_expr<X&>(a = il),
						magico::valid_expr<It>(a.insert(cit, t)), magico::valid_expr<It>(a.insert(cit, n, t)),
						magico::valid_expr<It>(a.insert(cit, it, it)), magico::valid_expr<It>(a.insert(cit, il)),
						magico::valid_expr<It>(a.erase(cit)), magico::valid_expr<It>(a.erase(cit, cit)),
						magico::valid_expr<void>((a.clear(), is_void)),
						a.assign(it, it), a.assign(il), a.assign(n, t)
						);
			};
			template<class X>
			auto require()->decltype(&Tmp::require<X>);
		};

		///<summary>
		///<para>キーに基づいて順序付けられたデータを検索するコンテナか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(AssociativeContainer)
		{
			struct Tmp
			{
				template<class X,
					class Key = typename X::key_type,
					class Compare = typename X::key_compare,
					class Value = typename X::value_type,
					class ValueCompare = typename X::value_compare,
					class It = typename X::iterator
				>
					auto require(X a, Compare c, Value t, It it, std::initializer_list<Value> il)->decltype(
						magico::extends<BinaryPredicate>::require<ValueCompare, Value>(),
						X(), X(c), X(it, it, c), X(it, it), X(il),
						magico::valid_expr<X&>(a = il),
						magico::valid_expr<Compare>(a.key_comp()),
						magico::valid_expr<ValueCompare>(a.value_comp())
						);
			};
			template<class X>
			auto require()->decltype(&Tmp::template require<X>);

		};
		///<summary>
		///<para>キーに基づいて順序付けられていないデータを検索するコンテナか</para>
		///<para>[ X ]</para>
		///</summary>
		MAGICO_CONCEPT(UnorderedAssociativeContainer)
		{
			struct Tmp
			{
				template<class X,
					class Key = typename X::key_type,
					class Value = typename X::value_type,
					class Pred = typename X::key_equal,
					class Hash = typename X::hasher,
					class It = typename X::iterator,
					class LIt = typename X::local_iterator,
					class CLIt = typename X::const_local_iterator,
					class Size = typename X::size_type
				>
					auto require(X a, const X& b, const Hash& hf, const Pred& eq,
						Value t, It it, std::initializer_list<Value> il, Size n)->decltype(
							X(), X(n), X(n, hf, eq), X(n, hf), X(it, it, n, hf, eq), X(it, it, n, hf),
							X(it, it, n), X(it, it), X(il), X(il, n), X(il, n, hf), X(il, n, hf, eq), X(b),
							magico::valid_expr<X&>(a = il),
							magico::valid_expr<X&>(a = b)
							);
			};
			template<class X>
			auto require()->decltype(&Tmp::template require<X>);
		};
	}//namespace concepts
}//namespace magico
