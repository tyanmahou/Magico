#pragma once
#include"../concept.hpp"
#include<functional>

namespace magico {
	namespace concepts
	{

		//************************************************************************************************
		//
		//Basic
		//
		//************************************************************************************************

		///<summary>
		///<para>operator =(copy) をもつか</para>
		///<para>[ To,From = To ]</para>
		///</summary>
		MAGICO_CONCEPT(CopyAssignable)
		{
			template<class To, class From = To>
			auto require(To& t, const From& f)->decltype(
				magico::extends<std::is_assignable>::require(t, f)
				);
		};

		///<summary>
		///<para>operator =(move) をもつか</para>
		///<para>[ To,From = To ]</para>
		///</summary>
		MAGICO_CONCEPT(MoveAssignable)
		{
			template<class To, class From = To>
			auto require(To t, From f)->decltype(
				magico::extends<std::is_assignable>::require<
				std::add_lvalue_reference_t<To>,
				std::add_rvalue_reference_t<From>
				>()
				);
		};

		///<summary>
		///<para>Type( Args... ) の形式のコンストラクタ呼び出しが可能か</para>
		///<para>[ Type,Args... ]</para>
		///</summary>
		MAGICO_CONCEPT(Constructible)
		{
			template<class Type, class... Args>
			auto require()->decltype(
				magico::extends<std::is_constructible>::require<Type, Args...>()
				);
		};

		///<summary>
		///<para>デフォルトコンストラクタ をもつか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(DefaultConstructible)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_default_constructible>::require<Type>()
				);
		};
		///<summary>
		///<para>コピーコンストラクタ をもつか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(CopyConstructible)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_copy_constructible>::require<Type>()
				);
		};
		///<summary>
		///<para>ムーブコンストラクタ をもつか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(MoveConstructible)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_move_constructible>::require<Type>()
				);
		};
		///<summary>
		///<para>デストラクタ をもつか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Destructible)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_destructible>::require<Type>()
				);
		};
		///<summary>
		///<para>仮想デストラクタ をもつか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(HasVirtualDestructor)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::has_virtual_destructor>::require<Type>()
				);
		};

		///<summary>
		///<para>TとUが同じか</para>
		///<para>[ T,U ]</para>
		///</summary>
		MAGICO_CONCEPT(IsSame)
		{
			template<class T, class U>
			auto require()->decltype(
				magico::extends<std::is_same>::require<T, U>()
				);
		};

		///<summary>
		///<para>TypeがBase,もしくはBaseを継承しているか</para>
		///<para>[ Type,Base ]</para>
		///</summary>
		MAGICO_CONCEPT(Extended)
		{
			template<class Type, class Base>
			auto require()->decltype(
				magico::extends<std::is_base_of>::require<Base, Type>()
				);
		};

		///<summary>
		///<para>スカラーかどうか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Scalar)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_scalar>::require<Type>()
				);
		};

		///<summary>
		///<para>仮想クラスかどうか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Abstract)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_abstract>::require<Type>()
				);
		};

		///<summary>
		///<para>enum型か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Enum)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_enum>::require<Type>()
				);
		};

		///<summary>
		///<para>class(struct)型か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Class)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_class>::require<Type>()
				);
		};

		///<summary>
		///<para>union型か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Union)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_union>::require<Type>()
				);
		};

		///<summary>
		///<para>関数型t(...)か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Function)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_function>::require<Type>()
				);
		};

		//************************************************************************************************
		//
		//Library-wide
		//
		//************************************************************************************************

		///<summary>
		///<para>小なり比較演算子で大小関係にあるか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(LessThanComparable)
		{
			template<class Type>
			auto require(Type a, Type b)->decltype(
				magico::valid_expr<bool>(a < b)
				);
		};

		///<summary>
		///<para>==演算子で等価関係にあるか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(EqualityComparable)
		{
			template<class Type>
			auto require(Type a, Type b)->decltype(
				magico::valid_expr<bool>(a == b)
				);
		};

		///<summary>
		///<para>アロケーターか</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Allocator)
		{
		private:
			size_t n;
			using ex = magico::extends<EqualityComparable, CopyConstructible, MoveConstructible>;
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
		///<para>TのオブジェクトとUのオブジェクトが入れ替え可能か</para>
		///<para>[ T,U=T ]</para>
		///</summary>
		MAGICO_CONCEPT(Swappable)
		{
			template<class T, class U = T>
			auto require(T&& a, U&& b)->decltype(
				std::swap(a, b),
				std::swap(b, a)
				);

		};

		///<summary>
		///<para> FromがToにキャストできるか</para>
		///<para>[ From,To ]</para>
		///</summary>
		MAGICO_CONCEPT(Convertible)
		{
			template< class From, class To >
			auto require()->decltype(
				magico::extends<std::is_convertible>::require<From, To>()
				);
		};

		///<summary>
		///<para> null許容か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(NullablePointer)
		{
		private:
			using ex = magico::extends<
				EqualityComparable,
				DefaultConstructible,
				CopyConstructible,
				CopyAssignable,
				Destructible
			>;
		public:
			template<class Type>
			auto require(Type&& null)->decltype(
				ex::require<Type>(),
				magico::extends<Constructible>::require<Type, std::nullptr_t>(),
				magico::valid_expr< bool >(null != null),
				magico::valid_expr< bool >(null = nullptr),
				magico::valid_expr< bool >(null == nullptr),
				magico::valid_expr< bool >(nullptr == null),
				magico::valid_expr< bool >(null != nullptr),
				magico::valid_expr< bool >(nullptr != null)
				);

		};

		///<summary>
		///<para>関数呼び出し可能な型か</para>
		///<para>[ F,Args... ]</para>
		///</summary>
		MAGICO_CONCEPT(Invocable)
		{
			template <class F, class... Args>
			auto require(F&& func, Args&&... args)->decltype(
				std::invoke(func, args...)
				);

		};

		///<summary>
		///<para>関数オブジェクトか</para>
		///<para>[ Type,Args... ]</para>
		///</summary>
		MAGICO_CONCEPT(FunctionObject)
		{
			template <class Type, class... Args>
			auto require()->decltype(
				magico::extends<Invocable>::require<Type, Args...>(),
				magico::extends<std::is_object>::require<Type>()
				);
		};
		///<summary>
		///<para>bool型になる関数オブジェクトか</para>
		///<para>[ Type,Args... ]</para>
		///</summary>
		MAGICO_CONCEPT(Predicate)
		{
			template <class Type, class... Args>
			auto require(Type t, Args&&... args)->decltype(
				magico::extends<FunctionObject>::require<Type, Args...>(),
				magico::valid_expr<bool>(t(args...))
				);
		};

		///<summary>
		///<para>bool型になる2引数関数オブジェクトか</para>
		///<para>[ Type,T,U=T ]</para>
		///</summary>
		MAGICO_CONCEPT(BinaryPredicate)
		{
			template <class Type, class T, class U = T>
			auto require(Type t, T a, U b)->decltype(
				magico::extends<CopyConstructible>::require<Type>(),
				magico::extends<Predicate>::require<Type, T, U>()
				);
		};

		///<summary>
		///<para>比較関数オブジェクトか</para>
		///<para>[ Type,T,U=T ]</para>
		///</summary>
		MAGICO_CONCEPT(Compare)
		{
			template <class Type, class T, class U = T>
			auto require(Type t, T a, U b)->decltype(
				magico::extends<BinaryPredicate>::require<Type, T, U>(),
				magico::valid_expr<bool>(!t(a, b) && !t(b, a))
				);
		};
		///<summary>
		///<para>メタ関数か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(MetaFunc)
		{
			template <class Type>
			auto require()->decltype(
				valid_expr<const bool&>(Type::value)
				);
		};

		///<summary>
		///<para>ハッシュ関数オブジェクトか</para>
		///<para>[ Type,Key ]</para>
		///</summary>
		MAGICO_CONCEPT(Hash)
		{
		private:
			using ex = magico::extends<CopyConstructible, Destructible>;
		public:
			template <class Type, class Key>
			auto require(Type&& t, Key&& key)->decltype(
				ex::require<Type>(),
				valid_expr<size_t>(t(key))
				);

		};

		///<summary>
		///<para>期間、時刻、現在の時刻を取得可能か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(Clock)
		{
			template <class Type>
			auto require(Type&& t)->decltype(
				magico::associated_type<typename Type::rep>(),
				magico::associated_type<typename Type::period>(),
				magico::associated_type<typename Type::duration>(),
				magico::associated_type<typename Type::time_point>(),
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
		///<para>トリビアルコピー可能か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(TriviallyCopyable)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_trivially_copyable>::require<Type>()
				);
		};

		///<summary>
		///<para>トリビアル型か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(TrivialType)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_trivial>::require<Type>()
				);
		};
		///<summary>
		///<para>標準レイアウト型か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(StandardLayoutType)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_standard_layout>::require<Type>()
				);
		};

		///<summary>
		///<para>POD型か</para>
		///<para>[ Type ]</para>
		///</summary>
		MAGICO_CONCEPT(PODType)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_pod>::require<Type>()
				);
		};
	}
}
