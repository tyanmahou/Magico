#include<iostream>
#include<Magico.hpp>
#include<stack>
#include<forward_list>
using namespace magico;

MAGICO_CONCEPT(Test)
{
	template<class T>
	auto require(T t)->void;
};


int main()
{
	std::vector<int> v;
	std::list<int> l;
	concepts::UnsignedIntegral<unsigned int>::value;
	return 0;
}


