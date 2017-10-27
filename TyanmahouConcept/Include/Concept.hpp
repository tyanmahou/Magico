#pragma once
#include<Concept/concept.hpp>

//************************************************************************************************
//
//concept example 
//
//************************************************************************************************

namespace tc {
	namespace concept {
		//************************************************************************************************
		//
		//Operator
		//
		//************************************************************************************************



#define TC_HAS_UNARY_OPERATOR(className,symbol)\
template<class Type> TC_CONCEPT(className,Type)\
{\
	template<class Type>\
	auto require(Type&& t)->decltype(symbol t);\
}

		///<summary>
		/// operator ! をもつか
		///</summary>
		TC_HAS_UNARY_OPERATOR(Negatable, !);


		///<summary>
		/// 単項operator + をもつか
		///</summary>
		TC_HAS_UNARY_OPERATOR(UnaryPlusable, +);

		///<summary>
		/// 単項operator - をもつか
		///</summary>
		TC_HAS_UNARY_OPERATOR(UnaryMinusable, -);

		///<summary>
		/// 前置インクリメント可能か
		///</summary>
		TC_HAS_UNARY_OPERATOR(PreIncrementable, ++);

		///<summary>
		/// 前置デクリメント可能か
		///</summary>
		TC_HAS_UNARY_OPERATOR(PreDecrementable, --);

		///<summary>
		/// 後置インクリメント可能か
		///</summary>
		template<class Type>
		TC_CONCEPT(PostIncrementable, Type)
		{
			template<class Type>
			auto require(Type&& t)->decltype(t++);
		};

		///<summary>
		/// 後置デクリメント可能か
		///</summary>
		template<class Type>
		TC_CONCEPT(PostDecrementable, Type)
		{
			template<class Type>
			auto require(Type&& t)->decltype(t--);
		};
		///<summary>
		/// 単項operator ~ をもつか
		///</summary>
		TC_HAS_UNARY_OPERATOR(Complementable, ~);

		///<summary>
		/// アドレス取得可能か
		///</summary>
		TC_HAS_UNARY_OPERATOR(AddressObtainable, &);

		///<summary>
		/// 関接演算可能か
		///</summary>
		TC_HAS_UNARY_OPERATOR(Indirectable, *);


#undef TC_HAS_UNARY_OPERATOR

#define TC_HAS_BINARY_OPERATOR(className,symbol)\
template<class Left,class Right=Left> \
TC_CONCEPT(className,Left,Right)\
{\
	template<class Left,class Right>\
	auto require(Left&& l,Right&& r)->decltype(l symbol r);\
}

		///<summary>
		/// 加算可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(Plusable, +);

		///<summary>
		/// 減算可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(Minusable, -);

		///<summary>
		/// 乗算可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(Multipliable, *);

		///<summary>
		/// 除算可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(Dividable, / );

		///<summary>
		/// 剰余可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(Modulable, %);

		///<summary>
		/// 加算代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(PlusAssignable, +=);

		///<summary>
		/// 減算代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(MinusAssignable, -=);

		///<summary>
		/// 乗算代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(MultiplyAssignable, *=);

		///<summary>
		/// 除算代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(DivideAssignable, /=);

		///<summary>
		/// 剰余代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(ModulusAssignable, %=);

		///<summary>
		/// 左シフト可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(LeftShiftable, << );

		///<summary>
		/// 右シフト可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(RightShiftable, >> );

		///<summary>
		/// 左シフト代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(LeftShiftAssignable, <<=);

		///<summary>
		/// 右シフト可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(RightShifAssignable, >>=);

		///<summary>
		/// ビットごとのAND演算可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitAndPossible, &);

		///<summary>
		/// ビットごとのOR演算可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitOrPossible, | );

		///<summary>
		/// ビットごとの排他的OR演算可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitXorPossible, ^);

		///<summary>
		/// ビットごとのAND演算代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitAndAssignable, &=);

		///<summary>
		/// ビットごとのOR演算代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitOrAssignable, |=);

		///<summary>
		/// ビットごとの排他的OR演算代入可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitXorAssignable, ^=);

		///<summary>
		/// 論理積可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(LogicalAndPossible, &&);

		///<summary>
		/// 論理和可能か
		///</summary>
		TC_HAS_BINARY_OPERATOR(LogicalOrPossible, || );

		///<summary>
		/// 小なり比較演算 をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessThanComparable, < );

		///<summary>
		/// 小なり=比較演算 をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessEqualComparable, <= );

		///<summary>
		/// 大なり比較演算をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(GreaterThanComparable, > );

		///<summary>
		/// 大なり=比較演算をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(GreaterEqualComparable, >= );

		///<summary>
		/// operator == をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(EqualityComparable, == );

		///<summary>
		/// operator != をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(NotEqualityComparable, != );

#undef TC_HAS_BINARY_OPERATOR

		//************************************************************************************************
		//
		//Basic
		//
		//************************************************************************************************

		///<summary>
		/// operator =(copy) をもつか
		///</summary>
		template< class To, class From = To> TC_CONCEPT(CopyAssignable, To, From)
		{
			template<class To, class From>
			auto require(To& t, const From& f)->decltype(
				tc::extends<std::is_assignable>::require(t, f)
				);
		};

		///<summary>
		/// operator =(move) をもつか
		///</summary>
		template< class To, class From = To> TC_CONCEPT(MoveAssignable, To, From)
		{
			template<class To, class From>
			auto require(To t, From f)->decltype(
				tc::extends<std::is_assignable>::require<
				std::add_lvalue_reference_t<To>,
				std::add_rvalue_reference_t<From>
				>()
				);
		};

		///<summary>
		/// Type( Args... ) の形式のコンストラクタ呼び出しが可能か
		///</summary>
		template< class Type, class... Args> TC_CONCEPT(Constructible, Type, Args...)
		{
			template<class Type, class... Args>
			auto require()->decltype(
				tc::extends<std::is_constructible>::require<Type, Args...>()
				);
		};

		///<summary>
		/// デフォルトコンストラクタ をもつか
		///</summary>
		template< class Type > TC_CONCEPT(DefaultConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_default_constructible>::require<Type>()
				);
		};
		///<summary>
		/// コピーコンストラクタ をもつか
		///</summary>
		template< class Type > TC_CONCEPT(CopyConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_copy_constructible>::require<Type>()
				);
		};
		///<summary>
		/// ムーブコンストラクタ をもつか
		///</summary>
		template< class Type > TC_CONCEPT(MoveConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_move_constructible>::require<Type>()
				);
		};
		///<summary>
		/// デストラクタ をもつか
		///</summary>
		template< class Type > TC_CONCEPT(Destructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_destructible>::require<Type>()
				);
		};
		///<summary>
		/// 仮想デストラクタ をもつか
		///</summary>
		template< class Type > TC_CONCEPT(HasVirtualDestructor, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::has_virtual_destructor>::require<Type>()
				);
		};

		///<summary>
		///TとUが同じか
		///</summary>
		template<class T, class U> TC_CONCEPT(IsSame, T, U)
		{
			template<class T, class U>
			auto require()->decltype(
				tc::extends<std::is_same>::require<T, U>()
				);
		};

		///<summary>
		///TypeがBase,もしくはBaseを継承しているか
		///</summary>
		template<class Type, class Base> TC_CONCEPT(Extended, Type, Base)
		{
			template<class Type, class Base>
			auto require()->decltype(
				tc::extends<std::is_base_of>::require<Base, Type>()
				);
		};

		///<summary>
		///スカラーかどうか
		///</summary>
		template< class Type > TC_CONCEPT(Scalar, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_scalar>::require<Type>()
				);
		};

		///<summary>
		///仮想クラスかどうか
		///</summary>
		template< class Type > TC_CONCEPT(Abstract, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_abstract>::require<Type>()
				);
		};

		///<summary>
		///enum型か
		///</summary>
		template< class Type > TC_CONCEPT(Enum, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_enum>::require<Type>()
				);
		};

		///<summary>
		///class(struct)型か
		///</summary>
		template< class Type > TC_CONCEPT(Class, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_class>::require<Type>()
				);
		};

		///<summary>
		///union型か
		///</summary>
		template< class Type > TC_CONCEPT(Union, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_union>::require<Type>()
				);
		};

		///<summary>
		///関数型t(...)か
		///</summary>
		template< class Type > TC_CONCEPT(Function, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_function>::require<Type>()
				);
		};

		//************************************************************************************************
		//
		//Library-wide
		//
		//************************************************************************************************


		///<summary>
		///アロケーターか
		///</summary>
		template<class Type> TC_CONCEPT(Allocator, Type)
		{
		private:
			size_t n;
			using ex = tc::extends<EqualityComparable, NotEqualityComparable, CopyConstructible, MoveConstructible>;
		public:
			template<class Type>
			auto require(Type& alloc, typename Type::value_type* ptr, const typename Type::value_type* cptr)->decltype(
				ex::require<Type>(),
				vailed_expr< typename Type::value_type& >(*ptr),
				vailed_expr< const typename Type::value_type& >(*cptr),
				vailed_expr< typename Type::value_type* >(alloc.allocate(n)),
				alloc.deallocate(ptr, n)
				);
		};

		///<summary>
		///TのオブジェクトとUのオブジェクトが入れ替え可能か
		///</summary>
		template<class T, class U = T> TC_CONCEPT(Swappable, T, U)
		{
			template<class T, class U>
			auto require(T&& a, U&& b)->decltype(
				std::swap(a, b),
				std::swap(b, a)
				);

		};

		///<summary>
		/// FromがToにキャストできるか
		///</summary>
		template< class From, class To > TC_CONCEPT(Convertible, From, To)
		{
			template< class From, class To >
			auto require()->decltype(
				tc::extends<std::is_convertible>::require<Type>()
				);
		};

		///<summary>
		/// null許容か
		///</summary>
		template<class Type> TC_CONCEPT(NullablePointer, Type)
		{
		private:
			using ex = tc::extends<
				EqualityComparable,
				NotEqualityComparable,
				DefaultConstructible,
				CopyConstructible,
				CopyAssignable,
				Destructible
			>;
		public:
			template<class Type>
			auto require(Type&& null)->decltype(
				ex::require<Type>(),
				tc::extends<Constructible>::require<Type, std::nullptr_t>(),
				null = nullptr,
				null == nullptr, nullptr == null,
				null != nullptr, nullptr != null
				);

		};

		///<summary>
		///関数呼び出し可能な型か
		///</summary>
		template <class F, class... Args>TC_CONCEPT(Invocable, F, Args...)
		{
			template <class F, class... Args>
			auto require(F&& func, Args&&... args)->decltype(
				std::invoke(func, args...)
				);

		};

		///<summary>
		///関数オブジェクトか
		///</summary>
		template <class Type> TC_CONCEPT(FunctionObject, Type)
		{
			template <class Type>
			auto require()->decltype(
				&Type::operator()
				);
		};

		///<summary>
		///メタ関数か
		///</summary>
		template <class Type> TC_CONCEPT(MetaFunc, Type)
		{
			template <class Type>
			auto require()->decltype(
				vailed_expr<const bool&>(Type::value)
				);
		};

		///<summary>
		///ハッシュ関数オブジェクトか
		///</summary>
		template <class Type, class Key> TC_CONCEPT(Hash, Type, Key)
		{
		private:
			using ex = tc::extends<CopyConstructible, Destructible>;
		public:
			template <class Type, class Key>
			auto require(Type&& t, Key&& key)->decltype(
				ex::require<Type>(),
				vailed_expr<size_t>(t(key))
				);

		};

		///<summary>
		///期間、時刻、現在の時刻を取得可能か
		///</summary>
		template <class Type> TC_CONCEPT(Clock, Type)
		{
			template <class Type>
			auto require(Type&& t)->decltype(
				tc::associated_type<typename Type::rep>(),
				tc::associated_type<typename Type::period>(),
				tc::associated_type<typename Type::duration>(),
				tc::associated_type<typename Type::time_point>(),
				vailed_expr<const bool&>(t.is_steady),
				vailed_expr<typename Type::time_point>(t.now())
				);

		};

		//************************************************************************************************
		//
		//Layout
		//
		//************************************************************************************************

		///<summary>
		///トリビアルコピー可能か
		///</summary>
		template< class Type > TC_CONCEPT(TriviallyCopyable, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_trivially_copyable>::require<Type>()
				);
		};

		///<summary>
		///トリビアル型か
		///</summary>
		template< class Type > TC_CONCEPT(TrivialType, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_trivial>::require<Type>()
				);
		};
		///<summary>
		///標準レイアウト型か
		///</summary>
		template<class Type>TC_CONCEPT(StandardLayoutType, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_standard_layout>::require<Type>()
				);
		};

		///<summary>
		///POD型か
		///</summary>
		template<class Type>TC_CONCEPT(PODType, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_pod>::require<Type>()
				);
		};

		//************************************************************************************************
		//
		//Iterator
		//
		//************************************************************************************************


		///<summary>
		///イテレーターかどうか
		///</summary>
		template<class It> TC_CONCEPT(Iterator, It)
		{
		private:
			using ex = tc::extends<
				CopyConstructible,
				CopyAssignable,
				Destructible,
				Swappable,
				Indirectable
			>;
		public:
			template<class It>
			auto require(It it, typename std::iterator_traits<It>::value_type v)->decltype(
				ex::require<It>(),
				vailed_expr<It&>(++it)
				);

		};


		///<summary>
		///入力イテレーターかどうか
		///</summary>
		template<class It> TC_CONCEPT(InputIterator, It)
		{
		private:
			using ex = tc::extends<
				Iterator,
				EqualityComparable,
				NotEqualityComparable
			>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				tc::convertible_expr<typename std::iterator_traits<It>::value_type>(*it),
				tc::convertible_expr<typename std::iterator_traits<It>::value_type>(*it++),
				tc::vailed_expr<typename std::iterator_traits<It>::reference>(*it)
				);
		};


		///<summary>
		///出力イテレーターかどうか
		///</summary>
		template<class It> TC_CONCEPT(OutputIterator, It)
		{
		private:
			using ex = tc::extends<Iterator>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				tc::convertible_expr<const It&>(it++),
				tc::vailed_expr<It&>(++it),
				*it = *it,
				*it++ = *it
				);
		};


		///<summary>
		///前方イテレーターかどうか
		///</summary>
		template<class It> TC_CONCEPT(ForwardIterator, It)
		{
		private:
			using ex = tc::extends<InputIterator, DefaultConstructible>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				tc::vailed_expr<It>(it++),
				tc::vailed_expr<typename std::iterator_traits<It>::reference>(*it++)
				);
		};


		///<summary>
		///双方向イテレーターかどうか
		///</summary>
		template<class It> TC_CONCEPT(BidirectionalIterator, It)
		{
		private:
			using ex = tc::extends<ForwardIterator>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				tc::convertible_expr<const It&>(it--),
				tc::vailed_expr<It&>(--it),
				tc::vailed_expr<typename std::iterator_traits<It>::reference>(*it--)
				);
		};

		///<summary>
		///ランダムアクセスイテレーターかどうか
		///</summary>
		template<class It> TC_CONCEPT(RandomAccessIterator, It)
		{
		private:
			using ex = tc::extends<
				BidirectionalIterator,
				LessThanComparable,
				LessEqualComparable,
				GreaterThanComparable,
				GreaterEqualComparable
			>;
		public:
			template<class It>
			auto require(It&& it, typename std::iterator_traits<It>::difference_type n)->decltype(
				ex::require<It>(),
				tc::vailed_expr<It&>(it += n),
				tc::vailed_expr<It>(it + n), tc::vailed_expr<It>(n + it),
				tc::vailed_expr<It&>(it -= n),
				tc::vailed_expr<It>(it - n),
				tc::convertible_expr<typename std::iterator_traits<It>::reference>(it[n])
				);

		};



		///<summary>
		///イテレーターの値型がスワップ可能か
		///</summary>
		template<class It> TC_CONCEPT(ValueSwappable, It)
		{
		private:
			using ex = tc::extends<Iterator>;
		public:
			template<class It>
			auto require(It it)->decltype(
				ex::require<It>(),
				tc::extends<Swappable>::require<typename std::iterator_traits<It>::value_type>()
				);
		};

		///<summary>
		///イテレーターをもつか
		///</summary>
		template<class Type> TC_CONCEPT(HasIterator, Type)
		{

			template<class Type>
			auto require()->decltype(
				tc::extends<Iterator>::require<typename Type::iterator>()
				);
		};

		//************************************************************************************************
		//
		//コンテナ
		//
		//************************************************************************************************


		///<summary>
		///コンテナかどうか
		///</summary>
		template<class X> TC_CONCEPT(Container, X)
		{
			template<class X,
				class value_t = typename X::value_type,
				class ref = typename X::reference,
				class cref = typename X::const_reference,
				class itr = typename X::iterator,
				class citr = typename X::const_iterator,
				class dif = typename X::difference_type,
				class size = typename X::size_type,
				class extends = tc::require<
				DefaultConstructible<X>,
				CopyConstructible<X>,
				Destructible<X>,
				EqualityComparable<X>,
				NotEqualityComparable<X>,
				CopyAssignable<X>
				>
			>
				auto require(X&& x)->decltype(
					tc::vailed_expr<itr>(x.begin()),
					tc::vailed_expr<itr>(x.end()),
					tc::vailed_expr<citr>(x.cbegin()),
					tc::vailed_expr<citr>(x.cend()),
					tc::vailed_expr<void>((x.swap(x), _void)),
					tc::vailed_expr<void>((std::swap(x, x), _void)),
					tc::vailed_expr<size>(x.size()),
					tc::vailed_expr<size>(x.max_size()),
					tc::convertible_expr<bool>(x.empty())
					);
		};

		///<summary>
		///前方イテレーターをもつコンテナかどうか
		///</summary>
		template<class X> TC_CONCEPT(ForwardContainer, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<ForwardIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///ランダムアクセスイテレーターをもつコンテナかどうか
		///</summary>
		template<class X> TC_CONCEPT(RandomAccessContainer, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<RandomAccessIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///リバースイテレーターをもつコンテナかどうか
		///</summary>
		template<class X> TC_CONCEPT(ReversibleContainer, X)
		{
			template<class X,
				class ritr = typename X::reverse_iterator,
				class critr = typename X::const_reverse_iterator
			>
				auto require(X&& x)->decltype(
					tc::extends<Container>::require<X>(),
					tc::extends<BidirectionalIterator>::require<typename X::iterator>(),
					tc::vailed_expr<ritr>(x.rbegin()),
					tc::vailed_expr<ritr>(x.rend()),
					tc::vailed_expr<critr>(x.crbegin()),
					tc::vailed_expr<critr>(x.crend())
					);
		};


		///<summary>
		///任意のコンテナXに対して、その要素型をデフォルトで挿入可能か
		///</summary>
		template<class X> TC_CONCEPT(DefaultInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<DefaultConstructible>::require<typename X::value_type>()
				);
		};

		///<summary>
		///任意のコンテナXに対して、その要素型のコピー挿入可能か
		///</summary>
		template<class X> TC_CONCEPT(CopyInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<CopyConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///任意のコンテナXに対して、その要素型の右辺値オブジェクトをムーブ挿入可能か
		///</summary>
		template<class X> TC_CONCEPT(MoveInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<MoveConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///任意のコンテナXに対して、要素型のコンストラクタ引数列Argsから直接構築可能か
		///</summary>
		template<class X, class... Args> TC_CONCEPT(EmplaceConstructible, X,Args...)
		{
			template<class X, class... Args>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<Constructible>::require<typename X::value_type, Args...>()
				);
		};

		///<summary>
		///任意のコンテナXに対して、要素型の破棄が可能か
		///</summary>
		template<class X> TC_CONCEPT(Erasable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<Destructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///アロケーターを認識するコンテナか
		///</summary>
		template<class X> TC_CONCEPT(AllocatorAwareContainer, X)
		{
			template<class X,class alloc_t = typename X::allocator_type>
				auto require(X&& x)->decltype(
					tc::extends<CopyAssignable, MoveAssignable, CopyInsertable, MoveInsertable>::require<X>(),
					tc::extends<DefaultConstructible>::require<std::allocator<typename X::value_type>>(),
					tc::extends<Constructible>::require<X, std::allocator<typename X::value_type>>(),
					tc::extends<Constructible>::require<X, std::add_lvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
					tc::extends<Constructible>::require<X, std::add_rvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
					tc::vailed_expr<alloc_t>(x.get_allocator()),
					tc::vailed_expr<void>((x.swap(x), _void))
					);
		};

		///<summary>
		///レンジかどうか
		///</summary>
		template<class Type> TC_CONCEPT(Range, Type)
		{
			template<class Type>
			auto require(Type&& range)->decltype(
				tc::extends<Iterator>::require(std::begin(range)),
				tc::extends<Iterator>::require(std::end(range))
				);
		};

		//************************************************************************************************
		//
		//その他
		//
		//************************************************************************************************
		///<summary>
		///条件式conditionalをみたすか concept_map不可
		///</summary>
		template<bool conditional>
		struct Condition : std::bool_constant<conditional>
		{};


		///<summary>
		///TypeがTemplate型であるか
		///</summary>
		template<template <class...>class Template, class Type>
		struct is_template : std::false_type
		{};
		template<template <class...>class Template, class... Args>
		struct is_template<Template, Template<Args...>> : std::true_type
		{};


	}//namespace concept
}//namespace tc


