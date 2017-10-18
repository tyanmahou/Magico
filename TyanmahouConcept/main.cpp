#include<iostream>
#include"Concept.hpp"


struct Test_c
{
	template<class T>
	auto requires(T&& t)->decltype(
			tc::vailed_expr<int&>(t.hoge())
			);
};

///aa
template<class T>
using Test = tc::to_concept<Test_c, T>;

struct A
{
	int a;
	int& hoge()
	{
		return a;
	}
};
struct doo
{
	bool operator<(const doo&) const{ return false; }
};
template<class RandomAccessIterator>
struct RandI
{
	void c()
	{
		typename std::iterator_traits<RandomAccessIterator>::difference_type n;
		i += n; // exercise the requirements for RandomAccessIterator
	}
};
#include<algorithm>
#include<list>

template<class RandomAccessIterator>
void stable_sort_(RandomAccessIterator b, RandomAccessIterator e)
{
	std::stable_sort(b, e);
}
int main()
{
	std::list<doo> l;
	stable_sort_(l.begin(),l.end());
	std::cout << Test<A>::value << std::endl;
	std::cout << tc::Concept::Plusable<int,tc::as_mapped<A>>::value << std::endl;
	return 0;

}
