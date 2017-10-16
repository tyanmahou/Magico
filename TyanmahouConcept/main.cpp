#include<iostream>
#include"Concept.hpp"


struct Test_c
{
	template<class T>
	auto requires(T&& t)->tc::constraint<
		decltype(
			tc::has_type<typename T::Hoge>
			)>;
};

///aa
template<class T>
using Test = tc::to_concept<Test_c, T>;
struct A
{
	int a;
	int operator -()
	{
		return -a;
	}
};

template<>
struct tc::concept_map<tc::Concept::Plusable<int,A>>
{
	int operator=(A& a)
	{
		return 10;
	}
};

int main()
{
	std::cout << Test<tc::as_mapped<A>>::value << std::endl;
	std::cout << tc::Concept::Plusable<int,tc::as_mapped<A>>::value << std::endl;
	return 0;

}
