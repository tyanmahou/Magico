#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<set>
#include<stack>
#include<chrono>
#include<algorithm>
#include"Concept.hpp"



namespace tc
{
	template<class T>
	auto stable_sort(T& x)->tc::where<void,tc::concept::AllocatorAwareContainer<T>>
	{
		std::stable_sort(x.begin(), x.end());
	}


}
int main()
{
	std::vector<int> v;
	std::list<int> l;
	std::set<int>    st;
	std::stack<int>  sk;
	int a;
	tc::stable_sort(v);
	tc::stable_sort(l);
	return 0;
}
