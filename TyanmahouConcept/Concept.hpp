#pragma once
#include<type_traits>
#include<tuple>
#include<memory>
#include<any>

//************************************************************************************************
//
//macro
//
//************************************************************************************************

///<summary>
///whereマクロ
///</summary>
#define TC_WHERE( ... ) tc::requires< __VA_ARGS__ > = nullptr

///<summary>
///whereマクロ(bool)
///</summary>
#define TC_WHERE_BOOL( ... ) tc::requires_bool< static_cast<bool>(__VA_ARGS__) > = nullptr

///<summary>
///条件をみたさないとアサート
///</summary>
#define TC_CONCEPT_ASSERT( ... ) static_assert(__VA_ARGS__::value,#__VA_ARGS__ )

///<summary>
///条件をみたさないとアサート(bool)
///</summary>
#define TC_CONCEPT_ASSERT_BOOL( ... ) static_assert(static_cast<bool>(__VA_ARGS__),#__VA_ARGS__ )

//************************************************************************************************
//
//util
//
//************************************************************************************************

namespace tc
{

	namespace detail
	{
		template<class...>
		struct void_t_impl
		{
			typedef void type;
		};
	}
	template<class... Test>
	using void_t = typename detail::void_t_impl<Test...>::type;

	namespace detail
	{

		template<class, template<class...>class Constraint, class ...Args>
		struct is_detected_impl :std::false_type
		{};

		template<template<class...>class Constraint, class ...Args>
		struct is_detected_impl<void_t<Constraint<Args...>>, Constraint, Args...> :std::true_type
		{};
	}
	//-- TODO std::is_detected

	template<template<class...>class Constraint, class ...Args>
	using is_detected = detail::is_detected_impl <void, Constraint, Args...>;



	///<summary>
	///仮のインスタンス作成(declvalのヘルパ)
	///</summary>
	template<class Type>
	Type&& val = std::declval<Type>();


}//namesapce tc

 //************************************************************************************************
 //
 //requires
 //
 //************************************************************************************************
namespace tc
{

	///<summary>
	///WhereConceptが条件を満たすか
	///</summary>
	template < class WhereConcept >
	using requires = std::enable_if_t<WhereConcept::value, std::nullptr_t >;

	///<summary>
	///Testがtureかどうか
	///</summary>
	template < bool Test >
	using requires_bool = std::enable_if_t<Test, std::nullptr_t >;


}//namesapce tc

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
		struct concept_mapping_impl<Concept, Type, void_t<decltype(concept_map<Concept>() = tc::val<Type>)>> :concept_map<Concept>
		{
			using concept_map<Concept>::operator=;
		};

		template<class... Type>
		auto ref_make_tuple(Type&&... arg)->decltype(std::tuple<Type...>(std::forward<Type>(arg)...))
		{
			return std::tuple<Type...>(std::forward<Type>(arg)...);
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
	///<summary>
	///インスタンスにコンセプトマップを適応させる 返り値tuple
	///</summary>
	template<template<class...>class Concept, class ...Type>
	auto concept_mapping(Type&&... value)->decltype(detail::make_mapping_tuple<Concept<std::remove_reference_t<Type>...>>(value...))
	{
		using C = Concept<std::remove_reference_t<Type>...>;
		return detail::make_mapping_tuple<C>(value...);
	}

	namespace detail
	{
		template<class Concept, class Arg>
		struct concept_mapped
		{
			using type = std::remove_reference_t<decltype(tc::concept_mapping<Concept>(tc::val<Arg&>))>;
		};

	}
	///<summary>
	///コンセプトマップ後の型
	///</summary>
	template<class Concept, class Arg>
	using concept_mapped_t = typename detail::concept_mapped<Concept, Arg>::type;


	///<summary>
	///コンセプトマップ後適用後の型として判定
	///</summary>
	template<class Type>
	struct as_mapped
	{};

	namespace detail
	{
		template<class Concept, class Arg>
		struct as_mapped_if
		{
			using type = Arg;
		};
		template<class Concept, class Arg>
		struct as_mapped_if<Concept, as_mapped<Arg>>
		{
			using type = concept_mapped_t<Concept, Arg>;
		};
		template<class Concept, class Arg>
		using as_mapped_if_t = typename as_mapped_if<Concept, Arg>::type;

		template<class Arg>
		struct remove_mapped
		{
			using type = Arg;
		};
		template<class Arg>
		struct remove_mapped<as_mapped<Arg>>
		{
			using type = Arg;
		};

		template<class Arg>
		using remove_mapped_t = typename remove_mapped<Arg>::type;
	}

	///<summary>
	///Typeがas_mappedだった場合マッピング後の型に
	///</summary>
	template<class Type, template<class...>class Concept, class ...Args>
	using as_mapped_if_t = typename tc::detail::as_mapped_if_t<Concept<typename tc::detail::remove_mapped_t<Args>...>, Type>;

	///<summary>
	///Conceptの継承
	///</summary>
	template<template<class...>class Sub, template<class...>class Super, class ...Args>
	using concept_extends = Super<as_mapped_if_t<Args, Sub, Args...>...>;
}//namespace tc

 //************************************************************************************************
 //
 //constraint
 //
 //************************************************************************************************

namespace tc
{
	namespace detail
	{
		template<class Concept, class ... Args>
		using ConceptCheck = decltype(&Concept::template requires<Args...>);

	}

	///<summary>
	///制約
	///</summary>
	template<class ...Args>
	using constraint = void_t<Args...>;


	///<summary>
	///requires実装クラスをコンセプト(メタ関数)に変換
	///</summary>
	template<class Constraint, class ...Args>
	struct to_concept : is_detected<
		detail::ConceptCheck,
		Constraint,
		as_mapped_if_t<
		Args,
		to_concept,
		Constraint,
		Args...
		>...
	>
	{};

	///<summary>
	///void_t/constraintをコンセプト(メタ関数)に変換
	///</summary>
	template<template<class...>class Constraint, class ...Args>
	struct void_t_to_concept : is_detected<
		Constraint,
		detail::as_mapped_if_t<
		void_t_to_concept<Constraint, detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};

	///<summary>
	///通常のメタ関数をコンセプト(メタ関数)に変換
	///</summary>
	template<template<class...>class Meta, class ...Args>
	struct meta_to_concept : Meta<
		detail::as_mapped_if_t<
		meta_to_concept<Meta, detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};

	///<summary>
	///Type型が存在するか
	///</summary>
	template<class Type>
	Type&& has_type = val<Type>;

	///<summary>
	///式ExpがRet型か
	///</summary>
	template<class Ret, class Exp>
	auto type_check(Exp&& exp)->tc::requires<std::is_same<Ret, Exp>>;

}//namespace tc

 //************************************************************************************************
 //
 //concept_any
 //
 //************************************************************************************************

namespace tc
{
	namespace detail
	{
		//実装
		template<class T>
		struct AnyCastImpl
		{
			static T Func(std::any& _any)
			{
				using Type = std::remove_const_t<T>;
				if (auto& type = _any.type(); type == typeid(std::reference_wrapper<Type>))
				{
					return std::any_cast<std::reference_wrapper<Type>>(_any);
				}
				else if (type == typeid(std::reference_wrapper<const Type>))
				{
					return std::any_cast<std::reference_wrapper<const Type>>(_any);
				}
				return std::any_cast<T>(_any);
			}
		};
		//参照
		template<class T>
		struct AnyCastImpl<T&>
		{
			static T& Func(std::any& _any)
			{
				if (_any.type() == typeid(std::reference_wrapper<T>))
				{
					return std::any_cast<std::reference_wrapper<T>>(_any);
				}
				return std::any_cast<T&>(_any);
			}
		};
		//const参照
		template<class T>
		struct AnyCastImpl<const T&>
		{
			static const T& Func(std::any& _any)
			{
				if (auto& type = _any.type(); type == typeid(std::reference_wrapper<T>))
				{
					return std::any_cast<std::reference_wrapper<T>>(_any);
				}
				else if (type == typeid(std::reference_wrapper<const T>))
				{
					return std::any_cast<std::reference_wrapper<const T>>(_any);
				}
				return std::any_cast<T&>(_any);
			}
		};

		//ラップ関数
		template<class T>
		T any_cast_wrapper(std::any& _any)
		{
			return AnyCastImpl<T>::Func(_any);
		}

	}
	///<summary>
	///コンセプトを満たす型
	///</summary>
	template<template<class>class Concept>
	class concept_any : protected std::any
	{
	public:

		concept_any() = default;

		template<class T, TC_WHERE(Concept<tc::concept_mapped_t<Concept<T>, T>>)>
		concept_any(const T& v) :
			std::any(v)
		{}
		template<class T, TC_WHERE(Concept<tc::concept_mapped_t<Concept<T>, T>>)>
		concept_any(const std::reference_wrapper<T>& v) :
			std::any(v)
		{}
		template<class T, TC_WHERE(Concept<tc::concept_mapped_t<Concept<T>, T>>)>
		concept_any& operator=(const T& v)
		{
			return static_cast<concept_any&>(std::any::operator=(v));
		}

		using std::any::emplace;
		using std::any::has_value;
		using std::any::reset;
		using std::any::type;
		void swap(concept_any& any)
		{
			std::any::swap(any);
		}

		template<class T>
		decltype(auto) get()
		{
			decltype(auto) v = get_origin<T>();
			return tc::concept_mapping<Concept<std::remove_reference_t<T>>>(v);
		}
		template<class T>
		decltype(auto) get_origin()
		{
			return detail::any_cast_wrapper<T>(*this);
		}
	};

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


			struct PostIncrementable_c
			{
				template<class Type>
				auto requires(Type&& t)->decltype(t++);
			};
			struct PostDecrementable_c
			{
				template<class Type>
				auto requires(Type&& t)->decltype(t--);
			};
		}


#define TC_HAS_UNARY_OPERATOR(className,symbol)\
namespace detail\
{\
	struct className##_c{\
	template<class Type>\
	auto requires(Type&& t)->decltype(symbol t);\
	};\
}\
template<class Type>\
struct className : tc::to_concept<detail::className##_c,tc::as_mapped_if_t<Type,className,Type>>{}


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
		struct PostIncrementable :to_concept<detail::PostIncrementable_c, tc::as_mapped_if_t<Type, PostIncrementable, Type>> {};

		///<summary>
		/// 後置デクリメント可能か
		///</summary>
		template<class Type>
		struct PostDecrementable : to_concept<detail::PostDecrementable_c, tc::as_mapped_if_t<Type, PostDecrementable, Type>> {};

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
namespace detail\
{\
	struct className##_c{\
	template<class Left,class Right=Left>\
	auto requires(Left&& l,Right&& r)->decltype(l symbol r);\
	};\
}\
template<class Left,class Right=Left>\
struct className:tc::to_concept<detail::className##_c,\
		tc::as_mapped_if_t<Left,className, Left,Right>,\
		tc::as_mapped_if_t<Right,className, Left,Right>\
>{}

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
		TC_HAS_BINARY_OPERATOR(LessThanComparable, <);

		///<summary>
		/// 小なり=比較演算 をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessEqualComparable, <= );

		///<summary>
		/// 大なり比較演算をもつか
		///</summary>
		TC_HAS_BINARY_OPERATOR(GreaterThanComparable, >);

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
		//基本的なコンセプト
		//
		//************************************************************************************************

		///<summary>
		/// operator =(copy) をもつか
		///</summary>
		template< class To, class From = To>
		struct CopyAssignable : std::is_assignable<
			std::add_lvalue_reference_t<as_mapped_if_t<To, CopyAssignable, To, From>>,
			std::add_lvalue_reference_t<std::add_const_t<as_mapped_if_t<From, CopyAssignable, To, From>>>
		> {};

		///<summary>
		/// operator =(move) をもつか
		///</summary>
		template< class To, class From = To>
		struct MoveAssignable : std::is_assignable<
			std::add_lvalue_reference_t<as_mapped_if_t<To, MoveAssignable, To, From>>,
			std::add_rvalue_reference_t<as_mapped_if_t<From, MoveAssignable, To, From>>
		> {};

		///<summary>
		/// Type( Args... ) の形式のコンストラクタ呼び出しが可能か
		///</summary>
		template< class Type, class... Args>
		struct Constructible : std::is_constructible<
			as_mapped_if_t<Type, Constructible, Type, Args...>,
			as_mapped_if_t<Args, Constructible, Type, Args...>...
		> {};

		///<summary>
		/// デフォルトコンストラクタ をもつか
		///</summary>
		template< class Type >
		struct DefaultConstructible : concept_extends<DefaultConstructible, std::is_default_constructible, Type> {};

		///<summary>
		/// コピーコンストラクタ をもつか
		///</summary>
		template<class Type>
		struct CopyConstructible : concept_extends<CopyConstructible, std::is_default_constructible, Type> {};

		///<summary>
		/// ムーブコンストラクタ をもつか
		///</summary>
		template< class Type>
		struct MoveConstructible : concept_extends<MoveConstructible, std::is_move_constructible, Type> {};

		///<summary>
		/// デストラクタ をもつか
		///</summary>
		template< class Type>
		struct Destructible : concept_extends<Destructible, std::is_destructible, Type> {};

		///<summary>
		/// 仮想デストラクタ をもつか
		///</summary>
		template< class Type>
		struct  HasVirtualDestructor :tc::concept_extends<HasVirtualDestructor, std::has_virtual_destructor, Type> {};

		///<summary>
		///TとUが同じか
		///</summary>
		template<class T, class U>
		struct  IsSame :tc::concept_extends<IsSame, std::is_same, T, U> {};

		///<summary>
		///TypeがBase,もしくはBaseを継承しているか
		///</summary>
		template<class Type, class Base>
		struct Extended :std::is_base_of<
			as_mapped_if_t<Base, Extended, Type, Base>,
			as_mapped_if_t<Type, Extended, Type, Base>
		> {};

		///<summary>
		///スカラーかどうか
		///</summary>
		template<class Type>
		struct Scalar : tc::concept_extends<Scalar, std::is_scalar, Type> {};

		///<summary>
		///仮想クラスかどうか
		///</summary>
		template<class Type>
		struct Abstract : tc::concept_extends<Abstract, std::is_abstract, Type> {};

		///<summary>
		///enum型か
		///</summary>
		template<class Type>
		struct Enum : tc::concept_extends<Enum, std::is_enum, Type> {};

		///<summary>
		///class(struct)型か
		///</summary>
		template<class Type>
		struct Class : tc::concept_extends<Class, std::is_class, Type> {};

		///<summary>
		///union型か
		///</summary>
		template<class Type>
		struct Union : tc::concept_extends<Union, std::is_union, Type> {};

		///<summary>
		///関数型t(...)か
		///</summary>
		template<class Type>
		struct Function : tc::concept_extends<Function, std::is_function, Type> {};


		//************************************************************************************************
		//
		//型の特性
		//
		//************************************************************************************************

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
		 /*
		 ///<summary>
		 ///アロケーターか
		 ///</summary>
		 template<class Type>
		 TC_TO_CONCEPT(Allocator, detail::Allocator_c, Type);
		 ///<summary>
		 ///TのオブジェクトとUのオブジェクトが入れ替え可能か
		 ///</summary>
		 template<class T, class U = T>
		 TC_TO_CONCEPT(Swappable, detail::Swappable_c, std::add_lvalue_reference_t<T>, std::add_lvalue_reference_t<U>);
		 ///<summary>
		 /// FromがToにキャストできるか
		 ///</summary>
		 template< class From, class To >
		 struct Convertible : tc::detail::to_concept<Convertible, std::is_convertible, From, To>
		 {};
		 ///<summary>
		 /// null許容か
		 ///</summary>
		 template<class Type>
		 TC_TO_CONCEPT(NullablePointer, detail::NullablePointer_c, Type);
		 //--TODO is_invocableに変更したい--
		 ///<summary>
		 ///関数呼び出し可能な型か
		 ///</summary>
		 template <class Func, class... Args>
		 TC_TO_CONCEPT(Invocable, detail::Invocable_c, Func, Args...);
		 ///<summary>
		 ///関数オブジェクトか
		 ///</summary>
		 template <class Type>
		 TC_TO_CONCEPT(FunctionObject, detail::FunctionObject_c, Type);
		 ///<summary>
		 ///コンセプトか
		 ///</summary>
		 template <class Type>
		 TC_TO_CONCEPT(Concept, detail::Concept_c, Type);
		 ///<summary>
		 ///ハッシュ関数オブジェクトか
		 ///</summary>
		 template <class Type, class Key>
		 TC_TO_CONCEPT(Hash, detail::Hash_c, Type, Key);
		 ///<summary>
		 ///期間、時刻、現在の時刻を取得可能か
		 ///</summary>
		 template<class Type>
		 TC_TO_CONCEPT(Clock, detail::Clock_c, Type);
		 //************************************************************************************************
		 //
		 //レイアウト
		 //
		 //************************************************************************************************
		 ///<summary>
		 ///トリビアルコピー可能か
		 ///</summary>
		 template<class Type>
		 struct TriviallyCopyable : tc::detail::to_concept<TriviallyCopyable, std::is_trivially_copyable, Type>
		 {};
		 ///<summary>
		 ///トリビアル型か
		 ///</summary>
		 template<class Type>
		 struct TrivialType : tc::detail::to_concept<TrivialType, std::is_trivial, Type>
		 {};
		 ///<summary>
		 ///標準レイアウト型か
		 ///</summary>
		 template<class Type>
		 struct StandardLayoutType : tc::detail::to_concept<StandardLayoutType, std::is_standard_layout, Type>
		 {};
		 ///<summary>
		 ///POD型か
		 ///</summary>
		 template<class Type>
		 struct PODType : tc::detail::to_concept<PODType, std::is_pod, Type>
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
		 TC_TO_CONCEPT(Iterator, detail::Iterator_c, It);
		 ///<summary>
		 ///入力イテレーターかどうか
		 ///</summary>
		 template<class It>
		 TC_TO_CONCEPT(InputIterator, detail::InputIterator_c, It);
		 ///<summary>
		 ///出力イテレーターかどうか
		 ///</summary>
		 template<class It>
		 TC_TO_CONCEPT(OutputIterator, detail::OutputIterator_c, It);
		 ///<summary>
		 ///前方イテレーターかどうか
		 ///</summary>
		 template<class It>
		 TC_TO_CONCEPT(ForwardIterator, detail::ForwardIterator_c, It);
		 ///<summary>
		 ///双方向イテレーターかどうか
		 ///</summary>
		 template<class It>
		 TC_TO_CONCEPT(BidirectionalIterator, detail::BidirectionalIterator_c, It);
		 ///<summary>
		 ///ランダムアクセスイテレーターかどうか
		 ///</summary>
		 template<class It>
		 TC_TO_CONCEPT(RandomAccessIterator, detail::RandomAccessIterator_c, It);
		 ///<summary>
		 ///イテレーターの値型がスワップ可能か
		 ///</summary>
		 template<class It>
		 TC_TO_CONCEPT(ValueSwappable, detail::ValueSwappable_c, It);
		 //************************************************************************************************
		 //
		 //コンテナ
		 //
		 //************************************************************************************************
		 namespace detail
		 {
		 template<class X>
		 using  Container_c = constraint<
		 typename X::value_type,
		 typename X::reference,
		 typename X::const_reference,
		 typename X::iterator,
		 typename X::const_iterator,
		 typename X::iterator,
		 typename X::difference_type,
		 typename X::size_type,
		 requires<DefaultConstructible<X>>,
		 requires<CopyConstructible<X>>,
		 requires<Destructible<X>>,
		 requires<EqualityComparable<X>>,
		 requires<NotEqualityComparable<X>>,
		 requires<CopyAssignable<X> >,
		 decltype(
		 std::declval<X&>().begin(),
		 std::declval<X&>().end(),
		 std::declval<X&>().cbegin(),
		 std::declval<X&>().cend(),
		 std::declval<X&>().swap(std::declval<X&>()),
		 std::swap(std::declval<X&>(), std::declval<X&>()),
		 std::declval<X&>().size(),
		 std::declval<X&>().max_size(),
		 std::declval<X&>().empty()
		 )
		 > ;
		 template<class X>
		 using  ForwardContainer_c = constraint<
		 Container_c<X>,
		 ForwardIterator_c<typename X::iterator>
		 >;
		 template<class X>
		 using RandomAccessContainer_c= constraint<
		 Container_c<X>,
		 RandomAccessIterator_c<typename X::iterator>
		 >;
		 template<class X>
		 using ReversibleContainer_c = constraint <
		 Container_c<X>,
		 requires<BidirectionalIterator<typename X::iterator>>,
		 typename X::reverse_iterator,
		 typename X::const_reverse_iterator,
		 decltype(
		 std::declval<X&>().rbegin(),
		 std::declval<X&>().rend(),
		 std::declval<X&>().crbegin(),
		 std::declval<X&>().crend()
		 )
		 >;
		 template<class X>
		 using DefaultInsertable_c = constraint<
		 Container_c<X>,
		 requires<DefaultConstructible<typename X::value_type>>
		 >;
		 template<class X>
		 using CopyInsertable_c = constraint<
		 Container_c<X>,
		 requires<CopyConstructible<typename X::value_type>>
		 >;
		 template<class X>
		 using MoveInsertable_c = constraint<
		 Container_c<X>,
		 requires<MoveConstructible<typename X::value_type>>
		 >;
		 template <class X, class... Args>
		 using EmplaceConstructible_c = constraint<
		 Container_c<X>,
		 requires<Constructible<typename X::value_type, Args...>>
		 >;
		 template<class X>
		 using Erasable_c = constraint<
		 Container_c<X>,
		 requires<Destructible<typename X::value_type>>
		 >;
		 template<class X>
		 using AllocatorAwareContainer_c = constraint<
		 requires<CopyAssignable<X>>,
		 CopyInsertable_c<X>,
		 requires<MoveAssignable<X>>,
		 MoveInsertable_c<X>,
		 requires<DefaultConstructible<std::allocator<typename X::value_type>>>,
		 requires<Constructible < X, std::allocator<typename X::value_type>>>,
		 requires<Constructible < X, std::add_lvalue_reference_t<X>, std::allocator<typename X::value_type >> >,
		 requires<Constructible < X, std::add_rvalue_reference_t<X>, std::allocator<typename X::value_type >> >,
		 typename X::allocator_type,
		 decltype(
		 std::declval<X&>().get_allocator(),
		 std::declval<X&>().swap(std::declval<X&>())
		 )
		 >;
		 }//namespace detail
		 ///<summary>
		 ///コンテナかどうか
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(Container, detail::Container_c, X);
		 ///<summary>
		 ///前方イテレーターをもつコンテナかどうか
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(ForwardContainer, detail::ForwardContainer_c, X);
		 ///<summary>
		 ///ランダムアクセスイテレーターをもつコンテナかどうか
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(RandomAccessContainer, detail::RandomAccessContainer_c, X);
		 ///<summary>
		 ///リバースイテレーターをもつコンテナかどうか
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(ReversibleContainer, detail::ReversibleContainer_c, X);
		 ///<summary>
		 ///任意のコンテナCに対して、その要素型をデフォルトで挿入可能か
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(DefaultInsertable, detail::DefaultInsertable_c, X);
		 ///<summary>
		 ///任意のコンテナCに対して、その要素型のコピー挿入可能か
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(CopyInsertable, detail::CopyInsertable_c, X);
		 ///<summary>
		 ///任意のコンテナCに対して、その要素型の右辺値オブジェクトをムーブ挿入可能か
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(MoveInsertable, detail::MoveInsertable_c, X);
		 ///<summary>
		 ///任意のコンテナCに対して、要素型のコンストラクタ引数列Argsから直接構築可能か
		 ///</summary>
		 template <class X, class... Args>
		 TC_TO_CONCEPT(EmplaceConstructible, detail::EmplaceConstructible_c, X, Args...);
		 ///<summary>
		 ///任意のコンテナCに対して、要素型の破棄が可能か
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(Erasable, detail::Erasable_c, X);
		 ///<summary>
		 ///アロケーターを認識するコンテナか
		 ///</summary>
		 template<class X>
		 TC_TO_CONCEPT(AllocatorAwareContainer, detail::AllocatorAwareContainer_c, X);
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
		 */
	}//namespace Concept


}//namespace tc