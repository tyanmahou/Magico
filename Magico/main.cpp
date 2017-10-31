#include<iostream>
#include<vector>
#include<stack>
#include<list>
#include<unordered_map>
#include<map>
#include<set>
#include<unordered_set>

#include<memory>
#include<chrono>
#include<string>
#include<Magico.hpp>


template<class T>
TC_CONCEPT(Test, T)
{
	template<class T>
	auto require(T t)->decltype(t.begain())
	{
	}
};
int main()
{
	std::list<int> s;
	std::vector<int> v;


	std::set<std::string>::value_compare;
	return 0;
}


