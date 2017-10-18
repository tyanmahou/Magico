#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<chrono>
#include"Concept.hpp"

using namespace tc;


struct As
{
	template<class T>
	auto requires(T&& v)->decltype(
		tc::requires_extends<tc::concept::Plusable<T>>,
		tc::requires_extends<tc::concept::Minusable<T>>
		);
};

template<class T>
using A = tc::to_concept<As, T>;

struct Hoge
{

};

template<>
struct tc::concept_map<tc::concept::Hash<Hoge,int>>
{
	struct T
	{
		size_t operator ()(int v)
		{
			return v;
		}
	};

	T operator =(const Hoge& a)
	{
		return T();
	}

};
int main()
{	
	std::cout<<tc::concept::Clock<std::chrono::system_clock>::value<<std::endl;
	return 0;
}
