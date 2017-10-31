#pragma once

#include<unordered_map>
#include"iterator.hpp"

namespace magico {
	namespace concepts {
		//************************************************************************************************
		//
		//Container
		//
		//************************************************************************************************


		///<summary>
		///コンテナかどうか
		///</summary>
		template<class X> MAGICO_CONCEPT(Container, X)
		{
			template<class X,
				class value_t = typename X::value_type,
				class ref = typename X::reference,
				class cref = typename X::const_reference,
				class itr = typename X::iterator,
				class citr = typename X::const_iterator,
				class dif = typename X::difference_type,
				class size = typename X::size_type,
				class extends = magico::require<
				Defaulmagicoonstructible<X>,
				CopyConstructible<X>,
				Destructible<X>,
				EqualityComparable<X>,
				CopyAssignable<X>
				>
			>
				auto require(X&& x)->decltype(
					magico::valid_expr<bool>(x != x),
					magico::valid_expr<itr>(x.begin()),
					magico::valid_expr<itr>(x.end()),
					magico::valid_expr<citr>(x.cbegin()),
					magico::valid_expr<citr>(x.cend()),
					magico::valid_expr<void>((x.swap(x), _void)),
					magico::valid_expr<void>((std::swap(x, x), _void)),
					magico::valid_expr<size>(x.size()),
					magico::valid_expr<size>(x.max_size()),
					magico::valid_expr<bool>(x.empty())
					);
		};

		///<summary>
		///前方イテレーターをもつコンテナかどうか
		///</summary>
		template<class X> MAGICO_CONCEPT(ForwardContainer, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<ForwardIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///ランダムアクセスイテレーターをもつコンテナかどうか
		///</summary>
		template<class X> MAGICO_CONCEPT(RandomAccessContainer, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<RandomAccessIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///リバースイテレーターをもつコンテナかどうか
		///</summary>
		template<class X> MAGICO_CONCEPT(ReversibleContainer, X)
		{
			template<class X,
				class ritr = typename X::reverse_iterator,
				class critr = typename X::const_reverse_iterator
			>
				auto require(X&& x)->decltype(
					magico::extends<Container>::require<X>(),
					magico::extends<BidirectionalIterator>::require<typename X::iterator>(),
					magico::valid_expr<ritr>(x.rbegin()),
					magico::valid_expr<ritr>(x.rend()),
					magico::valid_expr<critr>(x.crbegin()),
					magico::valid_expr<critr>(x.crend())
					);
		};


		///<summary>
		///任意のコンテナXに対して、その要素型をデフォルトで挿入可能か
		///</summary>
		template<class X> MAGICO_CONCEPT(DefaultInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<Defaulmagicoonstructible>::require<typename X::value_type>()
				);
		};

		///<summary>
		///任意のコンテナXに対して、その要素型のコピー挿入可能か
		///</summary>
		template<class X> MAGICO_CONCEPT(CopyInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<CopyConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///任意のコンテナXに対して、その要素型の右辺値オブジェクトをムーブ挿入可能か
		///</summary>
		template<class X> MAGICO_CONCEPT(MoveInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<MoveConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///任意のコンテナXに対して、要素型のコンストラクタ引数列Argsから直接構築可能か
		///</summary>
		template<class X, class... Args> MAGICO_CONCEPT(EmplaceConstructible, X, Args...)
		{
			template<class X, class... Args>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<Constructible>::require<typename X::value_type, Args...>()
				);
		};

		///<summary>
		///任意のコンテナXに対して、要素型の破棄が可能か
		///</summary>
		template<class X> MAGICO_CONCEPT(Erasable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<Destructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///アロケーターを認識するコンテナか
		///</summary>
		template<class X> MAGICO_CONCEPT(AllocatorAwareContainer, X)
		{
			template<class X, class alloc_t = typename X::allocator_type>
			auto require(X&& x)->decltype(
				magico::extends<Container>::require<X>(),
				magico::extends<CopyAssignable, MoveAssignable, CopyInsertable, MoveInsertable>::require<X>(),
				magico::extends<Defaulmagicoonstructible>::require<std::allocator<typename X::value_type>>(),
				magico::extends<Constructible>::require<X, std::allocator<typename X::value_type>>(),
				magico::extends<Constructible>::require<X, std::add_lvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
				magico::extends<Constructible>::require<X, std::add_rvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
				magico::valid_expr<alloc_t>(x.get_allocator()),
				magico::valid_expr<void>((x.swap(x), _void))
				);
		};
		///<summary>
		///線形保管するコンテナか
		///</summary>
		template<class X> MAGICO_CONCEPT(SequenceContainer, X)
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
					magico::valid_expr<void>((a.clear(), _void)),
					a.assign(it, it), a.assign(il), a.assign(n, t)
					);
		};
		///<summary>
		///キーに基づいて順序付けられたデータを検索するコンテナか
		///</summary>
		template<class X> MAGICO_CONCEPT(AssociativeContainer, X)
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
		///<summary>
		///キーに基づいて順序付けられていないデータを検索するコンテナか
		///</summary>
		template<class X> MAGICO_CONCEPT(UnorderedAssociativeContainer, X)
		{
			template<class X>
			struct GetValueType
			{
				using type = typename X::key_type;
			};
			template<class Key, class T, class Hash, class KeyEqual, class Allocator>
			struct GetValueType <std::unordered_map<Key, T, Hash, KeyEqual, Allocator> >
			{
				using type = std::pair<const Key, T>;
			};
			template<class Key, class T, class Hash, class KeyEqual, class Allocator>
			struct GetValueType <std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> >
			{
				using type = std::pair<const Key, T>;
			};
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
						magico::extends<IsSame>::require<Value, typename GetValueType<X>::type>(),
						X(), X(n), X(n, hf, eq), X(n, hf), X(it, it, n, hf, eq), X(it, it, n, hf),
						X(it, it, n), X(it, it), X(il), X(il, n), X(il, n, hf), X(il, n, hf, eq), X(b),
						magico::valid_expr<X&>(a = il),
						magico::valid_expr<X&>(a = b)
						);
		};
	}//namespace concept
}//namespace magico
