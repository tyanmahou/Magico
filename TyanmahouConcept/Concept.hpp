#pragma once
#include<type_traits>
#include<tuple>
#include<functional>
#include<memory>
#include<any>

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
	auto val()->decltype(std::declval<Type>());

	///<summary>
	///メタ関数 論理和
	///</summary>
	template<class Head, class... Tail>
	struct Or :std::conditional_t <  Head::value, std::true_type, Or < Tail... > >
	{};
	template<class Concpet>
	struct Or<Concpet> :std::bool_constant<Concpet::value>
	{};
	///<summary>
	///メタ関数 否定
	///</summary>
	template < class Concpet >
	struct Not : std::bool_constant <!Concpet::value >
	{};
	///<summary>
	///メタ関数 論理積
	///</summary>
	template <class ... Args>
	struct And :Not < Or  < Not < Args >... > >
	{};

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
	template < class... WhereConcept >
	using requires = std::enable_if_t<And<WhereConcept...>::value, std::nullptr_t >;

	///<summary>
	///Testがtureかどうか
	///</summary>
	template < bool Test >
	using requires_bool = std::enable_if_t<Test, std::nullptr_t >;


	///<summary>
	///制約チェック　Ret：返り値の型　Concept 制約
	///</summary>
	template<class Ret, class ...Concept>
	using where = std::enable_if_t<
		std::is_same<tc::requires<Concept...>, std::nullptr_t>::value,
		Ret>;
	///<summary>
	///制約チェック　Ret：返り値の型　Exp bool値
	///</summary>
	template<class Ret, bool Exp>
	using where_bool = std::enable_if_t<Exp,Ret>;
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
		struct concept_mapping_impl<Concept, Type, void_t<decltype(concept_map<Concept>() = tc::val<Type>())>> :concept_map<Concept>
		{
			using concept_map<Concept>::operator=;
		};

		template<class... Type>
		decltype(auto) ref_make_tuple(Type&&... arg)
		{
			return std::tuple<Type...>(std::forward<Type>(arg)...);
		}

		template<class Concept, class ...Type>
		decltype(auto)  make_mapping_tuple(Type&&... value)
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
	decltype(auto) concept_mapping(Type&&... value)
	{
		using C = Concept<std::remove_reference_t<Type>...>;
		return detail::make_mapping_tuple<C>(value...);
	}

	namespace detail
	{
		template<class Concept, class Arg>
		struct concept_mapped
		{
			using type = std::remove_reference_t<decltype(tc::concept_mapping<Concept>(tc::val<Arg&>()))>;
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
	template<class Type, template<class...>class Concept, class ... Args>
	using as_mapped_if_t = typename tc::detail::as_mapped_if_t<Concept<typename tc::detail::remove_mapped_t<Args>...>, Type>;


}//namespace tc

 //************************************************************************************************
 //
 //constraint
 //
 //************************************************************************************************

namespace tc
{

	/*
	to_concept
	*/
	namespace detail
	{
		template<class Concept, class ... Args>
		using ConceptCheck = decltype(&Concept::template requires<Args...>);

	}


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
	///requires実装クラスをコンセプト(メタ関数)に変換し継承 SubConceptには派生classを与える
	///</summary>
	template<template<class...>class SubConcept,class Constraint, class ...Args>
	struct to_concept_ex : is_detected<
		detail::ConceptCheck,
		Constraint,
		detail::as_mapped_if_t<
		SubConcept<detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};
	///<summary>
	///alias形式constraintをコンセプト(メタ関数)に変換
	///</summary>
	template<template<class...>class Constraint, class ...Args>
	struct alias_to_concept : is_detected<
		Constraint,
		detail::as_mapped_if_t<
		alias_to_concept<Constraint, detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};
	///<summary>
	///alias形式constraintをコンセプト(メタ関数)に変換し継承 SubConceptには派生classを与える
	///</summary>
	template<template<class...>class SubConcept,template<class...>class Constraint, class ...Args>
	struct alias_to_concept_ex : is_detected<
		Constraint,
		detail::as_mapped_if_t<
		SubConcept<detail::remove_mapped_t<Args>...>,
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
	///通常のメタ関数をコンセプト(メタ関数)に変換し継承 SubConceptには派生classを与える
	///</summary>
	template<template<class...>class SubConcept,template<class...>class Meta, class ...Args>
	struct meta_to_concept_ex : Meta<
		detail::as_mapped_if_t<
		SubConcept<detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};

	/*
	requires helper
	*/
	///<summary>
	///Type型が存在するか
	///</summary>
	template<class Type>
	auto associated_type()->decltype(val<Type>());

	///<summary>
	///式が評価可能でRet型か
	///</summary>
	///<param name= "exp">
	///評価する式
	///</param>
	template<class Ret, class Exp>
	auto vailed_expr(Exp&& exp)->tc::requires<std::is_same<Ret, Exp>>;

	///<summary>
	///式がType型に変更可能か
	///</summary>
	///<param name= "exp">
	///評価する式
	///</param>
	template<class Type, class Exp>
	auto convertible_expr(Exp&& exp)->tc::requires<std::is_convertible<Exp,Type>>;

	///<summary>
	///制約の継承
	///</summary>
	template<class... Concept>
	tc::requires<Concept...> requires_extends = nullptr;

	///<summary>
	///制約の継承(requires実装classから)
	///</summary>
	///<param name= "requires">
	///requires実装classのrequires関数のアドレス
	///</param>
	template<class... RequiresFuncAddress>
	void requires_extends_c(RequiresFuncAddress&&... requires);





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

		template<class T, tc::requires<Concept<tc::as_mapped<T>>> = nullptr>
		concept_any(const T& v) :
			std::any(v)
		{}
		template<class T, tc::requires<Concept<tc::as_mapped<T>>> = nullptr>
		concept_any(const std::reference_wrapper<T>& v) :
			std::any(v)
		{}
		template<class T, tc::requires<Concept<tc::as_mapped<T>>> = nullptr>
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
	namespace concept
	{
		//************************************************************************************************
		//
		//Operator
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
struct className : tc::to_concept_ex<className,detail::className##_c,Type>{}


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
struct className:tc::to_concept_ex<className,detail::className##_c,Left,Right>{}

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
		struct DefaultConstructible : meta_to_concept_ex <DefaultConstructible, std::is_default_constructible, Type> {};

		///<summary>
		/// コピーコンストラクタ をもつか
		///</summary>
		template<class Type>
		struct CopyConstructible : meta_to_concept_ex<CopyConstructible, std::is_copy_constructible, Type> {};

		///<summary>
		/// ムーブコンストラクタ をもつか
		///</summary>
		template< class Type>
		struct MoveConstructible : meta_to_concept_ex<MoveConstructible, std::is_move_constructible, Type> {};

		///<summary>
		/// デストラクタ をもつか
		///</summary>
		template< class Type>
		struct Destructible : meta_to_concept_ex<Destructible, std::is_destructible, Type> {};

		///<summary>
		/// 仮想デストラクタ をもつか
		///</summary>
		template< class Type>
		struct  HasVirtualDestructor :tc::meta_to_concept_ex<HasVirtualDestructor, std::has_virtual_destructor, Type> {};

		///<summary>
		///TとUが同じか
		///</summary>
		template<class T, class U>
		struct  IsSame :tc::meta_to_concept_ex<IsSame, std::is_same, T, U> {};

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
		struct Scalar : tc::meta_to_concept_ex<Scalar, std::is_scalar, Type> {};

		///<summary>
		///仮想クラスかどうか
		///</summary>
		template<class Type>
		struct Abstract : tc::meta_to_concept_ex<Abstract, std::is_abstract, Type> {};

		///<summary>
		///enum型か
		///</summary>
		template<class Type>
		struct Enum : tc::meta_to_concept_ex<Enum, std::is_enum, Type> {};

		///<summary>
		///class(struct)型か
		///</summary>
		template<class Type>
		struct Class : tc::meta_to_concept_ex<Class, std::is_class, Type> {};

		///<summary>
		///union型か
		///</summary>
		template<class Type>
		struct Union : tc::meta_to_concept_ex<Union, std::is_union, Type> {};

		///<summary>
		///関数型t(...)か
		///</summary>
		template<class Type>
		struct Function : tc::meta_to_concept_ex<Function, std::is_function, Type> {};


		//************************************************************************************************
		//
		//Library-wide
		//
		//************************************************************************************************

		namespace detail
		{
			struct Allocator_c
			{
			private:
				size_t n;
			public:
				template<class Type>
				auto requires(Type&& alloc, typename Type::value_type* ptr, const typename Type::value_type* cptr)->decltype(
					requires_extends<
					EqualityComparable<Type>,
					NotEqualityComparable<Type>,
					CopyConstructible<Type>,
					MoveConstructible<Type>
					>,
					vailed_expr< typename Type::value_type& >(*ptr),
					vailed_expr< const typename Type::value_type& >(*cptr),
					vailed_expr< typename Type::value_type* >(alloc.allocate(n)),
					alloc.deallocate(ptr, n)
					);

	
			};

			struct Swappable_c
			{
				template<class T, class U>
				auto requires(T&& a, U&& b)->decltype(
					std::swap(a, b),
					std::swap(b, a)
					);
			};

			struct NullablePointer_c
			{
				template<class Type>
				auto requires(Type&& null)->decltype(
					requires_extends<
					EqualityComparable<Type>,
					NotEqualityComparable<Type>,
					DefaultConstructible<Type>,
					CopyConstructible<Type>,
					Constructible<Type, std::nullptr_t>,
					CopyAssignable<Type>,
					Destructible<Type>
					>,
					null = nullptr,
					null == nullptr, nullptr == null,
					null != nullptr, nullptr != null
					);
			};

			struct Invocable_c
			{
				template <class F, class... Args>
				auto requires(F&& func, Args&&... args)->decltype(
					std::invoke(func, args...)
					);
			};


			struct FunctionObject_c
			{
				template <class Type>
				auto requires()->decltype(
					&Type::operator()
					);
			};

			struct MetaFunc_c
			{
				template <class Type>
				auto requires()->decltype(
					vailed_expr<const bool&>(Type::value)
					);
			};

			struct Hash_c
			{
				template <class Type, class Key>
				auto requires(Type&& t, Key&& key)->decltype(
					requires_extends<
					CopyConstructible<Type>,
					Destructible<Type>
					>,
					vailed_expr<size_t>(t(key))
					);
			};

			struct Clock_c
			{
				template <class Type>
				auto requires(Type&& t)->decltype(
					tc::associated_type<typename Type::rep>(),
					tc::associated_type<typename Type::period>(),
					tc::associated_type<typename Type::duration>(),
					tc::associated_type<typename Type::time_point>(),
					vailed_expr<const bool&>(t.is_steady),
					vailed_expr<typename Type::time_point>(t.now())
					);
			};
		}//namespace detail

		 ///<summary>
		 ///アロケーターか
		 ///</summary>
		template<class Type>
		struct Allocator :to_concept_ex<Allocator, detail::Allocator_c, Type>
		{};


		///<summary>
		///TのオブジェクトとUのオブジェクトが入れ替え可能か
		///</summary>
		template<class T, class U = T>
		struct Swappable :to_concept_ex<Swappable, detail::Swappable_c, T, U>
		{};


		///<summary>
		/// FromがToにキャストできるか
		///</summary>
		template< class From, class To >
		struct Convertible : meta_to_concept_ex<Convertible, std::is_convertible, From, To>
		{};

		///<summary>
		/// null許容か
		///</summary>
		template<class Type>
		struct NullablePointer :to_concept_ex<NullablePointer, detail::NullablePointer_c, Type>
		{};



		//--TODO is_invocableに変更したい--
		///<summary>
		///関数呼び出し可能な型か
		///</summary>
		template <class Func, class... Args>
		struct Invocable :to_concept_ex<Invocable, detail::Invocable_c, Func, Args...>
		{};
		///<summary>
		///関数オブジェクトか
		///</summary>
		template <class Type>
		struct FunctionObject : to_concept_ex<FunctionObject, detail::FunctionObject_c, Type>
		{};
		///<summary>
		///メタ関数か
		///</summary>
		template <class Type>
		struct MetaFunc : to_concept_ex<MetaFunc, detail::MetaFunc_c, Type>
		{};


		///<summary>
		///ハッシュ関数オブジェクトか
		///</summary>
		template <class Type, class Key>
		struct Hash : to_concept_ex< Hash, detail::Hash_c, Type, Key >
		{};

		///<summary>
		///期間、時刻、現在の時刻を取得可能か
		///</summary>
		template <class Type>
		struct Clock : to_concept_ex<Clock, detail::Clock_c, Type>
		{};


		//************************************************************************************************
		//
		//Layout
		//
		//************************************************************************************************
		///<summary>
		///トリビアルコピー可能か
		///</summary>
		template<class Type>
		struct TriviallyCopyable : meta_to_concept_ex<TriviallyCopyable, std::is_trivially_copyable, Type>
		{};
		///<summary>
		///トリビアル型か
		///</summary>
		template<class Type>
		struct TrivialType : meta_to_concept_ex<TrivialType, std::is_trivial, Type>
		{};
		///<summary>
		///標準レイアウト型か
		///</summary>
		template<class Type>
		struct StandardLayoutType : meta_to_concept_ex<StandardLayoutType, std::is_standard_layout, Type>
		{};
		///<summary>
		///POD型か
		///</summary>
		template<class Type>
		struct PODType : meta_to_concept_ex<PODType, std::is_pod, Type>
		{};

		//************************************************************************************************
		//
		//Iterator
		//
		//************************************************************************************************
		namespace detail
		{
			struct Iterator_c
			{
				template<class It>
				auto requires(It it, typename std::iterator_traits<It>::value_type v)->decltype(
					requires_extends<
					CopyConstructible<It>,
					CopyAssignable<It>,
					Destructible<It>,
					Swappable<It>,
					Indirectable<It>
					>,
					vailed_expr<It&>(++it)
					);
			};

			struct InputIterator_c
			{
				template<class It>
				auto requires(It&& it)->decltype(
					requires_extends_c(&Iterator_c::requires<It>),
					requires_extends<
					EqualityComparable<It>,
					NotEqualityComparable<It>
					>,
					tc::convertible_expr<typename std::iterator_traits<It>::value_type>(*it),
					tc::convertible_expr<typename std::iterator_traits<It>::value_type>(*it++),
					tc::vailed_expr<typename std::iterator_traits<It>::reference>(*it)
				);
			};

			struct OutputIterator_c
			{
				template<class It>
				auto requires(It&& it)->decltype(
					requires_extends_c(&Iterator_c::requires<It>),
					tc::convertible_expr<const It&>(it++),
					tc::vailed_expr<It&>(++it),
					*it = *it,
					*it++ = *it
				);
			};

			struct ForwardIterator_c
			{
				template<class It>
				auto requires(It&& it)->decltype(
					requires_extends_c(&InputIterator_c::requires<It>),
					requires_extends<
					DefaultConstructible<It>
					>,
					tc::vailed_expr<It>(it++),
					tc::vailed_expr<typename std::iterator_traits<It>::reference>(*it++)
				);
			};

			struct BidirectionalIterator_c
			{
				template<class It>
				auto requires(It&& it)->decltype(
					requires_extends_c(&ForwardIterator_c::requires<It>),
					tc::convertible_expr<const It&>(it--),
					tc::vailed_expr<It&>(--it),
					tc::vailed_expr<typename std::iterator_traits<It>::reference>(*it--)
				);
			};

			struct RandomAccessIterator_c
			{
				template<class It>
				auto requires(It&& it, typename std::iterator_traits<It>::difference_type n)->decltype(
					requires_extends_c(&BidirectionalIterator_c::requires<It>),
					requires_extends<
					LessThanComparable<It>, LessEqualComparable<It>,
					GreaterThanComparable<It>, GreaterEqualComparable<It>
					>,
					tc::vailed_expr<It&>(it += n),
					tc::vailed_expr<It>(it + n), tc::vailed_expr<It>(n + it),
					tc::vailed_expr<It&>(it -= n),
					tc::vailed_expr<It>(it - n),
					tc::convertible_expr<typename std::iterator_traits<It>::reference>(it[n])
					);
			};


			struct ValueSwappable_c
			{
				template<class It>
				auto requires(It it)->decltype(
					&Iterator_c::requires<It>,
					requires_extends<
					Swappable<typename std::iterator_traits<It>::value_type>
					>
				);
			};

			struct HasIterator_c
			{
				template<class Type>
				auto requires()->decltype(
					&Iterator_c::requires<typename Type::iterator>
				);
			};
		}//namespace detail


		///<summary>
		///イテレーターをもつか
		///</summary>
		template<class Type>
		struct HasIterator : to_concept_ex<HasIterator, detail::HasIterator_c, Type>
		{};

		///<summary>
		///イテレーターかどうか
		///</summary>
		template<class It>
		struct Iterator : to_concept_ex<Iterator, detail::Iterator_c, It>
		{};
		///<summary>
		///入力イテレーターかどうか
		///</summary>
		template<class It>
		struct InputIterator : to_concept_ex<InputIterator, detail::InputIterator_c, It>
		{};
		///<summary>
		///出力イテレーターかどうか
		///</summary>
		template<class It>
		struct OutputIterator : to_concept_ex<OutputIterator, detail::OutputIterator_c, It>
		{};

		///<summary>
		///前方イテレーターかどうか
		///</summary>
		template<class It>
		struct ForwardIterator : to_concept_ex<ForwardIterator, detail::ForwardIterator_c, It>
		{};
		///<summary>
		///双方向イテレーターかどうか
		///</summary>
		template<class It>
		struct BidirectionalIterator : to_concept_ex<BidirectionalIterator, detail::BidirectionalIterator_c, It>
		{};
		///<summary>
		///ランダムアクセスイテレーターかどうか
		///</summary>
		template<class It>
		struct RandomAccessIterator : to_concept_ex<RandomAccessIterator, detail::RandomAccessIterator_c, It>
		{};

	
		///<summary>
		///イテレーターの値型がスワップ可能か
		///</summary>
		template<class It>
		struct ValueSwappable : to_concept_ex<ValueSwappable, detail::ValueSwappable_c, It>
		{};
		/*
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

		*/


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


		namespace detail
		{
			template<template <class...>class Template, class Type>
			struct is_template : std::false_type
			{};
			template<template <class...>class Template, class... Args>
			struct is_template<Template, Template<Args...>> : std::true_type
			{};
		}

		///<summary>
		///TypeがTemplate型であるか
		///</summary>
		template<template <class...>class Template, class Type>
		struct IsTemplate : detail::is_template<Template,
			tc::detail::as_mapped_if_t<IsTemplate<Template, tc::detail::remove_mapped_t<Type>>, Type>
		>
		{};

	}//namespace Concept


}//namespace tc


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
#define TC_CONCEPT_ASSERT( ... ) static_assert(tc::And<__VA_ARGS__>::value,#__VA_ARGS__ )

 ///<summary>
 ///条件をみたさないとアサート(bool)
 ///</summary>
#define TC_CONCEPT_ASSERT_BOOL( ... ) static_assert(static_cast<bool>(__VA_ARGS__),#__VA_ARGS__ )

