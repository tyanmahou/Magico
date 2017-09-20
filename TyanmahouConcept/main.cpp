#include<iostream>
#include<vector>
#include<list>
#include"Concept.hpp"

template<class T, TC_WHERE(std::is_integral<T>)>
struct Test
{};

template<class T>
using IntegerTest_c = tc::constraint<
	typename Test<T>
>;

template<class T>
TC_TO_CONCEPT(IntegerTest, IntegerTest_c, T);


template<>
struct tc::concept_map<IntegerTest<double>>
{
	int operator =(double& a)
	{
		int b = a + 2;

		return b;
	}
};

template<class T>
void f(tc::concept_any<IntegerTest> _v)
{
	auto v = _v.get<T>();

	std::cout << v << std::endl;
}

std::tuple<double,int,int> test(double& a)
{
	return tc::detail::ref_make_tuple(0.5, 0, 1);
}
void main()
{
//	f<int>(0);
	double a = 0.5;
	double b = 1.5;
	//f<double>(0.5);
	std::cout << std::get<0>(tc::concept_mapping<IntegerTest>(0.5)) << std::endl;


}
