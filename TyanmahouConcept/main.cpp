#include<iostream>
#include"Concept.hpp"


struct Test_c {
	template<class T>
	auto requires(T& t)->decltype(
		tc::has_type<typename T::Hoge>
		);
};
template<class T>
using Test = tc::to_concept<Test_c, T>;


template<>
struct tc::concept_map<Test<int>>
{
	struct W :std::reference_wrapper<int>
	{
		void a(){}
		void b(){}
		struct Hoge 
		{
		private:
			Hoge() = default;
		};
		using reference_wrapper<int>::reference_wrapper;
		
		int& operator=(const int& v)
		{
			return static_cast<int&>(*this) = v;
		}
	};

	W operator=(int& a)
	{
		W ret(a);
		return ret;
	}
};


struct A
{
	int a = 0;
	using Hoge = int;
};

template<class T, TC_WHERE(Test<tc::ConceptMapped_t<Test<T>,T>>)>
void f(T a)
{
	std::cout << typeid(Test<T>).name() << std::endl;

}
int main()
{
	A aa{0};
	int a = 0;
	tc::concept_any<Test> any = aa;
	tc::concept_any<Test> any2(std::ref(a));
	
	any.swap(any2);

	any.get<int&>() = 114;
	
	std::cout << a;
	return 0;
}
