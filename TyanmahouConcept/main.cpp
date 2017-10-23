#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>

#include"Concept.hpp"


int main()
{
	std::vector<int> v;
	std::list<int> l;

	std::unordered_map<std::string,int> m;

	int ar[] = { 1,2,3 };
	tc::concept::Range<decltype(ar)>::value;

	return 0;
}
