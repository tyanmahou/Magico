#pragma once
#include"Magico/concept.hpp"
#include"Magico/concept_any.hpp"
#include"Magico/priority.hpp"

#include<functional>
#include<unordered_map>
//************************************************************************************************
//
//concept example 
//
//************************************************************************************************

namespace mc {
	namespace concept {
		//************************************************************************************************
		//
		//Operator
		//
		//************************************************************************************************



#define mc_HAS_UNARY_OPERATOR(className,symbol)\
template<class Type> MC_CONCEPT(className,Type)\
{\
	template<class Type>\
	auto require(Type&& t)->decltype(symbol t);\
}

		///<summary>
		/// operator ! をもつか
		///</summary>
		mc_HAS_UNARY_OPERATOR(HasLogicalNot, !);


		///<summary>
		/// 単項operator + をもつか
		///</summary>
		mc_HAS_UNARY_OPERATOR(HasUnaryPlus, +);

		///<summary>
		/// 単項operator - をもつか
		///</summary>
		mc_HAS_UNARY_OPERATOR(HasNegate, -);

		///<summary>
		/// 前置インクリメント可能か
		///</summary>
		mc_HAS_UNARY_OPERATOR(HasPreIncrement, ++);

		///<summary>
		/// 前置デクリメント可能か
		///</summary>
		mc_HAS_UNARY_OPERATOR(HasPreDecrement, --);

		///<summary>
		/// 後置インクリメント可能か
		///</summary>
		template<class Type>
		MC_CONCEPT(HasPostIncrement, Type)
		{
			template<class Type>
			auto require(Type&& t)->decltype(t++);
		};

		///<summary>
		/// 後置デクリメント可能か
		///</summary>
		template<class Type>
		MC_CONCEPT(HasPostDecrement, Type)
		{
			template<class Type>
			auto require(Type&& t)->decltype(t--);
		};
		///<summary>
		/// 単項operator ~ をもつか
		///</summary>
		mc_HAS_UNARY_OPERATOR(HasComplement, ~);

		///<summary>
		/// アドレス取得可能か
		///</summary>
		mc_HAS_UNARY_OPERATOR(HasAddressOf, &);

		///<summary>
		/// 関接演算可能か
		///</summary>
		mc_HAS_UNARY_OPERATOR(HasIndirect, *);


#undef mc_HAS_UNARY_OPERATOR

#define mc_HAS_BINARY_OPERATOR(className,symbol)\
template<class Left,class Right=Left> \
MC_CONCEPT(className,Left,Right)\
{\
	template<class Left,class Right>\
	auto require(Left&& l,Right&& r)->decltype(l symbol r);\
}

		///<summary>
		/// 代入演算可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasAssign, =);

		///<summary>
		/// 加算可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasPlus, +);

		///<summary>
		/// 減算可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasMinus, -);

		///<summary>
		/// 乗算可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasMultiply, *);

		///<summary>
		/// 除算可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasDivide, / );

		///<summary>
		/// 剰余可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasModulus, %);

		///<summary>
		/// 加算代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasPlusAssign, +=);

		///<summary>
		/// 減算代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasMinusAssign, -=);

		///<summary>
		/// 乗算代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasMultiplyAssign, *=);

		///<summary>
		/// 除算代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasDivideAssign, /=);

		///<summary>
		/// 剰余代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasModulusAssign, %=);

		///<summary>
		/// 左シフト可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasLeftShift, << );

		///<summary>
		/// 右シフト可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasRightShift, >> );

		///<summary>
		/// 左シフト代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasLeftShiftAssign, <<=);

		///<summary>
		/// 右シフト可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasRightShiftAssign, >>=);

		///<summary>
		/// ビットごとのAND演算可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasBitAnd, &);

		///<summary>
		/// ビットごとのOR演算可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasBitOr, | );

		///<summary>
		/// ビットごとの排他的OR演算可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasBitXor, ^);

		///<summary>
		/// ビットごとのAND演算代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasBitAndAssign, &=);

		///<summary>
		/// ビットごとのOR演算代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasBitOrAssign, |=);

		///<summary>
		/// ビットごとの排他的OR演算代入可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasBitXorAssign, ^=);

		///<summary>
		/// 論理積可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasLogicalAnd, &&);

		///<summary>
		/// 論理和可能か
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasLogicalOr, || );

		///<summary>
		/// 小なり比較演算 をもつか
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasLess, < );

		///<summary>
		/// 小なり=比較演算 をもつか
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasLessEqual, <= );

		///<summary>
		/// 大なり比較演算をもつか
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasGreater, > );

		///<summary>
		/// 大なり=比較演算をもつか
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasGreaterEqual, >= );

		///<summary>
		/// operator == をもつか
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasEqualTo, == );

		///<summary>
		/// operator != をもつか
		///</summary>
		mc_HAS_BINARY_OPERATOR(HasNotEqualTo, != );

		///<summary>
		/// operator , をもつか
		///</summary>
		template<class Left, class Right = Left>
		MC_CONCEPT(HasComma, Left, Right)
		{
			template<class Left, class Right>
			auto require(Left&& l, Right&& r)->decltype(l, r);
		};

		///<summary>
		/// operator [] をもつか
		///</summary>
		template<class Type, class Key = std::size_t>
		MC_CONCEPT(HasSubscript, Type, Key)
		{
			template<class Type, class Key>
			auto require(Type t, Key key)->decltype(t[key]);
		};

#undef mc_HAS_BINARY_OPERATOR



		//************************************************************************************************
		//
		//Basic
		//
		//************************************************************************************************

		///<summary>
		/// operator =(copy) をもつか
		///</summary>
		template< class To, class From = To> MC_CONCEPT(CopyAssignable, To, From)
		{
			template<class To, class From>
			auto require(To& t, const From& f)->decltype(
				mc::extends<std::is_assignable>::require(t, f)
				);
		};

		///<summary>
		/// operator =(move) をもつか
		///</summary>
		template< class To, class From = To> MC_CONCEPT(MoveAssignable, To, From)
		{
			template<class To, class From>
			auto require(To t, From f)->decltype(
				mc::extends<std::is_assignable>::require<
				std::add_lvalue_reference_t<To>,
				std::add_rvalue_reference_t<From>
				>()
				);
		};

		///<summary>
		/// Type( Args... ) の形式のコンストラクタ呼び出しが可能か
		///</summary>
		template< class Type, class... Args> MC_CONCEPT(Constructible, Type, Args...)
		{
			template<class Type, class... Args>
			auto require()->decltype(
				mc::extends<std::is_constructible>::require<Type, Args...>()
				);
		};

		///<summary>
		/// デフォルトコンストラクタ をもつか
		///</summary>
		template< class Type > MC_CONCEPT(Defaulmconstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_default_constructible>::require<Type>()
				);
		};
		///<summary>
		/// コピーコンストラクタ をもつか
		///</summary>
		template< class Type > MC_CONCEPT(CopyConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_copy_constructible>::require<Type>()
				);
		};
		///<summary>
		/// ムーブコンストラクタ をもつか
		///</summary>
		template< class Type > MC_CONCEPT(MoveConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_move_constructible>::require<Type>()
				);
		};
		///<summary>
		/// デストラクタ をもつか
		///</summary>
		template< class Type > MC_CONCEPT(Destructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_destructible>::require<Type>()
				);
		};
		///<summary>
		/// 仮想デストラクタ をもつか
		///</summary>
		template< class Type > MC_CONCEPT(HasVirtualDestructor, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::has_virtual_destructor>::require<Type>()
				);
		};

		///<summary>
		///TとUが同じか
		///</summary>
		template<class T, class U> MC_CONCEPT(IsSame, T, U)
		{
			template<class T, class U>
			auto require()->decltype(
				mc::extends<std::is_same>::require<T, U>()
				);
		};

		///<summary>
		///TypeがBase,もしくはBaseを継承しているか
		///</summary>
		template<class Type, class Base> MC_CONCEPT(Extended, Type, Base)
		{
			template<class Type, class Base>
			auto require()->decltype(
				mc::extends<std::is_base_of>::require<Base, Type>()
				);
		};

		///<summary>
		///スカラーかどうか
		///</summary>
		template< class Type > MC_CONCEPT(Scalar, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_scalar>::require<Type>()
				);
		};

		///<summary>
		///仮想クラスかどうか
		///</summary>
		template< class Type > MC_CONCEPT(Abstract, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_abstract>::require<Type>()
				);
		};

		///<summary>
		///enum型か
		///</summary>
		template< class Type > MC_CONCEPT(Enum, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_enum>::require<Type>()
				);
		};

		///<summary>
		///class(struct)型か
		///</summary>
		template< class Type > MC_CONCEPT(Class, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_class>::require<Type>()
				);
		};

		///<summary>
		///union型か
		///</summary>
		template< class Type > MC_CONCEPT(Union, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_union>::require<Type>()
				);
		};

		///<summary>
		///関数型t(...)か
		///</summary>
		template< class Type > MC_CONCEPT(Function, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_function>::require<Type>()
				);
		};

		//************************************************************************************************
		//
		//Library-wide
		//
		//************************************************************************************************

		///<summary>
		///　<　演算子で大小関係にあるか
		///</summary>
		template<class Type> MC_CONCEPT(LessThanComparable, Type)
		{
			template<class Type>
			auto require(Type a, Type b)->decltype(
				mc::valid_expr<bool>(a < b)
				);
		};

		///<summary>
		///　==演算子で等価関係にあるか
		///</summary>
		template<class Type> MC_CONCEPT(EqualityComparable, Type)
		{
			template<class Type>
			auto require(Type a, Type b)->decltype(
				mc::valid_expr<bool>(a == b)
				);
		};

		///<summary>
		///アロケーターか
		///</summary>
		template<class Type> MC_CONCEPT(Allocator, Type)
		{
		private:
			size_t n;
			using ex = mc::extends<EqualityComparable, CopyConstructible, MoveConstructible>;
		public:
			template<class Type>
			auto require(Type& alloc, typename Type::value_type* ptr, const typename Type::value_type* cptr)->decltype(
				ex::require<Type>(),
				valid_expr< typename Type::value_type& >(*ptr),
				valid_expr< const typename Type::value_type& >(*cptr),
				valid_expr< typename Type::value_type* >(alloc.allocate(n)),
				valid_expr< bool >(alloc != alloc),
				alloc.deallocate(ptr, n)
				);
		};

		///<summary>
		///TのオブジェクトとUのオブジェクトが入れ替え可能か
		///</summary>
		template<class T, class U = T> MC_CONCEPT(Swappable, T, U)
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
		template< class From, class To > MC_CONCEPT(Convertible, From, To)
		{
			template< class From, class To >
			auto require()->decltype(
				mc::extends<std::is_convertible>::require<From, To>()
				);
		};

		///<summary>
		/// null許容か
		///</summary>
		template<class Type> MC_CONCEPT(NullablePointer, Type)
		{
		private:
			using ex = mc::extends<
				EqualityComparable,
				Defaulmconstructible,
				CopyConstructible,
				CopyAssignable,
				Destructible
			>;
		public:
			template<class Type>
			auto require(Type&& null)->decltype(
				ex::require<Type>(),
				mc::extends<Constructible>::require<Type, std::nullptr_t>(),
				mc::valid_expr< bool >(null != null),
				mc::valid_expr< bool >(null = nullptr),
				mc::valid_expr< bool >(null == nullptr),
				mc::valid_expr< bool >(nullptr == null),
				mc::valid_expr< bool >(null != nullptr),
				mc::valid_expr< bool >(nullptr != null)
				);

		};

		///<summary>
		///関数呼び出し可能な型か
		///</summary>
		template <class F, class... Args>MC_CONCEPT(Invocable, F, Args...)
		{
			template <class F, class... Args>
			auto require(F&& func, Args&&... args)->decltype(
				std::invoke(func, args...)
				);

		};

		///<summary>
		///関数オブジェクトか
		///</summary>
		template <class Type, class... Args> MC_CONCEPT(FunctionObject, Type, Args...)
		{
			template <class Type, class... Args>
			auto require()->decltype(
				mc::extends<Invocable>::require<Type, Args...>(),
				mc::extends<std::is_object>::require<Type>()
				);
		};
		///<summary>
		///bool型になる関数オブジェクトか
		///</summary>
		template <class Type, class... Args> MC_CONCEPT(Predicate, Type, Args...)
		{
			template <class Type, class... Args>
			auto require(Type t, Args&&... args)->decltype(
				mc::extends<FunctionObject>::require<Type, Args...>(),
				mc::valid_expr<bool>(t(args...))
				);
		};

		///<summary>
		///bool型になる関数オブジェクトか
		///</summary>
		template <class Type, class T, class U = T> MC_CONCEPT(BinaryPredicate, Type, T, U)
		{
			template <class Type, class T, class U>
			auto require(Type t, T a, U b)->decltype(
				mc::extends<CopyConstructible>::require<Type>(),
				mc::extends<Predicate>::require<Type, T, U>()
				);
		};

		///<summary>
		///比較関数オブジェクトか
		///</summary>
		template <class Type, class T, class U = T> MC_CONCEPT(Compare, Type, T, U)
		{
			template <class Type, class T, class U>
			auto require(Type t, T a, U b)->decltype(
				mc::extends<BinaryPredicate>::require<Type, T, U>(),
				mc::valid_expr<bool>(!t(a, b) && !t(b, a))
				);
		};
		///<summary>
		///メタ関数か
		///</summary>
		template <class Type> MC_CONCEPT(MetaFunc, Type)
		{
			template <class Type>
			auto require()->decltype(
				valid_expr<const bool&>(Type::value)
				);
		};

		///<summary>
		///ハッシュ関数オブジェクトか
		///</summary>
		template <class Type, class Key> MC_CONCEPT(Hash, Type, Key)
		{
		private:
			using ex = mc::extends<CopyConstructible, Destructible>;
		public:
			template <class Type, class Key>
			auto require(Type&& t, Key&& key)->decltype(
				ex::require<Type>(),
				valid_expr<size_t>(t(key))
				);

		};

		///<summary>
		///期間、時刻、現在の時刻を取得可能か
		///</summary>
		template <class Type> MC_CONCEPT(Clock, Type)
		{
			template <class Type>
			auto require(Type&& t)->decltype(
				mc::associated_type<typename Type::rep>(),
				mc::associated_type<typename Type::period>(),
				mc::associated_type<typename Type::duration>(),
				mc::associated_type<typename Type::time_point>(),
				valid_expr<const bool&>(t.is_steady),
				valid_expr<typename Type::time_point>(t.now())
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
		template< class Type > MC_CONCEPT(TriviallyCopyable, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_trivially_copyable>::require<Type>()
				);
		};

		///<summary>
		///トリビアル型か
		///</summary>
		template< class Type > MC_CONCEPT(TrivialType, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_trivial>::require<Type>()
				);
		};
		///<summary>
		///標準レイアウト型か
		///</summary>
		template<class Type>MC_CONCEPT(StandardLayoutType, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_standard_layout>::require<Type>()
				);
		};

		///<summary>
		///POD型か
		///</summary>
		template<class Type>MC_CONCEPT(PODType, Type)
		{
			template<class Type>
			auto require()->decltype(
				mc::extends<std::is_pod>::require<Type>()
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
		template<class It> MC_CONCEPT(Iterator, It)
		{
		private:
			using ex = mc::extends<
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
		template<class It> MC_CONCEPT(InputIterator, It)
		{
		private:
			using ex = mc::extends<
				Iterator,
				EqualityComparable
			>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				mc::valid_expr<bool>(it != it),
				mc::valid_expr<typename std::iterator_traits<It>::value_type>(*it),
				mc::valid_expr<typename std::iterator_traits<It>::value_type>(*it++),
				mc::valid_expr<typename std::iterator_traits<It>::reference>(*it)
				);
		};


		///<summary>
		///出力イテレーターかどうか
		///</summary>
		template<class It> MC_CONCEPT(OutputIterator, It)
		{
		private:
			using ex = mc::extends<Iterator>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				mc::valid_expr<const It&>(it++),
				mc::valid_expr<It&>(++it),
				*it = *it,
				*it++ = *it
				);
		};


		///<summary>
		///前方イテレーターかどうか
		///</summary>
		template<class It> MC_CONCEPT(ForwardIterator, It)
		{
		private:
			using ex = mc::extends<InputIterator, Defaulmconstructible>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				mc::valid_expr<It>(it++),
				mc::valid_expr<typename std::iterator_traits<It>::reference>(*it++)
				);
		};


		///<summary>
		///双方向イテレーターかどうか
		///</summary>
		template<class It> MC_CONCEPT(BidirectionalIterator, It)
		{
		private:
			using ex = mc::extends<ForwardIterator>;
		public:
			template<class It>
			auto require(It&& it)->decltype(
				ex::require<It>(),
				mc::valid_expr<const It&>(it--),
				mc::valid_expr<It&>(--it),
				mc::valid_expr<typename std::iterator_traits<It>::reference>(*it--)
				);
		};

		///<summary>
		///ランダムアクセスイテレーターかどうか
		///</summary>
		template<class It> MC_CONCEPT(RandomAccessIterator, It)
		{
		private:
			using ex = mc::extends<
				BidirectionalIterator,
				LessThanComparable
			>;
		public:
			template<class It>
			auto require(It&& it, typename std::iterator_traits<It>::difference_type n)->decltype(
				ex::require<It>(),
				mc::valid_expr<bool>(it <= it),
				mc::valid_expr<bool>(it > it),
				mc::valid_expr<bool>(it >= it),
				mc::valid_expr<It&>(it += n),
				mc::valid_expr<It>(it + n), mc::valid_expr<It>(n + it),
				mc::valid_expr<It&>(it -= n),
				mc::valid_expr<It>(it - n),
				mc::valid_expr<typename std::iterator_traits<It>::reference>(it[n])
				);

		};



		///<summary>
		///イテレーターの値型がスワップ可能か
		///</summary>
		template<class It> MC_CONCEPT(ValueSwappable, It)
		{
		private:
			using ex = mc::extends<Iterator>;
		public:
			template<class It>
			auto require(It it)->decltype(
				ex::require<It>(),
				mc::extends<Swappable>::require<typename std::iterator_traits<It>::value_type>()
				);
		};

		///<summary>
		///イテレーターをもつか
		///</summary>
		template<class Type> MC_CONCEPT(HasIterator, Type)
		{

			template<class Type>
			auto require()->decltype(
				mc::extends<Iterator>::require<typename Type::iterator>()
				);
		};

		//************************************************************************************************
		//
		//Container
		//
		//************************************************************************************************


		///<summary>
		///コンテナかどうか
		///</summary>
		template<class X> MC_CONCEPT(Container, X)
		{
			template<class X,
				class value_t = typename X::value_type,
				class ref = typename X::reference,
				class cref = typename X::const_reference,
				class itr = typename X::iterator,
				class citr = typename X::const_iterator,
				class dif = typename X::difference_type,
				class size = typename X::size_type,
				class extends = mc::require<
				Defaulmconstructible<X>,
				CopyConstructible<X>,
				Destructible<X>,
				EqualityComparable<X>,
				CopyAssignable<X>
				>
			>
				auto require(X&& x)->decltype(
					mc::valid_expr<bool>(x != x),
					mc::valid_expr<itr>(x.begin()),
					mc::valid_expr<itr>(x.end()),
					mc::valid_expr<citr>(x.cbegin()),
					mc::valid_expr<citr>(x.cend()),
					mc::valid_expr<void>((x.swap(x), _void)),
					mc::valid_expr<void>((std::swap(x, x), _void)),
					mc::valid_expr<size>(x.size()),
					mc::valid_expr<size>(x.max_size()),
					mc::valid_expr<bool>(x.empty())
					);
		};

		///<summary>
		///前方イテレーターをもつコンテナかどうか
		///</summary>
		template<class X> MC_CONCEPT(ForwardContainer, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				mc::extends<Container>::require<X>(),
				mc::extends<ForwardIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///ランダムアクセスイテレーターをもつコンテナかどうか
		///</summary>
		template<class X> MC_CONCEPT(RandomAccessContainer, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				mc::extends<Container>::require<X>(),
				mc::extends<RandomAccessIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///リバースイテレーターをもつコンテナかどうか
		///</summary>
		template<class X> MC_CONCEPT(ReversibleContainer, X)
		{
			template<class X,
				class ritr = typename X::reverse_iterator,
				class critr = typename X::const_reverse_iterator
			>
				auto require(X&& x)->decltype(
					mc::extends<Container>::require<X>(),
					mc::extends<BidirectionalIterator>::require<typename X::iterator>(),
					mc::valid_expr<ritr>(x.rbegin()),
					mc::valid_expr<ritr>(x.rend()),
					mc::valid_expr<critr>(x.crbegin()),
					mc::valid_expr<critr>(x.crend())
					);
		};


		///<summary>
		///任意のコンテナXに対して、その要素型をデフォルトで挿入可能か
		///</summary>
		template<class X> MC_CONCEPT(DefaultInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				mc::extends<Container>::require<X>(),
				mc::extends<Defaulmconstructible>::require<typename X::value_type>()
				);
		};

		///<summary>
		///任意のコンテナXに対して、その要素型のコピー挿入可能か
		///</summary>
		template<class X> MC_CONCEPT(CopyInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				mc::extends<Container>::require<X>(),
				mc::extends<CopyConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///任意のコンテナXに対して、その要素型の右辺値オブジェクトをムーブ挿入可能か
		///</summary>
		template<class X> MC_CONCEPT(MoveInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				mc::extends<Container>::require<X>(),
				mc::extends<MoveConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///任意のコンテナXに対して、要素型のコンストラクタ引数列Argsから直接構築可能か
		///</summary>
		template<class X, class... Args> MC_CONCEPT(EmplaceConstructible, X, Args...)
		{
			template<class X, class... Args>
			auto require(X&& x)->decltype(
				mc::extends<Container>::require<X>(),
				mc::extends<Constructible>::require<typename X::value_type, Args...>()
				);
		};

		///<summary>
		///任意のコンテナXに対して、要素型の破棄が可能か
		///</summary>
		template<class X> MC_CONCEPT(Erasable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				mc::extends<Container>::require<X>(),
				mc::extends<Destructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///アロケーターを認識するコンテナか
		///</summary>
		template<class X> MC_CONCEPT(AllocatorAwareContainer, X)
		{
			template<class X, class alloc_t = typename X::allocator_type>
			auto require(X&& x)->decltype(
				mc::extends<Container>::require<X>(),
				mc::extends<CopyAssignable, MoveAssignable, CopyInsertable, MoveInsertable>::require<X>(),
				mc::extends<Defaulmconstructible>::require<std::allocator<typename X::value_type>>(),
				mc::extends<Constructible>::require<X, std::allocator<typename X::value_type>>(),
				mc::extends<Constructible>::require<X, std::add_lvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
				mc::extends<Constructible>::require<X, std::add_rvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
				mc::valid_expr<alloc_t>(x.get_allocator()),
				mc::valid_expr<void>((x.swap(x), _void))
				);
		};
		///<summary>
		///線形保管するコンテナか
		///</summary>
		template<class X> MC_CONCEPT(SequenceContainer, X)
		{
			//アロケーターの型
			template<class X, class = void>
			struct GetAllocator
			{
				using type = std::allocator<typename X::value_type>;
			};
			template<class X>
			struct GetAllocator<X, mc::void_t<typename X::allocator_type>>
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
					mc::extends<Container>::require<X>(),
					X(n, t), X(it, it), X(il),
					mc::valid_expr<X&>(a = il),
					mc::valid_expr<It>(a.insert(cit, t)), mc::valid_expr<It>(a.insert(cit, n, t)),
					mc::valid_expr<It>(a.insert(cit, it, it)), mc::valid_expr<It>(a.insert(cit, il)),
					mc::valid_expr<It>(a.erase(cit)), mc::valid_expr<It>(a.erase(cit, cit)),
					mc::valid_expr<void>((a.clear(), _void)),
					a.assign(it, it), a.assign(il), a.assign(n, t)
					);
		};
		///<summary>
		///キーに基づいて順序付けられたデータを検索するコンテナか
		///</summary>
		template<class X> MC_CONCEPT(AssociativeContainer, X)
		{
			template<class X,
				class Key = typename X::key_type,
				class Compare = typename X::key_compare,
				class Value = typename X::value_type,
				class ValueCompare = typename X::value_compare,
				class It = typename X::iterator
			>
				auto require(X a, Compare c, Value t, It it, std::initializer_list<Value> il)->decltype(
					mc::extends<BinaryPredicate>::require<ValueCompare, Value>(),
					X(), X(c), X(it, it, c), X(it, it), X(il),
					mc::valid_expr<X&>(a = il),
					mc::valid_expr<Compare>(a.key_comp()),
					mc::valid_expr<ValueCompare>(a.value_comp())
					);
		};
		///<summary>
		///キーに基づいて順序付けられていないデータを検索するコンテナか
		///</summary>
		template<class X> MC_CONCEPT(UnorderedAssociativeContainer, X)
		{
			template<class X>
			struct GetValueType
			{
				using type = typename X::key_type;
			};
			template<class Key,class T, class Hash, class KeyEqual, class Allocator>
			struct GetValueType <std::unordered_map<Key,T,Hash,KeyEqual,Allocator> >
			{
				using type = std::pair<const Key,T>;
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
				auto require(X a, const X& b,const Hash& hf, const Pred& eq,
					Value t, It it, std::initializer_list<Value> il, Size n)->decltype(
						mc::extends<IsSame>::require<Value,typename GetValueType<X>::type>(),
						X(), X(n), X(n, hf, eq), X(n, hf), X(it, it, n, hf, eq), X(it, it, n, hf),
						X(it, it, n), X(it, it), X(il), X(il,n), X(il, n,hf), X(il, n, hf, eq),X(b),
						mc::valid_expr<X&>(a = il),
						mc::valid_expr<X&>(a = b)
						);
		};

		///<summary>
		///レンジかどうか
		///</summary>
		template<class Type> MC_CONCEPT(Range, Type)
		{
			template<class Type>
			auto require(Type&& range)->decltype(
				mc::extends<Iterator>::require(std::begin(range)),
				mc::extends<Iterator>::require(std::end(range))
				);
		};

		//************************************************************************************************
		//
		//Other
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
}//namespace mc


