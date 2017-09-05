#pragma once
#include<type_traits>
#include<tuple>

//----------------------------------------------------------------------------------
//memNameのメンバをもつかどうか
//----------------------------------------------------------------------------------

#define TC_HAS_MEMBER(className,memName)\
namespace detail\
{\
template<class Type>\
using className##_c = tc::constraint<decltype(&Type::memName)>;\
}\
template<class Type>\
struct className : tc::to_concept<detail::className##_c>::type<Type>\
{}

//----------------------------------------------------------------------------------
//typeNameのメンバ型名をもつかどうか
//----------------------------------------------------------------------------------

#define TC_HAS_MEMBER_TYPE(className ,typeName)\
namespace detail\
{\
template<class Type>\
using className##_c = tc::constraint<typename Type::typeName>;\
}\
template<class Type>\
struct className : tc::to_concept<detail::className##_c>::type<Type>\
{}

//----------------------------------------------------------------------------------
//symbol単項演算が可能か
//----------------------------------------------------------------------------------

#define TC_HAS_UNARY_OPERATOR(className,symbol)\
namespace detail\
{\
	template<class Type>\
	using className##_c = tc::constraint<decltype(symbol std::declval<Type&>())>;\
}\
template<class Type>\
struct className: tc::to_concept<detail::className##_c>::type<Type>\
{}

//----------------------------------------------------------------------------------
//symbol2項演算が可能か
//----------------------------------------------------------------------------------

#define TC_HAS_BINARY_OPERATOR(className,symbol)\
namespace detail\
{\
template<class Left,class Right=Left>\
using className##_c = tc::constraint<decltype(std::declval<Left&>() symbol std::declval<Right&>())>; \
}\
template<class Left,class Right=Left>\
struct className: tc::to_concept<detail::className##_c>::type<Left,Right>\
{}

//----------------------------------------------------------------------------------
namespace tc
{
	namespace detail
	{
		template<class...>
		struct void_t_impl
		{
			typedef void type;
		};
		template<class... Test>
		using void_t = typename void_t_impl<Test...>::type;

		template<class = void, template<class...>class Constraint, class ...Args>
		struct is_detected_impl :std::false_type
		{};

		template<template<class...>class Constraint, class ...Args>
		struct is_detected_impl<void_t<Constraint<Args...>>, Constraint, Args...> :std::true_type
		{};

		//-- TODO std::experimental::is_detected
		template<template<class...>class Constraint, class ...Args>
		using is_detected = is_detected_impl <void, Constraint, Args...>;


		template<bool Test, class = void, template<class...>class Constraint, class...Args>
		struct constraint_if_impl
		{};
		template<template<class...>class Constraint, class...Args>
		struct constraint_if_impl<true, void_t<Constraint<Args...>>, Constraint, Args...>
		{
			using type = void_t<Constraint<Args...>>;
		};
		template<template<class...>class Constraint, class...Args>
		struct constraint_if_impl<false, void, Constraint, Args...>
		{
			using type = void;
		};
	}//namespace detail

	///<summary>
	///仮のインスタンス作成(declvalのヘルパ)
	///</summary>
	template<class Type>
	Type&& val = std::declval<Type>();

	///<summary>
	///制約
	///</summary>
	template<class ...Args>
	using constraint = detail::void_t<Args...>;

	///<summary>
	///Testが成功した場合 Constraintを制約とみなす
	///</summary>
	template<bool Test, template<class...>class Constraint, class...Args>
	using constraint_if = typename detail::constraint_if_impl<Test, void, Constraint, Args...>::type;

	///<summary>
	///Conceptが条件を満たすか
	///</summary>
	template < class WhereConcept >
	using requires = std::enable_if_t<WhereConcept::value, std::nullptr_t >;

	///<summary>
	///Constraintを満たすか
	///</summary>
	template <template<class...>class Constraint, class ...Args>
	using requires_c = requires<detail::is_detected<Constraint, Args...>>;

	///<summary>
	///ConstraintをConcept形式に変換
	///</summary>
	template<template<class...>class Constraint>
	struct to_concept
	{
		template<class... Args>
		using type = detail::is_detected<Constraint, Args...>;
	};

	template<template<class...>class Constraint, class... Args>
	using to_concept_t = typename to_concept<Constraint>::template type<Args...>;

	///<summary>
	///ConceptをConstraint形式に変換
	///</summary>
	template<template<class...>class Concept>
	struct to_constraint
	{
		template<class... Args>
		using type = constraint<requires<Concept<Args...>>>;
	};
	template<template<class...>class Concept, class... Args>
	using to_constraint_t = typename to_constraint<Concept>::template type<Args...>;

}//namespace tc

 //************************************************************************************************
 //
 //concept example 
 //
 //************************************************************************************************
namespace tc
{
	namespace Concept
	{
		//************************************************************************************************
		//
		//演算子
		//
		//************************************************************************************************
		namespace detail
		{

			template<class Type>
			using PostIncrementable_c = constraint<decltype(std::declval<Type&>()++)>;

			template<class Type>
			using PostDecrementable_c = constraint<decltype(std::declval<Type&>()--)>;
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
		struct PostIncrementable :to_concept<detail::PostIncrementable_c>::type< Type>
		{};
		///<summary>
		/// 後置デクリメント可能か
		///</summary>		
		template<class Type>
		struct PostDecrementable :to_concept<detail::PostDecrementable_c>::type< Type>
		{};
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
		/// operator < をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessThanComparable, <);

		///<summary>
		/// operator <= をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessEqualComparable, <= );

		///<summary>
		/// operator > をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(GreaterThanComparable, >);

		///<summary>
		/// operator >= をもつか
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

		//************************************************************************************************
		//
		//基本的なコンセプト
		//
		//************************************************************************************************

		///<summary>
		/// operator =(copy) をもつか
		///</summary>
		template< class To, class From = To>
		struct CopyAssignable : std::is_assignable<
			typename std::add_lvalue_reference<To>::type,
			typename std::add_lvalue_reference<
			typename std::add_const<From>::type
			>::type
		>::type
		{};

		///<summary>
		/// operator =(move) をもつか
		///</summary>
		template< class To, class From = To>
		struct MoveAssignable :std::is_assignable<
			typename std::add_lvalue_reference<To>::type,
			typename std::add_rvalue_reference<From>::type
		>::type
		{};

		///<summary>
		/// Type( Args... ) の形式のコンストラクタ呼び出しが可能か
		///</summary>
		template< class Type, class... Args>
		struct Constructible : std::is_constructible<Type, Args...>
		{};

		///<summary>
		/// デフォルトコンストラクタ をもつか
		///</summary>
		template< class Type >
		struct DefaultConstructible : std::is_default_constructible<Type>
		{};

		///<summary>
		/// コピーコンストラクタ をもつか
		///</summary>
		template< class Type>
		struct CopyConstructible : std::is_copy_constructible<Type>
		{};

		///<summary>
		/// ムーブコンストラクタ をもつか
		///</summary>
		template< class Type>
		struct MoveConstructible : std::is_move_constructible<Type>
		{};

		///<summary>
		/// デストラクタ をもつか
		///</summary>
		template< class Type>
		struct Destructible : std::is_destructible<Type>
		{};

		///<summary>
		/// 仮想デストラクタ をもつか
		///</summary>
		template< class Type>
		struct HasVirtualDestructor : std::has_virtual_destructor<Type>
		{};

		///<summary>
		///TとUが同じか
		///</summary>
		template<class T, class U>
		struct IsSame : std::is_same<T, U>
		{};

		///<summary>
		///TypeがBase,もしくはBaseを継承しているか
		///</summary>
		template<class Type, class Base>
		struct Extended : std::is_base_of<Base, Type>
		{};

		///<summary>
		///スカラーかどうか
		///</summary>
		template<class Type>
		struct Scalar :std::is_scalar<Type>
		{};

		///<summary>
		///仮想クラスかどうか
		///</summary>
		template<class Type>
		struct Abstract :std::is_abstract<Type>
		{};
		///<summary>
		///enum型か
		///</summary>
		template<class Type>
		struct Enum :std::is_enum<Type>
		{};

		///<summary>
		///class(struct)型か
		///</summary>
		template<class Type>
		struct Class :std::is_class<Type>
		{};

		///<summary>
		///union型か
		///</summary>
		template<class Type>
		struct Union :std::is_union<Type>
		{};

		///<summary>
		///関数型t(...)か
		///</summary>
		template<class Type>
		struct Function :std::is_function<Type>
		{};

		//////************************************************************************************************
		//////
		//////型の特性
		//////
		//////************************************************************************************************

		namespace detail
		{
			template<class Type>
			using Allocator_c = constraint<
				requires<EqualityComparable<Type>>,
				requires<NotEqualityComparable<Type>>,
				requires<CopyConstructible<Type>>,
				requires<MoveConstructible<Type>>,
				typename Type::value_type,
				decltype(
					*std::declval<typename Type::value_type*&>(),
					*std::declval<const typename Type::value_type*&>(),
					std::declval<Type&>().allocate(std::declval<size_t&>()),
					std::declval<Type&>().deallocate(std::declval<typename Type::value_type*&>(), std::declval<size_t&>())
					)
			>;

			template<class T, class U>
			using Swappable_c = constraint<
				decltype(
					std::declval<T&>() = std::declval<U&>(),
					std::declval<U&>() = std::declval<T&>()
					)
			>;

			template<class Type>
			using NullablePointer_c = constraint<
				requires<EqualityComparable<Type>>,
				requires<NotEqualityComparable<Type>>,
				requires<DefaultConstructible<Type>>,
				requires<CopyConstructible<Type>>,
				requires<Constructible<Type, std::nullptr_t>>,
				requires<CopyAssignable<Type>>,
				requires<Destructible<Type>>,
				decltype(
					std::declval<Type&>() = nullptr,
					std::declval<Type&>() == nullptr,
					nullptr == std::declval<Type&>(),
					std::declval<Type& >() != nullptr,
					nullptr != std::declval<Type&>()
					)
			>;

			template <class F, class... Args>
			using Invocable_c = constraint<
				decltype(std::invoke(std::declval<F&>(), std::declval<Args&>()...))
			>;

			template <class Type>
			using FunctionObject_c = constraint<
				decltype(&Type::operator())
			>;

			template<class Type>
			using Concept_c = constraint<
				requires<IsSame<const bool*, decltype(&Type::value)>>
			>;

			template<class Type, class Key>
			using Hash_c = constraint<
				FunctionObject_c<Type>,
				requires<CopyConstructible<Type>>,
				requires<Destructible<Type>>,
				requires<IsSame<std::size_t, decltype(std::declval<Type&>()(std::declval<Key&>()))>>
			>;

			template<class Type>
			using Clock_c = constraint<
				typename Type::rep,
				typename Type::period,
				typename Type::duration,
				typename Type::time_point,
				decltype(
					std::declval<Type>().is_steady,
					std::declval<Type>().now()
					)
			>;
		}//namespace detail

		///<summary>
		///アロケーターか
		///</summary>
		template<class Type>
		struct Allocator :to_concept<detail::Allocator_c>::type<Type>
		{};

		///<summary>
		///TのオブジェクトとUのオブジェクトが入れ替え可能か
		///</summary>
		template<class T, class U = T>
		struct Swappable :to_concept<detail::Swappable_c>::type< std::add_lvalue_reference_t<T>, std::add_lvalue_reference_t<U>>
		{};

		///<summary>
		/// FromがToにキャストできるか
		///</summary>
		template< class From, class To >
		struct Convertible :std::is_convertible<From, To>
		{};

		///<summary>
		/// null許容か
		///</summary>
		template<class Type>
		struct NullablePointer : to_concept<detail::NullablePointer_c>::type<Type>
		{};

		//--TODO is_invocableに変更したい--

		///<summary>
		///関数呼び出し可能な型か
		///</summary>
		template <class Func, class... Args>
		struct Invocable : to_concept<detail::Invocable_c>::type< Func, Args...>
		{};

		///<summary>
		///関数オブジェクトか
		///</summary>
		template <class Type>
		struct FunctionObject :to_concept<detail::FunctionObject_c>::type< Type>
		{};

		///<summary>
		///コンセプトか
		///</summary>
		template <class Type>
		struct Concept : to_concept<detail::Concept_c>::type< Type>
		{};

		///<summary>
		///ハッシュ関数オブジェクトか
		///</summary>
		template <class Type, class Key>
		struct Hash : to_concept<detail::Hash_c>::type< Type, Key>
		{};

		///<summary>
		///期間、時刻、現在の時刻を取得可能か
		///</summary>
		template<class Type>
		struct Clock :to_concept<detail::Clock_c>::type< Type>
		{};


		//************************************************************************************************
		//
		//レイアウト
		//
		//************************************************************************************************


		///<summary>
		///トリビアルコピー可能か
		///</summary>
		template<class Type>
		struct TriviallyCopyable : std::is_trivially_copyable<Type>
		{};

		///<summary>
		///トリビアル型か
		///</summary>
		template<class Type>
		struct TrivialType : std::is_trivial<Type>
		{};

		///<summary>
		///標準レイアウト型か
		///</summary>
		template<class Type>
		struct StandardLayoutType : std::is_standard_layout<Type>
		{};

		///<summary>
		///POD型か
		///</summary>
		template<class Type>
		struct PODType : std::is_pod<Type>
		{};


		//************************************************************************************************
		//
		//イテレーター
		//
		//************************************************************************************************

		namespace detail
		{

			template<class It>
			using Iterator_c = constraint<
				requires<CopyConstructible<It>>,
				requires<CopyAssignable<It>>,
				requires<Destructible<It>>,
				requires<Swappable<It>>,
				requires<Indirectable<It>>,
				requires<IsSame<It&, decltype(++std::declval<It&>())>>,
				typename std::iterator_traits<It>::value_type
			>;
			template<class It>
			using InputIterator_c = constraint <
				Iterator_c<It>,
				requires<EqualityComparable<It>>,
				requires<NotEqualityComparable<It>>,
				requires<IsSame<typename std::iterator_traits<It>::reference, decltype(*std::declval<It&>())>>,
				requires <Convertible<decltype(*std::declval<It&>()), typename std::iterator_traits<It>::value_type>>,
				requires<Convertible< decltype(*std::declval<It&>()++), typename std::iterator_traits<It>::value_type>>,
				decltype(
				(void)(++std::declval<It&>())
					)
			>;

			template<class It>
			using OutputIterator_c = constraint<
				Iterator_c<It>,
				requires<IsSame<It&, decltype(++std::declval<It&>())>>,
				requires<Convertible<decltype(std::declval<It&>()++), const It&>>,
				decltype(
					*std::declval<It&>() = *std::declval<It&>(),
					(*std::declval<It&>())++ = *std::declval<It&>()
					)
			>;

			template<class It>
			using ForwardIterator_c = constraint<
				InputIterator_c<It>,
				requires<DefaultConstructible<It>>,
				requires<IsSame<It, decltype(std::declval<It&>()++)>>,
				requires<IsSame<typename std::iterator_traits<It>::reference, decltype(*std::declval<It&>()++)>>
			>;

			template<class It>
			using BidirectionalIterator_c = constraint<
				ForwardIterator_c<It>,
				requires<IsSame<It&, decltype(--std::declval<It&>())>>,
				requires<Convertible<decltype(std::declval<It&>()--), const It&>>,
				requires<IsSame<typename std::iterator_traits<It>::reference, decltype(*std::declval<It&>()--)>>
			>;
			template<class It>
			using RandomAccessIterator_c = constraint <
				BidirectionalIterator_c<It>,
				requires<LessThanComparable<It>>,
				requires<LessEqualComparable<It>>,
				requires<GreaterThanComparable<It>>,
				requires<GreaterEqualComparable<It>>,
				requires<IsSame<It&, decltype(std::declval<It&>() += std::declval<typename std::iterator_traits<It>::difference_type&>())>>,
				requires<IsSame<It, decltype(
					std::declval<It&>() + std::declval<typename std::iterator_traits<It>::difference_type&>(),
					std::declval<typename std::iterator_traits<It>::difference_type&>() + std::declval<It&>()
					)>>,
				requires<IsSame<It&, decltype(std::declval<It&>() -= std::declval<typename std::iterator_traits<It>::difference_type&>())>>,
				requires<IsSame<It, decltype(std::declval<It&>() - std::declval<typename std::iterator_traits<It>::difference_type&>())>>,
				requires<Convertible<
				decltype(std::declval<It&>()[std::declval<typename std::iterator_traits<It>::difference_type&>()]),
				typename std::iterator_traits<It>::reference>
				>
				>;

			template<class It>
			using ValueSwappable_c = constraint<
				Iterator_c<It>,
				requires<Swappable<typename std::iterator_traits<It>::value_type>>
			>;

		}//namespace detail

		///<summary>
		///イテレーターをもつか
		///</summary>
		TC_HAS_MEMBER_TYPE(HasIterator, iterator);

		///<summary>
		///イテレーターかどうか
		///</summary>
		template<class It>
		struct Iterator : to_concept<detail::Iterator_c>::type<It>
		{};
		///<summary>
		///入力イテレーターかどうか
		///</summary>
		template<class It>
		struct InputIterator : to_concept<detail::InputIterator_c>::type<It>
		{};

		///<summary>
		///出力イテレーターかどうか
		///</summary>
		template<class It>
		struct OutputIterator : to_concept<detail::OutputIterator_c>::type<It>
		{};

		///<summary>
		///前方イテレーターかどうか
		///</summary>
		template<class It>
		struct ForwardIterator : to_concept<detail::ForwardIterator_c>::type<It>
		{};
		///<summary>
		///双方向イテレーターかどうか
		///</summary>
		template<class It>
		struct BidirectionalIterator : to_concept<detail::BidirectionalIterator_c>::type<It>
		{};

		///<summary>
		///ランダムアクセスイテレーターかどうか
		///</summary>
		template<class It>
		struct RandomAccessIterator : to_concept<detail::RandomAccessIterator_c>::type<It>
		{};

		///<summary>
		///イテレーターの値型がスワップ可能か
		///</summary>
		template<class It>
		struct ValueSwappable : to_concept<detail::ValueSwappable_c>::type<It>
		{};


		//************************************************************************************************
		//
		//コンテナ
		//
		//************************************************************************************************

		namespace detail
		{

			template<class C, class IteratorConcept = Iterator<typename C::iterator>>
			using  Container_c = constraint<
				typename C::value_type,
				typename C::reference,
				typename C::const_reference,
				typename C::iterator,
				typename C::const_iterator,
				typename C::iterator,
				typename C::difference_type,
				typename C::size_type,
				requires<IteratorConcept>,
				requires<DefaultConstructible<C>>,
				requires<CopyConstructible<C>>,
				requires<Destructible<C>>,
				requires<EqualityComparable<C>>,
				requires<NotEqualityComparable<C>>,
				requires<CopyAssignable<C> >,
				decltype(
					std::declval<C&>().begin(),
					std::declval<C&>().end(),
					std::declval<C&>().cbegin(),
					std::declval<C&>().cend(),
					std::declval<C&>().swap(std::declval<C&>()),
					std::swap(std::declval<C&>(), std::declval<C&>()),
					std::declval<C&>().size(),
					std::declval<C&>().max_size(),
					std::declval<C&>().empty()
					)
			>;


			template<class C>
			using ReversibleContainer_c = constraint <
				Container_c<C>,
				requires<BidirectionalIterator<typename C::iterator>>,
				typename C::reverse_iterator,
				typename C::const_reverse_iterator,
				decltype(
					std::declval<C&>().rbegin(),
					std::declval<C&>().rend(),
					std::declval<C&>().crbegin(),
					std::declval<C&>().crend()
					)
			>;


			template<class C>
			using DefaultInsertable_c = constraint<
				Container_c<C>,
				requires<DefaultConstructible<typename C::value_type>>
			>;


			template<class C>
			using CopyInsertable_c = constraint<
				Container_c<C>,
				requires<CopyConstructible<typename C::value_type>>
			>;


			template<class C>
			using MoveInsertable_c = constraint<
				Container_c<C>,
				requires<MoveConstructible<typename C::value_type>>
			>;

			template <class C, class... Args>
			using EmplaceConstructible_c = constraint<
				Container_c<C>,
				requires<Constructible<typename C::value_type, Args...>>
			>;

			template<class C>
			using Erasable_c = constraint<
				Container_c<C>,
				requires<Destructible<typename C::value_type>>
			>;

			template<class C>
			using AllocatorAwareContainer_c = constraint<
				requires<CopyAssignable<C>>,
				CopyInsertable_c<C>,
				requires<MoveAssignable<C>>,
				MoveInsertable_c<C>,
				requires<DefaultConstructible<std::allocator<typename C::value_type>>>,
				requires<Constructible<C, std::allocator<typename C::value_type>>>,
				requires<Constructible < C, std::add_lvalue_reference_t<C>, std::allocator<typename C::value_type >> >,
				requires<Constructible < C, std::add_rvalue_reference_t<C>, std::allocator<typename C::value_type >> >,
				typename C::allocator_type,
				decltype(
					std::declval<C&>().get_allocator(),
					std::declval<C&>().swap(std::declval<C&>())
					)
			>;

		}//namespace detail

		///<summary>
		///コンテナかどうか
		///</summary>
		template<class C>
		struct Container : to_concept<detail::Container_c>::type<C, Iterator<C>>
		{};

		///<summary>
		///前方イテレーターをもつコンテナかどうか
		///</summary>
		template<class C>
		struct ForwardContainer : to_concept<detail::Container_c>::type< C, ForwardIterator<C>>
		{};
		///<summary>
		///ランダムアクセスイテレーターをもつコンテナかどうか
		///</summary>
		template<class C>
		struct RandomAccessContainer : to_concept<detail::Container_c>::type< C, RandomAccessIterator<C>>
		{};

		///<summary>
		///リバースイテレーターをもつコンテナかどうか
		///</summary>
		template<class C>
		struct ReversibleContainer : to_concept<detail::ReversibleContainer_c>::type<C>
		{};

		///<summary>
		///任意のコンテナCに対して、その要素型をデフォルトで挿入可能か
		///</summary>
		template<class C>
		struct DefaultInsertable :to_concept<detail::DefaultInsertable_c>::type<C>
		{};

		///<summary>
		///任意のコンテナCに対して、その要素型のコピー挿入可能か
		///</summary>
		template<class C>
		struct CopyInsertable :to_concept<detail::CopyInsertable_c>::type<C>
		{};
		///<summary>
		///任意のコンテナCに対して、その要素型の右辺値オブジェクトをムーブ挿入可能か
		///</summary>
		template<class C>
		struct MoveInsertable :to_concept<detail::MoveInsertable_c>::type<C>
		{};

		///<summary>
		///任意のコンテナCに対して、要素型のコンストラクタ引数列Argsから直接構築可能か
		///</summary>
		template <class C, class... Args>
		struct EmplaceConstructible : to_concept<detail::EmplaceConstructible_c>::type<C, Args...>
		{};

		///<summary>
		///任意のコンテナCに対して、要素型の破棄が可能か
		///</summary>
		template<class C>
		struct Erasable :to_concept<detail::Erasable_c>::type<C>
		{};

		///<summary>
		///アロケーターを認識するコンテナか
		///</summary>
		template<class C>
		struct AllocatorAwareContainer : to_concept<detail::AllocatorAwareContainer_c>::type<C>
		{};
		//************************************************************************************************
		//
		//その他
		//
		//************************************************************************************************

		///<summary>
		///条件式conditionalをみたすか
		///</summary>
		template<bool conditional>
		struct Condition : std::bool_constant<conditional>
		{};

	}//namespace Concept


}//namespace tc


//************************************************************************************************
//
//concept_map
//
//************************************************************************************************

namespace tc
{

	template<class Concept>
	struct concept_map
	{
		using is_default = void;

		template<class T>
		T&& operator =(T&& other)
		{
			return std::forward<T>(other);
		}
	};

	namespace detail
	{
		template<class Concept, class Type, class = void>
		struct concept_mapping_impl : concept_map<void>
		{
			using concept_map<void>::operator=;
		};
		template<class Concept, class Type>
		struct concept_mapping_impl<Concept, Type, void_t<decltype(concept_map<Concept>() = tc::val<Type&>)>> :concept_map<Concept>
		{
			using concept_map<Concept>::operator=;
		};

		template<class... Type>
		auto ref_make_tuple(Type&&... arg)->decltype(std::tuple<Type&&...>(arg...))
		{
			return std::tuple<Type&&...>(arg...);
		}

		template<class Concept, class ...Type>
		auto make_mapping_tuple(Type&&... value)->decltype(ref_make_tuple((concept_mapping_impl<Concept, Type>() = value)...))
		{
			return ref_make_tuple((concept_mapping_impl<Concept, Type>() = value)...);
		}
	}//namespace detail


	 ///<summary>
	 ///インスタンスにコンセプトマップを適応させる
	 ///</summary>
	template<class Concept, class Type>
	auto concept_mapping(Type&& value)->decltype(detail::concept_mapping_impl<Concept, Type>() = value)
	{

		return detail::concept_mapping_impl<Concept, Type>() = value;
	}
	template<template<class...>class Concept, class ...Type>
	auto concept_mapping(Type&&... value)->decltype(detail::make_mapping_tuple<Concept<std::remove_reference_t<Type>...>>(value...))
	{
		using C = Concept<std::remove_reference_t<Type>...>;
		return detail::make_mapping_tuple<C>(value...);
	}


}//namespace tc

///<summary>
///whereマクロ
///</summary>
#define TC_WHERE( ... ) tc::requires< __VA_ARGS__ > = nullptr

///<summary>
///条件をみたさないとアサート
///</summary>
#define TC_CONCEPT_ASSERT( ... ) static_assert(__VA_ARGS__::value,#__VA_ARGS__ )


