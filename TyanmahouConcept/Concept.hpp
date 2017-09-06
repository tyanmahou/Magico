#pragma once
#include<type_traits>
#include<tuple>

///<summary>
///�R���Z�v�g�ɕϊ�
///</summary>
#define TC_TO_CONCEPT(className,constraint,...)\
struct className : tc::detail::to_concept< className ,typename tc::to_meta_func<constraint>::template type,__VA_ARGS__>\
{}

///<summary>
///memName�̃����o�������ǂ���
///</summary>
#define TC_HAS_MEMBER(className,memName)\
namespace detail\
{\
template<class Type>\
using className##_c = tc::constraint<decltype(&Type::memName)>;\
}\
template<class Type>\
TC_TO_CONCEPT(className, detail::className##_c, Type)

///<summary>
///typeName�̃����o�^���������ǂ���
///</summary>
#define TC_HAS_MEMBER_TYPE(className ,typeName)\
namespace detail\
{\
template<class Type>\
using className##_c = tc::constraint<typename Type::typeName>;\
}\
template<class Type>\
TC_TO_CONCEPT(className, detail::className##_c, Type)

///<summary>
///symbol�P�����Z���\��
///</summary>
#define TC_HAS_UNARY_OPERATOR(className,symbol)\
namespace detail\
{\
	template<class Type>\
	using className##_c = tc::constraint<decltype(symbol std::declval<Type&>())>;\
}\
template<class Type>\
TC_TO_CONCEPT(className, detail::className##_c, Type)

///<summary>
///symbol2�����Z���\��
///</summary>
#define TC_HAS_BINARY_OPERATOR(className,symbol)\
namespace detail\
{\
template<class Left,class Right=Left>\
using className##_c = tc::constraint<decltype(std::declval<Left&>() symbol std::declval<Right&>())>; \
}\
template<class Left,class Right=Left>\
TC_TO_CONCEPT(className, detail::className##_c, Left,Right)


//************************************************************************************************
//
//concept_map
//
//************************************************************************************************

namespace tc
{

	///<summary>
	///���̃C���X�^���X�쐬(declval�̃w���p)
	///</summary>
	template<class Type>
	Type&& val = std::declval<Type>();

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
		template<class...>
		struct void_t_impl
		{
			typedef void type;
		};
		template<class... Test>
		using void_t = typename void_t_impl<Test...>::type;

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
		auto ref_make_tuple(Type&&... arg)->decltype(std::tuple<Type&&...>(std::forward<Type>(arg)...))
		{
			return std::tuple<Type&&...>(std::forward<Type>(arg)...);
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
	template<template<class...>class Concept, class ...Type>
	auto concept_mapping(Type&&... value)->decltype(detail::make_mapping_tuple<Concept<std::remove_reference_t<Type>...>>(value...))
	{
		using C = Concept<std::remove_reference_t<Type>...>;
		return detail::make_mapping_tuple<C>(value...);
	}

}//namespace tc

 //----------------------------------------------------------------------------------
namespace tc
{

	namespace detail
	{
		template<class = void, template<class...>class Constraint, class ...Args>
		struct is_detected_impl :std::false_type
		{};

		template<template<class...>class Constraint, class ...Args>
		struct is_detected_impl<void_t<Constraint<Args...>>, Constraint, Args...> :std::true_type
		{};

		//-- TODO std::is_detected
		template<template<class...>class Constraint, class ...Args>
		using is_detected = is_detected_impl <void, Constraint, Args...>;

		template<class Concept, class Arg>
		struct mapped_type
		{
			using type = std::remove_reference_t<decltype(tc::concept_mapping<Concept>(tc::val<Arg&>))>;
		};
		template<template<class...>class Concept, template<class...>class Meta, class ...Arg>
		struct to_concept :Meta<typename mapped_type<Concept<std::remove_reference_t<Arg>...>, Arg>::type...>
		{};

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
	 ///����
	 ///</summary>
	template<class ...Args>
	using constraint = detail::void_t<Args...>;

	///<summary>
	///Test�����������ꍇ Constraint�𐧖�Ƃ݂Ȃ�
	///</summary>
	template<bool Test, template<class...>class Constraint, class...Args>
	using constraint_if = typename detail::constraint_if_impl<Test, void, Constraint, Args...>::type;

	///<summary>
	///Concept�������𖞂�����
	///</summary>
	template < class WhereConcept >
	using requires = std::enable_if_t<WhereConcept::value, std::nullptr_t >;

	///<summary>
	///Constraint�����^�֐��ɕϊ�
	///</summary>
	template<template<class...>class Constraint>
	struct to_meta_func
	{
		template<class... Args>
		using type = detail::is_detected<Constraint, Args...>;
	};
	template<template<class...>class Constraint, class ...Args>
	using to_meta_func_t = typename to_meta_func<Constraint>::template type<Args...>;


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
		//���Z�q
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
		TC_TO_CONCEPT(PostIncrementable, detail::PostIncrementable_c, Type);

		///<summary>
		/// ��u�f�N�������g�\��
		///</summary>		
		template<class Type>
		TC_TO_CONCEPT(PostDecrementable, detail::PostDecrementable_c, Type);

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

		//************************************************************************************************
		//
		//��{�I�ȃR���Z�v�g
		//
		//************************************************************************************************

		///<summary>
		/// operator =(copy) ������
		///</summary>
		template< class To, class From = To>
		struct CopyAssignable : tc::detail::to_concept< CopyAssignable, std::is_assignable,
			typename std::add_lvalue_reference<To>::type,
			typename std::add_lvalue_reference<typename std::add_const<From>::type>::type
		>
		{};

		///<summary>
		/// operator =(move) ������
		///</summary>
		template< class To, class From = To>
		struct MoveAssignable :tc::detail::to_concept< MoveAssignable, std::is_assignable,
			typename std::add_lvalue_reference<To>::type,
			typename std::add_rvalue_reference<From>::type
		>
		{};

		///<summary>
		/// Type( Args... ) �̌`���̃R���X�g���N�^�Ăяo�����\��
		///</summary>
		template< class Type, class... Args>
		struct Constructible :tc::detail::to_concept<Constructible, std::is_constructible, Type, Args...>
		{};

		///<summary>
		/// �f�t�H���g�R���X�g���N�^ ������
		///</summary>
		template< class Type >
		struct DefaultConstructible : tc::detail::to_concept<DefaultConstructible, std::is_default_constructible, Type>
		{};

		///<summary>
		/// �R�s�[�R���X�g���N�^ ������
		///</summary>
		template< class Type>
		struct CopyConstructible : tc::detail::to_concept<CopyConstructible, std::is_copy_constructible, Type>
		{};

		///<summary>
		/// ���[�u�R���X�g���N�^ ������
		///</summary>
		template< class Type>
		struct MoveConstructible : tc::detail::to_concept< MoveConstructible, std::is_move_constructible, Type>
		{};

		///<summary>
		/// �f�X�g���N�^ ������
		///</summary>
		template< class Type>
		struct Destructible : tc::detail::to_concept<Destructible, std::is_destructible, Type>
		{};

		///<summary>
		/// ���z�f�X�g���N�^ ������
		///</summary>
		template< class Type>
		struct HasVirtualDestructor : tc::detail::to_concept<HasVirtualDestructor, std::has_virtual_destructor, Type>
		{};

		///<summary>
		///T��U��������
		///</summary>
		template<class T, class U>
		struct IsSame : tc::detail::to_concept<IsSame, std::is_same, T, U>
		{};

		///<summary>
		///Type��Base,��������Base���p�����Ă��邩
		///</summary>
		template<class Type, class Base>
		struct Extended : tc::detail::to_concept<Extended, std::is_base_of, Base, Type>
		{};

		///<summary>
		///�X�J���[���ǂ���
		///</summary>
		template<class Type>
		struct Scalar :tc::detail::to_concept<Scalar, std::is_scalar, Type>
		{};

		///<summary>
		///���z�N���X���ǂ���
		///</summary>
		template<class Type>
		struct Abstract :tc::detail::to_concept<Abstract, std::is_abstract, Type>
		{};
		///<summary>
		///enum�^��
		///</summary>
		template<class Type>
		struct Enum :tc::detail::to_concept<Enum, std::is_enum, Type>
		{};

		///<summary>
		///class(struct)�^��
		///</summary>
		template<class Type>
		struct Class :tc::detail::to_concept<Class, std::is_class, Type>
		{};

		///<summary>
		///union�^��
		///</summary>
		template<class Type>
		struct Union :tc::detail::to_concept<Union, std::is_union, Type>
		{};

		///<summary>
		///�֐��^t(...)��
		///</summary>
		template<class Type>
		struct Function :tc::detail::to_concept<Function, std::is_function, Type>
		{};

		//************************************************************************************************
		//
		//�^�̓���
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

		 ///<summary>
		 ///�A���P�[�^�[��
		 ///</summary>
		template<class Type>
		TC_TO_CONCEPT(Allocator, detail::Allocator_c, Type);

		///<summary>
		///T�̃I�u�W�F�N�g��U�̃I�u�W�F�N�g������ւ��\��
		///</summary>
		template<class T, class U = T>
		TC_TO_CONCEPT(Swappable, detail::Swappable_c, std::add_lvalue_reference_t<T>, std::add_lvalue_reference_t<U>);

		///<summary>
		/// From��To�ɃL���X�g�ł��邩
		///</summary>
		template< class From, class To >
		struct Convertible : tc::detail::to_concept<Convertible, std::is_convertible, From, To>
		{};

		///<summary>
		/// null���e��
		///</summary>
		template<class Type>
		TC_TO_CONCEPT(NullablePointer, detail::NullablePointer_c, Type);

		//--TODO is_invocable�ɕύX������--

		///<summary>
		///�֐��Ăяo���\�Ȍ^��
		///</summary>
		template <class Func, class... Args>
		TC_TO_CONCEPT(Invocable, detail::Invocable_c, Func, Args...);

		///<summary>
		///�֐��I�u�W�F�N�g��
		///</summary>
		template <class Type>
		TC_TO_CONCEPT(FunctionObject, detail::FunctionObject_c, Type);

		///<summary>
		///�R���Z�v�g��
		///</summary>
		template <class Type>
		TC_TO_CONCEPT(Concept, detail::Concept_c, Type);

		///<summary>
		///�n�b�V���֐��I�u�W�F�N�g��
		///</summary>
		template <class Type, class Key>
		TC_TO_CONCEPT(Hash, detail::Hash_c, Type, Key);

		///<summary>
		///���ԁA�����A���݂̎������擾�\��
		///</summary>
		template<class Type>
		TC_TO_CONCEPT(Clock, detail::Clock_c, Type);

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

		//************************************************************************************************
		//
		//���̑�
		//
		//************************************************************************************************

		///<summary>
		///������conditional���݂�����
		///</summary>
		template<bool conditional>
		struct Condition : std::bool_constant<conditional>
		{};

	}//namespace Concept


}//namespace tc


 ///<summary>
 ///where�}�N��
 ///</summary>
#define TC_WHERE( ... ) tc::requires< __VA_ARGS__ > = nullptr

 ///<summary>
 ///�������݂����Ȃ��ƃA�T�[�g
 ///</summary>
#define TC_CONCEPT_ASSERT( ... ) static_assert(__VA_ARGS__::value,#__VA_ARGS__ )
