#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include<memory>
#include<chrono>

#include<Concept.hpp>
#include<cassert>

template<class N>
struct Number : tc::to_concept_ex<Number, struct __Number_c, N> 
{
	template<class... Args>
	static bool axion(Args&&...args)
	{
		return __Number_c::axion<N>(args...);
	}
}; 
struct __Number_c 
{
	template<class N>
	auto require()->decltype(
		tc::extends<tc::concept::Plusable>::require<N, int>(),
		tc::extends<tc::concept::Plusable>::require<int, N>(),
		tc::extends<tc::concept::EqualityComparable>::require<N>()
		);
	template<class N>
	static const bool axion()
	{

		static N a, b;
		static bool check =
			(a + 0 == a)&
			(0 + a == a)&
			(a + b == b + a);
		return check;
	}
};

struct A
{
	int v = 0;

	A operator+(A o)
	{
		return { this->v + o.v };
	}

	A operator+(int a)
	{
		return { this->v + a + 2 };
	}
	friend A operator+(int a, A b)
	{
		return { b.v + a };
	}
	bool operator==(const A& o)
	{
		return this->v == o.v;
	}

};

class axion_exception
	: public std::exception
{
public:

	axion_exception() throw() :
		std::exception("bad allocation", 1)
	{
	}
	axion_exception(char const* const _Message) throw() :
		std::exception(_Message, 1)
	{
	}
};

#define TC_AXION_CHECK(...)\
if (!__VA_ARGS__)\
{\
	throw axion_exception(#__VA_ARGS__);\
}
template<class T>
void test()
{
	try
	{
		TC_AXION_CHECK(Number<T>::axion())

	}
	catch (axion_exception e)
	{
		std::cout<<e.what();
	}

}
#include<any>
int main()
{
	Number<int>::value;

	test<A>();
	std::any a = 0;

	std::cout << "Test";
	return 0;
}
