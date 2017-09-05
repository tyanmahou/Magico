#pragma once
#include<type_traits>
#include<tuple>

//----------------------------------------------------------------------------------
//memName�̃����o�������ǂ���
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
//typeName�̃����o�^���������ǂ���
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
//symbol�P�����Z���\��
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
//symbol2�����Z���\��
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
	///���̃C���X�^���X�쐬(declval�̃w���p)
	///</summary>
	template<class Type>
	Type&& val = std::declval<Type>();

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
	///Constraint�𖞂�����
	///</summary>
	template <template<class...>class Constraint, class ...Args>
	using requires_c = requires<detail::is_detected<Constraint, Args...>>;

	///<summary>
	///Constraint��Concept�`���ɕϊ�
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
	///Concept��Constraint�`���ɕϊ�
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
		struct PostIncrementable :to_concept<detail::PostIncrementable_c>::type< Type>
		{};
		///<summary>
		/// ��u�f�N�������g�\��
		///</summary>		
		template<class Type>
		struct PostDecrementable :to_concept<detail::PostDecrementable_c>::type< Type>
		{};
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
		/// operator < ������
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessThanComparable, <);

		///<summary>
		/// operator <= ������
		///</summary>
		TC_HAS_BINARY_OPERATOR(LessEqualComparable, <= );

		///<summary>
		/// operator > ������
		///</summary>
		TC_HAS_BINARY_OPERATOR(GreaterThanComparable, >);

		///<summary>
		/// operator >= ������
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
		struct CopyAssignable : std::is_assignable<
			typename std::add_lvalue_reference<To>::type,
			typename std::add_lvalue_reference<
			typename std::add_const<From>::type
			>::type
		>::type
		{};

		///<summary>
		/// operator =(move) ������
		///</summary>
		template< class To, class From = To>
		struct MoveAssignable :std::is_assignable<
			typename std::add_lvalue_reference<To>::type,
			typename std::add_rvalue_reference<From>::type
		>::type
		{};

		///<summary>
		/// Type( Args... ) �̌`���̃R���X�g���N�^�Ăяo�����\��
		///</summary>
		template< class Type, class... Args>
		struct Constructible : std::is_constructible<Type, Args...>
		{};

		///<summary>
		/// �f�t�H���g�R���X�g���N�^ ������
		///</summary>
		template< class Type >
		struct DefaultConstructible : std::is_default_constructible<Type>
		{};

		///<summary>
		/// �R�s�[�R���X�g���N�^ ������
		///</summary>
		template< class Type>
		struct CopyConstructible : std::is_copy_constructible<Type>
		{};

		///<summary>
		/// ���[�u�R���X�g���N�^ ������
		///</summary>
		template< class Type>
		struct MoveConstructible : std::is_move_constructible<Type>
		{};

		///<summary>
		/// �f�X�g���N�^ ������
		///</summary>
		template< class Type>
		struct Destructible : std::is_destructible<Type>
		{};

		///<summary>
		/// ���z�f�X�g���N�^ ������
		///</summary>
		template< class Type>
		struct HasVirtualDestructor : std::has_virtual_destructor<Type>
		{};

		///<summary>
		///T��U��������
		///</summary>
		template<class T, class U>
		struct IsSame : std::is_same<T, U>
		{};

		///<summary>
		///Type��Base,��������Base���p�����Ă��邩
		///</summary>
		template<class Type, class Base>
		struct Extended : std::is_base_of<Base, Type>
		{};

		///<summary>
		///�X�J���[���ǂ���
		///</summary>
		template<class Type>
		struct Scalar :std::is_scalar<Type>
		{};

		///<summary>
		///���z�N���X���ǂ���
		///</summary>
		template<class Type>
		struct Abstract :std::is_abstract<Type>
		{};
		///<summary>
		///enum�^��
		///</summary>
		template<class Type>
		struct Enum :std::is_enum<Type>
		{};

		///<summary>
		///class(struct)�^��
		///</summary>
		template<class Type>
		struct Class :std::is_class<Type>
		{};

		///<summary>
		///union�^��
		///</summary>
		template<class Type>
		struct Union :std::is_union<Type>
		{};

		///<summary>
		///�֐��^t(...)��
		///</summary>
		template<class Type>
		struct Function :std::is_function<Type>
		{};

		//////************************************************************************************************
		//////
		//////�^�̓���
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
		///�A���P�[�^�[��
		///</summary>
		template<class Type>
		struct Allocator :to_concept<detail::Allocator_c>::type<Type>
		{};

		///<summary>
		///T�̃I�u�W�F�N�g��U�̃I�u�W�F�N�g������ւ��\��
		///</summary>
		template<class T, class U = T>
		struct Swappable :to_concept<detail::Swappable_c>::type< std::add_lvalue_reference_t<T>, std::add_lvalue_reference_t<U>>
		{};

		///<summary>
		/// From��To�ɃL���X�g�ł��邩
		///</summary>
		template< class From, class To >
		struct Convertible :std::is_convertible<From, To>
		{};

		///<summary>
		/// null���e��
		///</summary>
		template<class Type>
		struct NullablePointer : to_concept<detail::NullablePointer_c>::type<Type>
		{};

		//--TODO is_invocable�ɕύX������--

		///<summary>
		///�֐��Ăяo���\�Ȍ^��
		///</summary>
		template <class Func, class... Args>
		struct Invocable : to_concept<detail::Invocable_c>::type< Func, Args...>
		{};

		///<summary>
		///�֐��I�u�W�F�N�g��
		///</summary>
		template <class Type>
		struct FunctionObject :to_concept<detail::FunctionObject_c>::type< Type>
		{};

		///<summary>
		///�R���Z�v�g��
		///</summary>
		template <class Type>
		struct Concept : to_concept<detail::Concept_c>::type< Type>
		{};

		///<summary>
		///�n�b�V���֐��I�u�W�F�N�g��
		///</summary>
		template <class Type, class Key>
		struct Hash : to_concept<detail::Hash_c>::type< Type, Key>
		{};

		///<summary>
		///���ԁA�����A���݂̎������擾�\��
		///</summary>
		template<class Type>
		struct Clock :to_concept<detail::Clock_c>::type< Type>
		{};


		//************************************************************************************************
		//
		//���C�A�E�g
		//
		//************************************************************************************************


		///<summary>
		///�g���r�A���R�s�[�\��
		///</summary>
		template<class Type>
		struct TriviallyCopyable : std::is_trivially_copyable<Type>
		{};

		///<summary>
		///�g���r�A���^��
		///</summary>
		template<class Type>
		struct TrivialType : std::is_trivial<Type>
		{};

		///<summary>
		///�W�����C�A�E�g�^��
		///</summary>
		template<class Type>
		struct StandardLayoutType : std::is_standard_layout<Type>
		{};

		///<summary>
		///POD�^��
		///</summary>
		template<class Type>
		struct PODType : std::is_pod<Type>
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
		struct Iterator : to_concept<detail::Iterator_c>::type<It>
		{};
		///<summary>
		///���̓C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct InputIterator : to_concept<detail::InputIterator_c>::type<It>
		{};

		///<summary>
		///�o�̓C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct OutputIterator : to_concept<detail::OutputIterator_c>::type<It>
		{};

		///<summary>
		///�O���C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct ForwardIterator : to_concept<detail::ForwardIterator_c>::type<It>
		{};
		///<summary>
		///�o�����C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct BidirectionalIterator : to_concept<detail::BidirectionalIterator_c>::type<It>
		{};

		///<summary>
		///�����_���A�N�Z�X�C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct RandomAccessIterator : to_concept<detail::RandomAccessIterator_c>::type<It>
		{};

		///<summary>
		///�C�e���[�^�[�̒l�^���X���b�v�\��
		///</summary>
		template<class It>
		struct ValueSwappable : to_concept<detail::ValueSwappable_c>::type<It>
		{};


		//************************************************************************************************
		//
		//�R���e�i
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
		///�R���e�i���ǂ���
		///</summary>
		template<class C>
		struct Container : to_concept<detail::Container_c>::type<C, Iterator<C>>
		{};

		///<summary>
		///�O���C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class C>
		struct ForwardContainer : to_concept<detail::Container_c>::type< C, ForwardIterator<C>>
		{};
		///<summary>
		///�����_���A�N�Z�X�C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class C>
		struct RandomAccessContainer : to_concept<detail::Container_c>::type< C, RandomAccessIterator<C>>
		{};

		///<summary>
		///���o�[�X�C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class C>
		struct ReversibleContainer : to_concept<detail::ReversibleContainer_c>::type<C>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^���f�t�H���g�ő}���\��
		///</summary>
		template<class C>
		struct DefaultInsertable :to_concept<detail::DefaultInsertable_c>::type<C>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^�̃R�s�[�}���\��
		///</summary>
		template<class C>
		struct CopyInsertable :to_concept<detail::CopyInsertable_c>::type<C>
		{};
		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^�̉E�Ӓl�I�u�W�F�N�g�����[�u�}���\��
		///</summary>
		template<class C>
		struct MoveInsertable :to_concept<detail::MoveInsertable_c>::type<C>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA�v�f�^�̃R���X�g���N�^������Args���璼�ڍ\�z�\��
		///</summary>
		template <class C, class... Args>
		struct EmplaceConstructible : to_concept<detail::EmplaceConstructible_c>::type<C, Args...>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA�v�f�^�̔j�����\��
		///</summary>
		template<class C>
		struct Erasable :to_concept<detail::Erasable_c>::type<C>
		{};

		///<summary>
		///�A���P�[�^�[��F������R���e�i��
		///</summary>
		template<class C>
		struct AllocatorAwareContainer : to_concept<detail::AllocatorAwareContainer_c>::type<C>
		{};
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

///<summary>
///where�}�N��
///</summary>
#define TC_WHERE( ... ) tc::requires< __VA_ARGS__ > = nullptr

///<summary>
///�������݂����Ȃ��ƃA�T�[�g
///</summary>
#define TC_CONCEPT_ASSERT( ... ) static_assert(__VA_ARGS__::value,#__VA_ARGS__ )


