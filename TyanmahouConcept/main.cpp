#include<iostream>
#include<vector>

#include"Concept.hpp"


template<class T>
using test_c = tc::constraint<
	typename T::hoge
>;

template<class T>
TC_TO_CONCEPT(Test, test_c,T);


template<class T>
using test2_c = tc::constraint<
	Test<T>::constraint<T>,
	typename T::piyo
>;

template<class T>
TC_TO_CONCEPT(Test2, test2_c, T);

struct A {
	using hoge = int;
	using piyo = int;
};
struct B {
	using piyo=int;
};

template<>
struct tc::concept_map<Test<B>>
{
	struct Wrap:B
	{
		using hoge = int;
	};

	Wrap& operator=(B& o)
	{
		return static_cast<Wrap&>(o);
	}
};
template<class T,TC_WHERE(Test2<T>)>
void f(T a)
{
//	T::hoge i = 0;
}

void main()
{
	Test<A> a;
	f(A{});
	f(B{});
}
