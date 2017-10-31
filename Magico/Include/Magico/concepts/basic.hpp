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
		/// operator =(copy) をもつか
		///</summary>
		template< class To, class From = To> MAGICO_CONCEPT(CopyAssignable, To, From)
		{
			template<class To, class From>
			auto require(To& t, const From& f)->decltype(
				magico::extends<std::is_assignable>::require(t, f)
				);
		};

		///<summary>
		/// operator =(move) をもつか
		///</summary>
		template< class To, class From = To> MAGICO_CONCEPT(MoveAssignable, To, From)
		{
			template<class To, class From>
			auto require(To t, From f)->decltype(
				magico::extends<std::is_assignable>::require<
				std::add_lvalue_reference_t<To>,
				std::add_rvalue_reference_t<From>
				>()
				);
		};

		///<summary>
		/// Type( Args... ) の形式のコンストラクタ呼び出しが可能か
		///</summary>
		template< class Type, class... Args> MAGICO_CONCEPT(Constructible, Type, Args...)
		{
			template<class Type, class... Args>
			auto require()->decltype(
				magico::extends<std::is_constructible>::require<Type, Args...>()
				);
		};

		///<summary>
		/// デフォルトコンストラクタ をもつか
		///</summary>
		template< class Type > MAGICO_CONCEPT(Defaulmagicoonstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_default_constructible>::require<Type>()
				);
		};
		///<summary>
		/// コピーコンストラクタ をもつか
		///</summary>
		template< class Type > MAGICO_CONCEPT(CopyConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_copy_constructible>::require<Type>()
				);
		};
		///<summary>
		/// ムーブコンストラクタ をもつか
		///</summary>
		template< class Type > MAGICO_CONCEPT(MoveConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_move_constructible>::require<Type>()
				);
		};
		///<summary>
		/// デストラクタ をもつか
		///</summary>
		template< class Type > MAGICO_CONCEPT(Destructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_destructible>::require<Type>()
				);
		};
		///<summary>
		/// 仮想デストラクタ をもつか
		///</summary>
		template< class Type > MAGICO_CONCEPT(HasVirtualDestructor, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::has_virtual_destructor>::require<Type>()
				);
		};

		///<summary>
		///TとUが同じか
		///</summary>
		template<class T, class U> MAGICO_CONCEPT(IsSame, T, U)
		{
			template<class T, class U>
			auto require()->decltype(
				magico::extends<std::is_same>::require<T, U>()
				);
		};

		///<summary>
		///TypeがBase,もしくはBaseを継承しているか
		///</summary>
		template<class Type, class Base> MAGICO_CONCEPT(Extended, Type, Base)
		{
			template<class Type, class Base>
			auto require()->decltype(
				magico::extends<std::is_base_of>::require<Base, Type>()
				);
		};

		///<summary>
		///スカラーかどうか
		///</summary>
		template< class Type > MAGICO_CONCEPT(Scalar, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_scalar>::require<Type>()
				);
		};

		///<summary>
		///仮想クラスかどうか
		///</summary>
		template< class Type > MAGICO_CONCEPT(Abstract, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_abstract>::require<Type>()
				);
		};

		///<summary>
		///enum型か
		///</summary>
		template< class Type > MAGICO_CONCEPT(Enum, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_enum>::require<Type>()
				);
		};

		///<summary>
		///class(struct)型か
		///</summary>
		template< class Type > MAGICO_CONCEPT(Class, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_class>::require<Type>()
				);
		};

		///<summary>
		///union型か
		///</summary>
		template< class Type > MAGICO_CONCEPT(Union, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_union>::require<Type>()
				);
		};

		///<summary>
		///関数型t(...)か
		///</summary>
		template< class Type > MAGICO_CONCEPT(Function, Type)
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
		///　<　演算子で大小関係にあるか
		///</summary>
		template<class Type> MAGICO_CONCEPT(LessThanComparable, Type)
		{
			template<class Type>
			auto require(Type a, Type b)->decltype(
				magico::valid_expr<bool>(a < b)
				);
		};

		///<summary>
		///　==演算子で等価関係にあるか
		///</summary>
		template<class Type> MAGICO_CONCEPT(EqualityComparable, Type)
		{
			template<class Type>
			auto require(Type a, Type b)->decltype(
				magico::valid_expr<bool>(a == b)
				);
		};

		///<summary>
		///アロケーターか
		///</summary>
		template<class Type> MAGICO_CONCEPT(Allocator, Type)
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
		///TのオブジェクトとUのオブジェクトが入れ替え可能か
		///</summary>
		template<class T, class U = T> MAGICO_CONCEPT(Swappable, T, U)
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
		template< class From, class To > MAGICO_CONCEPT(Convertible, From, To)
		{
			template< class From, class To >
			auto require()->decltype(
				magico::extends<std::is_convertible>::require<From, To>()
				);
		};

		///<summary>
		/// null許容か
		///</summary>
		template<class Type> MAGICO_CONCEPT(NullablePointer, Type)
		{
		private:
			using ex = magico::extends<
				EqualityComparable,
				Defaulmagicoonstructible,
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
		///関数呼び出し可能な型か
		///</summary>
		template <class F, class... Args>MAGICO_CONCEPT(Invocable, F, Args...)
		{
			template <class F, class... Args>
			auto require(F&& func, Args&&... args)->decltype(
				std::invoke(func, args...)
				);

		};

		///<summary>
		///関数オブジェクトか
		///</summary>
		template <class Type, class... Args> MAGICO_CONCEPT(FunctionObject, Type, Args...)
		{
			template <class Type, class... Args>
			auto require()->decltype(
				magico::extends<Invocable>::require<Type, Args...>(),
				magico::extends<std::is_object>::require<Type>()
				);
		};
		///<summary>
		///bool型になる関数オブジェクトか
		///</summary>
		template <class Type, class... Args> MAGICO_CONCEPT(Predicate, Type, Args...)
		{
			template <class Type, class... Args>
			auto require(Type t, Args&&... args)->decltype(
				magico::extends<FunctionObject>::require<Type, Args...>(),
				magico::valid_expr<bool>(t(args...))
				);
		};

		///<summary>
		///bool型になる関数オブジェクトか
		///</summary>
		template <class Type, class T, class U = T> MAGICO_CONCEPT(BinaryPredicate, Type, T, U)
		{
			template <class Type, class T, class U>
			auto require(Type t, T a, U b)->decltype(
				magico::extends<CopyConstructible>::require<Type>(),
				magico::extends<Predicate>::require<Type, T, U>()
				);
		};

		///<summary>
		///比較関数オブジェクトか
		///</summary>
		template <class Type, class T, class U = T> MAGICO_CONCEPT(Compare, Type, T, U)
		{
			template <class Type, class T, class U>
			auto require(Type t, T a, U b)->decltype(
				magico::extends<BinaryPredicate>::require<Type, T, U>(),
				magico::valid_expr<bool>(!t(a, b) && !t(b, a))
				);
		};
		///<summary>
		///メタ関数か
		///</summary>
		template <class Type> MAGICO_CONCEPT(MetaFunc, Type)
		{
			template <class Type>
			auto require()->decltype(
				valid_expr<const bool&>(Type::value)
				);
		};

		///<summary>
		///ハッシュ関数オブジェクトか
		///</summary>
		template <class Type, class Key> MAGICO_CONCEPT(Hash, Type, Key)
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
		///期間、時刻、現在の時刻を取得可能か
		///</summary>
		template <class Type> MAGICO_CONCEPT(Clock, Type)
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
		///トリビアルコピー可能か
		///</summary>
		template< class Type > MAGICO_CONCEPT(TriviallyCopyable, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_trivially_copyable>::require<Type>()
				);
		};

		///<summary>
		///トリビアル型か
		///</summary>
		template< class Type > MAGICO_CONCEPT(TrivialType, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_trivial>::require<Type>()
				);
		};
		///<summary>
		///標準レイアウト型か
		///</summary>
		template<class Type>MAGICO_CONCEPT(StandardLayoutType, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_standard_layout>::require<Type>()
				);
		};

		///<summary>
		///POD型か
		///</summary>
		template<class Type>MAGICO_CONCEPT(PODType, Type)
		{
			template<class Type>
			auto require()->decltype(
				magico::extends<std::is_pod>::require<Type>()
				);
		};
	}
}
