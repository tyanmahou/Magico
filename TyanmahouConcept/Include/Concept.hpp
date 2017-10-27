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
		TC_CONCEPT(PostIncrementable, Type)
		{
			template<class Type>
			auto require(Type&& t)->decltype(t++);
		};

		///<summary>
		/// ��u�f�N�������g�\��
		///</summary>
		template<class Type>
		TC_CONCEPT(PostDecrementable, Type)
		{
			template<class Type>
			auto require(Type&& t)->decltype(t--);
		};
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
template<class Left,class Right=Left> \
TC_CONCEPT(className,Left,Right)\
{\
	template<class Left,class Right>\
	auto require(Left&& l,Right&& r)->decltype(l symbol r);\
}

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
		//Basic
		//
		//************************************************************************************************

		///<summary>
		/// operator =(copy) ������
		///</summary>
		template< class To, class From = To> TC_CONCEPT(CopyAssignable, To, From)
		{
			template<class To, class From>
			auto require(To& t, const From& f)->decltype(
				tc::extends<std::is_assignable>::require(t, f)
				);
		};

		///<summary>
		/// operator =(move) ������
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
		/// Type( Args... ) �̌`���̃R���X�g���N�^�Ăяo�����\��
		///</summary>
		template< class Type, class... Args> TC_CONCEPT(Constructible, Type, Args...)
		{
			template<class Type, class... Args>
			auto require()->decltype(
				tc::extends<std::is_constructible>::require<Type, Args...>()
				);
		};

		///<summary>
		/// �f�t�H���g�R���X�g���N�^ ������
		///</summary>
		template< class Type > TC_CONCEPT(DefaultConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_default_constructible>::require<Type>()
				);
		};
		///<summary>
		/// �R�s�[�R���X�g���N�^ ������
		///</summary>
		template< class Type > TC_CONCEPT(CopyConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_copy_constructible>::require<Type>()
				);
		};
		///<summary>
		/// ���[�u�R���X�g���N�^ ������
		///</summary>
		template< class Type > TC_CONCEPT(MoveConstructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_move_constructible>::require<Type>()
				);
		};
		///<summary>
		/// �f�X�g���N�^ ������
		///</summary>
		template< class Type > TC_CONCEPT(Destructible, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_destructible>::require<Type>()
				);
		};
		///<summary>
		/// ���z�f�X�g���N�^ ������
		///</summary>
		template< class Type > TC_CONCEPT(HasVirtualDestructor, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::has_virtual_destructor>::require<Type>()
				);
		};

		///<summary>
		///T��U��������
		///</summary>
		template<class T, class U> TC_CONCEPT(IsSame, T, U)
		{
			template<class T, class U>
			auto require()->decltype(
				tc::extends<std::is_same>::require<T, U>()
				);
		};

		///<summary>
		///Type��Base,��������Base���p�����Ă��邩
		///</summary>
		template<class Type, class Base> TC_CONCEPT(Extended, Type, Base)
		{
			template<class Type, class Base>
			auto require()->decltype(
				tc::extends<std::is_base_of>::require<Base, Type>()
				);
		};

		///<summary>
		///�X�J���[���ǂ���
		///</summary>
		template< class Type > TC_CONCEPT(Scalar, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_scalar>::require<Type>()
				);
		};

		///<summary>
		///���z�N���X���ǂ���
		///</summary>
		template< class Type > TC_CONCEPT(Abstract, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_abstract>::require<Type>()
				);
		};

		///<summary>
		///enum�^��
		///</summary>
		template< class Type > TC_CONCEPT(Enum, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_enum>::require<Type>()
				);
		};

		///<summary>
		///class(struct)�^��
		///</summary>
		template< class Type > TC_CONCEPT(Class, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_class>::require<Type>()
				);
		};

		///<summary>
		///union�^��
		///</summary>
		template< class Type > TC_CONCEPT(Union, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_union>::require<Type>()
				);
		};

		///<summary>
		///�֐��^t(...)��
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
		///�A���P�[�^�[��
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
		///T�̃I�u�W�F�N�g��U�̃I�u�W�F�N�g������ւ��\��
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
		/// From��To�ɃL���X�g�ł��邩
		///</summary>
		template< class From, class To > TC_CONCEPT(Convertible, From, To)
		{
			template< class From, class To >
			auto require()->decltype(
				tc::extends<std::is_convertible>::require<Type>()
				);
		};

		///<summary>
		/// null���e��
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
		///�֐��Ăяo���\�Ȍ^��
		///</summary>
		template <class F, class... Args>TC_CONCEPT(Invocable, F, Args...)
		{
			template <class F, class... Args>
			auto require(F&& func, Args&&... args)->decltype(
				std::invoke(func, args...)
				);

		};

		///<summary>
		///�֐��I�u�W�F�N�g��
		///</summary>
		template <class Type> TC_CONCEPT(FunctionObject, Type)
		{
			template <class Type>
			auto require()->decltype(
				&Type::operator()
				);
		};

		///<summary>
		///���^�֐���
		///</summary>
		template <class Type> TC_CONCEPT(MetaFunc, Type)
		{
			template <class Type>
			auto require()->decltype(
				vailed_expr<const bool&>(Type::value)
				);
		};

		///<summary>
		///�n�b�V���֐��I�u�W�F�N�g��
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
		///���ԁA�����A���݂̎������擾�\��
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
		///�g���r�A���R�s�[�\��
		///</summary>
		template< class Type > TC_CONCEPT(TriviallyCopyable, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_trivially_copyable>::require<Type>()
				);
		};

		///<summary>
		///�g���r�A���^��
		///</summary>
		template< class Type > TC_CONCEPT(TrivialType, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_trivial>::require<Type>()
				);
		};
		///<summary>
		///�W�����C�A�E�g�^��
		///</summary>
		template<class Type>TC_CONCEPT(StandardLayoutType, Type)
		{
			template<class Type>
			auto require()->decltype(
				tc::extends<std::is_standard_layout>::require<Type>()
				);
		};

		///<summary>
		///POD�^��
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
		namespace detail
		{

			struct ValueSwappable_c
			{
			};

			struct HasIterator_c
			{
				template<class Type>
				auto require()->decltype(
					&Iterator_c::require<typename Type::iterator>
					);
			};
		}//namespace detail


		///<summary>
		///�C�e���[�^�[������
		///</summary>
		template<class Type>
		struct HasIterator : to_concept_ex<HasIterator, detail::HasIterator_c, Type>
		{};

		///<summary>
		///�C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct Iterator : to_concept_ex<Iterator, detail::Iterator_c, It>
		{};
		///<summary>
		///���̓C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct InputIterator : to_concept_ex<InputIterator, detail::InputIterator_c, It>
		{};
		///<summary>
		///�o�̓C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct OutputIterator : to_concept_ex<OutputIterator, detail::OutputIterator_c, It>
		{};

		///<summary>
		///�O���C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct ForwardIterator : to_concept_ex<ForwardIterator, detail::ForwardIterator_c, It>
		{};
		///<summary>
		///�o�����C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct BidirectionalIterator : to_concept_ex<BidirectionalIterator, detail::BidirectionalIterator_c, It>
		{};
		///<summary>
		///�����_���A�N�Z�X�C�e���[�^�[���ǂ���
		///</summary>
		template<class It>
		struct RandomAccessIterator : to_concept_ex<RandomAccessIterator, detail::RandomAccessIterator_c, It>
		{};


		///<summary>
		///�C�e���[�^�[�̒l�^���X���b�v�\��
		///</summary>
		template<class It>
		struct ValueSwappable : to_concept_ex<ValueSwappable, detail::ValueSwappable_c, It>
		{};

		//************************************************************************************************
		//
		//�R���e�i
		//
		//************************************************************************************************
		namespace detail
		{
			struct Container_c
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

			struct ForwardContainer_c
			{
				template<class X,
					class extends = tc::require<ForwardIterator<typename X::iterator>>
				>
					auto require(X&& x)->decltype(
						tc::require_c(&Container_c::require<X>)
						);
			};


			struct RandomAccessContainer_c
			{
				template<class X,
					class extends = tc::require<RandomAccessIterator<typename X::iterator>>
				>
					auto require(X&& x)->decltype(
						tc::require_c(&Container_c::require<X>)
						);
			};

			struct ReversibleContainer_c
			{
				template<class X,
					class ritr = typename X::reverse_iterator,
					class critr = typename X::const_reverse_iterator,
					class extends = tc::require<BidirectionalIterator<typename X::iterator>>
				>
					auto require(X&& x)->decltype(
						tc::require_c(&Container_c::require<X>),
						tc::vailed_expr<ritr>(x.rbegin()),
						tc::vailed_expr<ritr>(x.rend()),
						tc::vailed_expr<critr>(x.crbegin()),
						tc::vailed_expr<critr>(x.crend())
						);
			};

			struct DefaultInsertable_c
			{
				template<class X,
					class extends = tc::require<DefaultConstructible<typename X::iterator>>
				>
					auto require(X&& x)->decltype(
						tc::require_c(&Container_c::require<X>)
						);
			};

			struct CopyInsertable_c
			{
				template<class X,
					class extends = tc::require<CopyConstructible<typename X::iterator>>
				>
					auto require(X&& x)->decltype(
						tc::require_c(&Container_c::require<X>)
						);
			};

			struct MoveInsertable_c
			{
				template<class X,
					class extends = tc::require<MoveConstructible<typename X::iterator>>
				>
					auto require(X&& x)->decltype(
						tc::require_c(&Container_c::require<X>)
						);
			};

			struct EmplaceConstructible_c
			{
				template<class X, class... Args>
				auto require(X&& x)->decltype(
					tc::require<Constructible<typename X::value_type, Args...>>{},
					tc::require_c(&Container_c::require<X>)
					);
			};

			struct Erasable_c
			{
				template<class X,
					class extends = tc::require<Destructible<typename X::iterator>>
				>
					auto require(X&& x)->decltype(
						tc::require_c(&Container_c::require<X>)
						);
			};

			struct AllocatorAwareContainer_c
			{
				template<class X,
					class alloc_t = typename X::allocator_type,
					class extends = tc::require<
					CopyAssignable<X>,
					MoveAssignable<X>,
					DefaultConstructible<std::allocator<typename X::value_type>>,
					Constructible < X, std::allocator<typename X::value_type>>,
					Constructible < X, std::add_lvalue_reference_t<X>, std::allocator<typename X::value_type >>,
					Constructible < X, std::add_rvalue_reference_t<X>, std::allocator<typename X::value_type >>
					>
				>
					auto require(X&& x)->decltype(
						tc::require_c(&CopyInsertable_c::require<X>, &MoveInsertable_c::require<X>),
						tc::vailed_expr<alloc_t>(x.get_allocator()),
						tc::vailed_expr<void>((x.swap(x), _void))
						);
			};


			struct Range_c
			{
				template<class Type>
				auto require(Type&& t)->decltype(
					tc::require<
					Iterator<decltype(std::begin(t))>,
					Iterator<decltype(std::end(t))>
					>{}
				);
			};

		}//namespace detail


		///<summary>
		///�R���e�i���ǂ���
		///</summary>
		template<class X>
		struct Container :tc::to_concept_ex<Container, detail::Container_c, X>
		{};


		///<summary>
		///�O���C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X>
		struct ForwardContainer :tc::to_concept_ex<ForwardContainer, detail::ForwardContainer_c, X>
		{};
		///<summary>
		///�����_���A�N�Z�X�C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X>
		struct RandomAccessContainer :tc::to_concept_ex<RandomAccessContainer, detail::RandomAccessContainer_c, X>
		{};

		///<summary>
		///���o�[�X�C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X>
		struct ReversibleContainer :tc::to_concept_ex<ReversibleContainer, detail::ReversibleContainer_c, X>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^���f�t�H���g�ő}���\��
		///</summary>
		template<class X>
		struct DefaultInsertable :tc::to_concept_ex< DefaultInsertable, detail::DefaultInsertable_c, X>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^�̃R�s�[�}���\��
		///</summary>
		template<class X>
		struct CopyInsertable :tc::to_concept_ex<CopyInsertable, detail::CopyInsertable_c, X>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA���̗v�f�^�̉E�Ӓl�I�u�W�F�N�g�����[�u�}���\��
		///</summary>
		template<class X>
		struct MoveInsertable :tc::to_concept_ex<MoveInsertable, detail::MoveInsertable_c, X>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA�v�f�^�̃R���X�g���N�^������Args���璼�ڍ\�z�\��
		///</summary>
		template <class X, class... Args>
		struct EmplaceConstructible :tc::to_concept_ex<EmplaceConstructible, detail::EmplaceConstructible_c, X, Args...>
		{};

		///<summary>
		///�C�ӂ̃R���e�iC�ɑ΂��āA�v�f�^�̔j�����\��
		///</summary>
		template<class X>
		struct Erasable :tc::to_concept_ex<Erasable, detail::Erasable_c, X>
		{};


		///<summary>
		///�A���P�[�^�[��F������R���e�i��
		///</summary>
		template<class X>
		struct AllocatorAwareContainer :tc::to_concept_ex<AllocatorAwareContainer, detail::AllocatorAwareContainer_c, X>
		{};

		///<summary>
		///�����W���ǂ���
		///</summary>
		template<class Type>
		struct Range :tc::to_concept_ex< Range, detail::Range_c, Type>
		{};

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


	}
}//namespace tc::concept


