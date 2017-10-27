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


		///<summary>
		///�C�e���[�^�[���ǂ���
		///</summary>
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


		///<summary>
		///���̓C�e���[�^�[���ǂ���
		///</summary>
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


		///<summary>
		///�o�̓C�e���[�^�[���ǂ���
		///</summary>
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


		///<summary>
		///�O���C�e���[�^�[���ǂ���
		///</summary>
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


		///<summary>
		///�o�����C�e���[�^�[���ǂ���
		///</summary>
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

		///<summary>
		///�����_���A�N�Z�X�C�e���[�^�[���ǂ���
		///</summary>
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



		///<summary>
		///�C�e���[�^�[�̒l�^���X���b�v�\��
		///</summary>
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

		///<summary>
		///�C�e���[�^�[������
		///</summary>
		template<class Type> TC_CONCEPT(HasIterator, Type)
		{

			template<class Type>
			auto require()->decltype(
				tc::extends<Iterator>::require<typename Type::iterator>()
				);
		};

		//************************************************************************************************
		//
		//�R���e�i
		//
		//************************************************************************************************


		///<summary>
		///�R���e�i���ǂ���
		///</summary>
		template<class X> TC_CONCEPT(Container, X)
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

		///<summary>
		///�O���C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X> TC_CONCEPT(ForwardContainer, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<ForwardIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///�����_���A�N�Z�X�C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X> TC_CONCEPT(RandomAccessContainer, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<RandomAccessIterator>::require<typename X::iterator>()
				);
		};

		///<summary>
		///���o�[�X�C�e���[�^�[�����R���e�i���ǂ���
		///</summary>
		template<class X> TC_CONCEPT(ReversibleContainer, X)
		{
			template<class X,
				class ritr = typename X::reverse_iterator,
				class critr = typename X::const_reverse_iterator
			>
				auto require(X&& x)->decltype(
					tc::extends<Container>::require<X>(),
					tc::extends<BidirectionalIterator>::require<typename X::iterator>(),
					tc::vailed_expr<ritr>(x.rbegin()),
					tc::vailed_expr<ritr>(x.rend()),
					tc::vailed_expr<critr>(x.crbegin()),
					tc::vailed_expr<critr>(x.crend())
					);
		};


		///<summary>
		///�C�ӂ̃R���e�iX�ɑ΂��āA���̗v�f�^���f�t�H���g�ő}���\��
		///</summary>
		template<class X> TC_CONCEPT(DefaultInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<DefaultConstructible>::require<typename X::value_type>()
				);
		};

		///<summary>
		///�C�ӂ̃R���e�iX�ɑ΂��āA���̗v�f�^�̃R�s�[�}���\��
		///</summary>
		template<class X> TC_CONCEPT(CopyInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<CopyConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///�C�ӂ̃R���e�iX�ɑ΂��āA���̗v�f�^�̉E�Ӓl�I�u�W�F�N�g�����[�u�}���\��
		///</summary>
		template<class X> TC_CONCEPT(MoveInsertable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<MoveConstructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///�C�ӂ̃R���e�iX�ɑ΂��āA�v�f�^�̃R���X�g���N�^������Args���璼�ڍ\�z�\��
		///</summary>
		template<class X, class... Args> TC_CONCEPT(EmplaceConstructible, X,Args...)
		{
			template<class X, class... Args>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<Constructible>::require<typename X::value_type, Args...>()
				);
		};

		///<summary>
		///�C�ӂ̃R���e�iX�ɑ΂��āA�v�f�^�̔j�����\��
		///</summary>
		template<class X> TC_CONCEPT(Erasable, X)
		{
			template<class X>
			auto require(X&& x)->decltype(
				tc::extends<Container>::require<X>(),
				tc::extends<Destructible>::require<typename X::value_type>()
				);
		};


		///<summary>
		///�A���P�[�^�[��F������R���e�i��
		///</summary>
		template<class X> TC_CONCEPT(AllocatorAwareContainer, X)
		{
			template<class X,class alloc_t = typename X::allocator_type>
				auto require(X&& x)->decltype(
					tc::extends<CopyAssignable, MoveAssignable, CopyInsertable, MoveInsertable>::require<X>(),
					tc::extends<DefaultConstructible>::require<std::allocator<typename X::value_type>>(),
					tc::extends<Constructible>::require<X, std::allocator<typename X::value_type>>(),
					tc::extends<Constructible>::require<X, std::add_lvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
					tc::extends<Constructible>::require<X, std::add_rvalue_reference_t<X>, std::allocator<typename X::value_type >>(),
					tc::vailed_expr<alloc_t>(x.get_allocator()),
					tc::vailed_expr<void>((x.swap(x), _void))
					);
		};

		///<summary>
		///�����W���ǂ���
		///</summary>
		template<class Type> TC_CONCEPT(Range, Type)
		{
			template<class Type>
			auto require(Type&& range)->decltype(
				tc::extends<Iterator>::require(std::begin(range)),
				tc::extends<Iterator>::require(std::end(range))
				);
		};

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


		///<summary>
		///Type��Template�^�ł��邩
		///</summary>
		template<template <class...>class Template, class Type>
		struct is_template : std::false_type
		{};
		template<template <class...>class Template, class... Args>
		struct is_template<Template, Template<Args...>> : std::true_type
		{};


	}//namespace concept
}//namespace tc


