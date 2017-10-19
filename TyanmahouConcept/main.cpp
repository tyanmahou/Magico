#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<set>
#include<stack>
#include<chrono>
#include"Concept.hpp"


template<class T>
auto test(T begin,T end)->tc::where<void,tc::concept::RandomAccessIterator<T>>
{
	std::cout << tc::concept::RandomAccessIterator<T>::value << std::endl;
}

int main()
{
	std::vector<int> v;
	std::list<int> l;
	std::set<int>    st;
	std::stack<int>  sk;
	std::cout<<tc::concept::Clock<std::chrono::system_clock>::value;
	test(v.begin(), v.end());
//	test(l.begin(), l.end());
	return 0;
}
