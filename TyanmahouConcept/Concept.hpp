#pragma once
#include<type_traits>
#include<tuple>
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
	///���̃C���X�^���X�쐬(declval�̃w���p)
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
	///WhereConcept�������𖞂�����
	///</summary>
	template < class WhereConcept >
	using requires = std::enable_if_t<WhereConcept::value, std::nullptr_t >;

	///<summary>
	///Test��ture���ǂ���
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
	 ///�C���X�^���X�ɃR���Z�v�g�}�b�v��K��������
	 ///</summary>
	template<class Concept, class Type>
	auto concept_mapping(Type&& value)->decltype(detail::concept_mapping_impl<Concept, Type>() = value)
	{

		return detail::concept_mapping_impl<Concept, Type>() = value;
	}
	///<summary>
	///�C���X�^���X�ɃR���Z�v�g�}�b�v��K�������� �Ԃ�ltuple
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
	///�R���Z�v�g�}�b�v��̌^
	///</summary>
	template<class Concept, class Arg>
	using concept_mapped_t = typename detail::concept_mapped<Concept, Arg>::type;


	///<summary>
	///�R���Z�v�g�}�b�v��K�p��̌^�Ƃ��Ĕ���
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
	///Type��as_mapped�������ꍇ�}�b�s���O��̌^��
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
	namespace detail
	{
		template<class Concept, class ... Args>
		using ConceptCheck = decltype(&Concept::template requires<Args...>);

	}

	///<summary>
	///����
	///</summary>
	template<class ...Args>
	using constraint = void_t<Args...>;


	///<summary>
	///requires�����N���X���R���Z�v�g(���^�֐�)�ɕϊ�
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
	///alias�`��constraint���R���Z�v�g(���^�֐�)�ɕϊ�
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
	///�ʏ�̃��^�֐����R���Z�v�g(���^�֐�)�ɕϊ�
	///</summary>
	template<template<class...>class Meta, class ...Args>
	struct meta_to_concept : Meta<
		detail::as_mapped_if_t<
		meta_to_concept<Meta, detail::remove_mapped_t<Args>...>,
		Args>...
	>
	{};

	///<summary>
	///Type�^�����݂��邩
	///</summary>
	template<class Type>
	Type&& associated_type = val<Type>;

	///<summary>
	///�����]���\��Ret�^��
	///</summary>
	///<param name= "exp">
	///�]�����鎮
	///</param>
	template<class Ret, class Exp>
	auto vailed_expr(Exp&& exp)->tc::requires<std::is_same<Ret, Exp>>;

	///<summary>
	///����̌p��
	///</summary>
	template<class... Concept>
	tc::void_t<tc::requires<Concept>...>* requires_extends = nullptr;

	///<summary>
	///Concept���p������Ƃ��Ɏd�l
	///</summary>
	template<template<class...>class Sub, class Constraint, class... Args>
	using concept_extends = to_concept<Constraint, tc::detail::as_mapped_if_t<Sub<typename tc::detail::remove_mapped_t<Args>...>, Args>... >;

	template<template<class...>class Sub, template<class...>class Super, class ...Args>
	using concept_extends_meta = Super<as_mapped_if_t<Args, Sub, Args...>...>;

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
		//����
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
		//�Q��
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
		//const�Q��
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

		//���b�v�֐�
		template<class T>
		T any_cast_wrapper(std::any& _any)
		{
			return AnyCastImpl<T>::Func(_any);
		}

	}
	///<summary>
	///�R���Z�v�g�𖞂����^
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
		//���Z�q
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
struct className : tc::concept_extends<className,detail::className##_c,Type>{}


		///<summary>
		/// operator ! ������
		///</summary>
		TC_HAS_UNARY_OPERATOR(Negatable, !);


		///<summary>
		/// �P��operator + ������
		///</summary>
		TC_HAS_UNARY_OPERATOR(UnaryPlusable, +);

		///<summary>
		/// �P��operator - ������
		///</summary>
		TC_HAS_UNARY_OPERATOR(UnaryMinusable, -);

		///<summary>
		/// �O�u�C���N�������g�\��
		///</summary>
		TC_HAS_UNARY_OPERATOR(PreIncrementable, ++);

		///<summary>
		/// �O�u�f�N�������g�\��
		///</summary>
		TC_HAS_UNARY_OPERATOR(PreDecrementable, --);

		///<summary>
		/// ��u�C���N�������g�\��
		///</summary>
		template<class Type>
		struct PostIncrementable :to_concept<detail::PostIncrementable_c, tc::as_mapped_if_t<Type, PostIncrementable, Type>> {};

		///<summary>
		/// ��u�f�N�������g�\��
		///</summary>
		template<class Type>
		struct PostDecrementable : to_concept<detail::PostDecrementable_c, tc::as_mapped_if_t<Type, PostDecrementable, Type>> {};

		///<summary>
		/// �P��operator ~ ������
		///</summary>
		TC_HAS_UNARY_OPERATOR(Complementable, ~);

		///<summary>
		/// �A�h���X�擾�\��
		///</summary>
		TC_HAS_UNARY_OPERATOR(AddressObtainable, &);

		///<summary>
		/// �֐ډ��Z�\��
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
struct className:tc::concept_extends<className,detail::className##_c,Left,Right>{}

		///<summary>
		/// ���Z�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(Plusable, +);

		///<summary>
		/// ���Z�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(Minusable, -);

		///<summary>
		/// ��Z�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(Multipliable, *);

		///<summary>
		/// ���Z�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(Dividable, / );

		///<summary>
		/// ��]�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(Modulable, %);

		///<summary>
		/// ���Z����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(PlusAssignable, +=);

		///<summary>
		/// ���Z����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(MinusAssignable, -=);

		///<summary>
		/// ��Z����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(MultiplyAssignable, *=);

		///<summary>
		/// ���Z����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(DivideAssignable, /=);

		///<summary>
		/// ��]����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(ModulusAssignable, %=);

		///<summary>
		/// ���V�t�g�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(LeftShiftable, << );

		///<summary>
		/// �E�V�t�g�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(RightShiftable, >> );

		///<summary>
		/// ���V�t�g����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(LeftShiftAssignable, <<=);

		///<summary>
		/// �E�V�t�g�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(RightShifAssignable, >>=);

		///<summary>
		/// �r�b�g���Ƃ�AND���Z�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitAndPossible, &);

		///<summary>
		/// �r�b�g���Ƃ�OR���Z�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitOrPossible, | );

		///<summary>
		/// �r�b�g���Ƃ̔r���IOR���Z�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitXorPossible, ^);

		///<summary>
		/// �r�b�g���Ƃ�AND���Z����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitAndAssignable, &=);

		///<summary>
		/// �r�b�g���Ƃ�OR���Z����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitOrAssignable, |=);

		///<summary>
		/// �r�b�g���Ƃ̔r���IOR���Z����\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(BitXorAssignable, ^=);

		///<summary>
		/// �_���ω\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(LogicalAndPossible, &&);

		///<summary>
		/// �_���a�\��
		///</summary>
		TC_HAS_BINARY_OPERATOR(LogicalOrPossible, || );

		///<summary>
		/// ���Ȃ��r���Z ������
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessThanComparable, < );

		///<summary>
		/// ���Ȃ�=��r���Z ������
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessEqualComparable, <= );

		///<summary>
		/// ��Ȃ��r���Z������
		///</summary>
		TC_HAS_BINARY_OPERATOR(GreaterThanComparable, > );

		///<summary>
		/// ��Ȃ�=��r���Z������
		///</summary>
		TC_HAS_BINARY_OPERATOR(GreaterEqualComparable, >= );

		///<summary>
		/// operator == ������
		///</summary>
		TC_HAS_BINARY_OPERATOR(EqualityComparable, == );

		///<summary>
		/// operator != ������
		///</summary>
		TC_HAS_BINARY_OPERATOR(NotEqualityComparable, != );

#undef TC_HAS_BINARY_OPERATOR

		//************************************************************************************************
		//
		//��{�I�ȃR���Z�v�g
		//
		//************************************************************************************************

		///<summary>
		/// operator =(copy) ������
		///</summary>
		template< class To, class From = To>
		struct CopyAssignable : std::is_assignable<
			std::add_lvalue_reference_t<as_mapped_if_t<To, CopyAssignable, To, From>>,
			std::add_lvalue_reference_t<std::add_const_t<as_mapped_if_t<From, CopyAssignable, To, From>>>
		> {};

		///<summary>
		/// operator =(move) ������
		///</summary>
		template< class To, class From = To>
		struct MoveAssignable : std::is_assignable<
			std::add_lvalue_reference_t<as_mapped_if_t<To, MoveAssignable, To, From>>,
			std::add_rvalue_reference_t<as_mapped_if_t<From, MoveAssignable, To, From>>
		> {};

		///<summary>
		/// Type( Args... ) �̌`���̃R���X�g���N�^�Ăяo�����\��
		///</summary>
		template< class Type, class... Args>
		struct Constructible : std::is_constructible<
			as_mapped_if_t<Type, Constructible, Type, Args...>,
			as_mapped_if_t<Args, Constructible, Type, Args...>...
		> {};

		///<summary>
		/// �f�t�H���g�R���X�g���N�^ ������
		///</summary>
		template< class Type >
		struct DefaultConstructible : concept_extends_meta <DefaultConstructible, std::is_default_constructible, Type> {};

		///<summary>
		/// �R�s�[�R���X�g���N�^ ������
		///</summary>
		template<class Type>
		struct CopyConstructible : concept_extends_meta <CopyConstructible, std::is_default_constructible, Type> {};

		///<summary>
		/// ���[�u�R���X�g���N�^ ������
		///</summary>
		template< class Type>
		struct MoveConstructible : concept_extends_meta <MoveConstructible, std::is_move_constructible, Type> {};

		///<summary>
		/// �f�X�g���N�^ ������
		///</summary>
		template< class Type>
		struct Destructible : concept_extends_meta <Destructible, std::is_destructible, Type> {};

		///<summary>
		/// ���z�f�X�g���N�^ ������
		///</summary>
		template< class Type>
		struct  HasVirtualDestructor :tc::concept_extends_meta <HasVirtualDestructor, std::has_virtual_destructor, Type> {};

		///<summary>
		///T��U��������
		///</summary>
		template<class T, class U>
		struct  IsSame :tc::concept_extends_meta <IsSame, std::is_same, T, U> {};

		///<summary>
		///Type��Base,��������Base���p�����Ă��邩
		///</summary>
		template<class Type, class Base>
		struct Extended :std::is_base_of<
			as_mapped_if_t<Base, Extended, Type, Base>,
			as_mapped_if_t<Type, Extended, Type, Base>
		> {};

		///<summary>
		///�X�J���[���ǂ���
		///</summary>
		template<class Type>
		struct Scalar : tc::concept_extends_meta <Scalar, std::is_scalar, Type> {};

		///<summary>
		///���z�N���X���ǂ���
		///</summary>
		template<class Type>
		struct Abstract : tc::concept_extends_meta <Abstract, std::is_abstract, Type> {};

		///<summary>
		///enum�^��
		///</summary>
		template<class Type>
		struct Enum : tc::concept_extends_meta <Enum, std::is_enum, Type> {};

		///<summary>
		///class(struct)�^��
		///</summary>
		template<class Type>
		struct Class : tc::concept_extends_meta <Class, std::is_class, Type> {};

		///<summary>
		///union�^��
		///</summary>
		template<class Type>
		struct Union : tc::concept_extends_meta <Union, std::is_union, Type> {};

		///<summary>
		///�֐��^t(...)��
		///</summary>
		template<class Type>
		struct Function : tc::concept_extends_meta <Function, std::is_function, Type> {};


		//************************************************************************************************
		//
		//�^�̓���
		//
		//************************************************************************************************

		namespace detail
		{
			struct Allocator_c
			{
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

				size_t n;
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
					tc::associated_type<Type::rep>,
					tc::associated_type<Type::period>,
					tc::associated_type<Type::duration>,
					tc::associated_type<Type::time_point>,
					vailed_expr<const bool&>(t.is_steady),
					vailed_expr<Type::time_point>(t.now())
					);
			};
		}//namespace detail

		 ///<summary>
		 ///�A���P�[�^�[��
		 ///</summary>
		template<class Type>
		struct Allocator :tc::concept_extends<Allocator, detail::Allocator_c, Type>
		{};


		///<summary>
		///T�̃I�u�W�F�N�g��U�̃I�u�W�F�N�g������ւ��\��
		///</summary>
		template<class T, class U = T>
		struct Swappable :tc::concept_extends<Swappable, detail::Swappable_c, T, U>
		{};


		///<summary>
		/// From��To�ɃL���X�g�ł��邩
		///</summary>
		template< class From, class To >
		struct Convertible : tc::concept_extends_meta<Convertible, std::is_convertible, From, To>
		{};

		///<summary>
		/// null���e��
		///</summary>
		template<class Type>
		struct NullablePointer :tc::concept_extends<NullablePointer, detail::NullablePointer_c, Type>
		{};



		//--TODO is_invocable�ɕύX������--
		///<summary>
		///�֐��Ăяo���\�Ȍ^��
		///</summary>
		template <class Func, class... Args>
		struct Invocable :tc::concept_extends<Invocable, detail::Invocable_c, Func, Args...>
		{};
		///<summary>
		///�֐��I�u�W�F�N�g��
		///</summary>
		template <class Type>
		struct FunctionObject : tc::concept_extends<FunctionObject, detail::FunctionObject_c, Type>
		{};
		///<summary>
		///���^�֐���
		///</summary>
		template <class Type>
		struct MetaFunc : tc::concept_extends<MetaFunc, detail::MetaFunc_c, Type>
		{};

	
		///<summary>
		///�n�b�V���֐��I�u�W�F�N�g��
		///</summary>
		template <class Type, class Key>
		struct Hash : tc::concept_extends< Hash, detail::Hash_c, Type, Key >
		{};

		///<summary>
		///���ԁA�����A���݂̎������擾�\��
		///</summary>
		template <class Type>
		struct Clock : tc::concept_extends<Clock, detail::Clock_c, Type>
		{};

		/*
		//************************************************************************************************
		//
		//���C�A�E�g
		//
		//************************************************************************************************
		///<summary>
		///�g���r�A���R�s�[�\��
		///</summary>
		template<class Type>
		struct TriviallyCopyable : tc::detail::to_concept<TriviallyCopyable, std::is_trivially_copyable, Type>
		{};
		///<summary>
		///�g���r�A���^��
		///</summary>
		template<class Type>
		struct TrivialType : tc::detail::to_concept<TrivialType, std::is_trivial, Type>
		{};
		///<summary>
		///�W�����C�A�E�g�^��
		///</summary>
		template<class Type>
		struct StandardLayoutType : tc::detail::to_concept<StandardLayoutType, std::is_standard_layout, Type>
		{};
		///<summary>
		///POD�^��
		///</summary>
		template<class Type>
		struct PODType : tc::detail::to_concept<PODType, std::is_pod, Type>
		{};
		//************************************************************************************************
		//
		//�C�e���[�^�[
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
		///�C�e���[�^�[������
		///</summary>
		TC_HAS_MEMBER_TYPE(HasIterator, iterator);
		///<summary>
		///�C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		TC_TO_CONCEPT(Iterator, detail::Iterator_c, It);
		///<summary>
		///���̓C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		TC_TO_CONCEPT(InputIterator, detail::InputIterator_c, It);
		///<summary>
		///�o�̓C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		TC_TO_CONCEPT(OutputIterator, detail::OutputIterator_c, It);
		///<summary>
		///�O���C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		TC_TO_CONCEPT(ForwardIterator, detail::ForwardIterator_c, It);
		///<summary>
		///�o�����C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		TC_TO_CONCEPT(BidirectionalIterator, detail::BidirectionalIterator_c, It);
		///<summary>
		///�����_���A�N�Z�X�C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		TC_TO_CONCEPT(RandomAccessIterator, detail::RandomAccessIterator_c, It);
		///<summary>
		///�C�e���[�^�[�̒l�^���X���b�v�\��
		///</summary>
		template<class It>
		TC_TO_CONCEPT(ValueSwappable, detail::ValueSwappable_c, It);
		//************************************************************************************************
		//
		//�R���e�i
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
		///�R���e�i���ǂ���
		///</summary>
		template<class X>
		TC_TO_CONCEPT(Container, detail::Container_c, X);
		///<summary>
		///�O���C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X>
		TC_TO_CONCEPT(ForwardContainer, detail::ForwardContainer_c, X);
		///<summary>
		///�����_���A�N�Z�X�C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X>
		TC_TO_CONCEPT(RandomAccessContainer, detail::RandomAccessContainer_c, X);
		///<summary>
		///���o�[�X�C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X>
		TC_TO_CONCEPT(ReversibleContainer, detail::ReversibleContainer_c, X);
		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^���f�t�H���g�ő}���\��
		///</summary>
		template<class X>
		TC_TO_CONCEPT(DefaultInsertable, detail::DefaultInsertable_c, X);
		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^�̃R�s�[�}���\��
		///</summary>
		template<class X>
		TC_TO_CONCEPT(CopyInsertable, detail::CopyInsertable_c, X);
		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^�̉E�Ӓl�I�u�W�F�N�g�����[�u�}���\��
		///</summary>
		template<class X>
		TC_TO_CONCEPT(MoveInsertable, detail::MoveInsertable_c, X);
		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA�v�f�^�̃R���X�g���N�^������Args���璼�ڍ\�z�\��
		///</summary>
		template <class X, class... Args>
		TC_TO_CONCEPT(EmplaceConstructible, detail::EmplaceConstructible_c, X, Args...);
		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA�v�f�^�̔j�����\��
		///</summary>
		template<class X>
		TC_TO_CONCEPT(Erasable, detail::Erasable_c, X);
		///<summary>
		///�A���P�[�^�[��F������R���e�i��
		///</summary>
		template<class X>
		TC_TO_CONCEPT(AllocatorAwareContainer, detail::AllocatorAwareContainer_c, X);

		*/


		//************************************************************************************************
		//
		//���̑�
		//
		//************************************************************************************************
		///<summary>
		///������conditional���݂����� concept_map�s��
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
		///Type��Template�^�ł��邩
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
 ///where�}�N��
 ///</summary>
#define TC_WHERE( ... ) tc::requires< __VA_ARGS__ > = nullptr

 ///<summary>
 ///where�}�N��(bool)
 ///</summary>
#define TC_WHERE_BOOL( ... ) tc::requires_bool< static_cast<bool>(__VA_ARGS__) > = nullptr

 ///<summary>
 ///�������݂����Ȃ��ƃA�T�[�g
 ///</summary>
#define TC_CONCEPT_ASSERT( ... ) static_assert(__VA_ARGS__::value,#__VA_ARGS__ )

 ///<summary>
 ///�������݂����Ȃ��ƃA�T�[�g(bool)
 ///</summary>
#define TC_CONCEPT_ASSERT_BOOL( ... ) static_assert(static_cast<bool>(__VA_ARGS__),#__VA_ARGS__ )