#include<iostream>

#include<Magico.hpp>

using namespace magico;


//test<T>
MAGICO_CONCEPT(Test)
{
	template<class T>
	auto require(T t)->decltype
	(
		magico::associated_type<typename T::iterator>()
	);
};
template<class T>
using TestOp = typename T::iterator;

#include<set>
int main()
{
	std::list<int> s;
	std::vector<int> v{11,22,33};

	std::cout<<magico::concepts::AssociativeContainer<std::multiset<int>>::value;


	return 0;
}


