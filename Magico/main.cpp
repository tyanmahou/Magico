#include<iostream>
#include<Magico.hpp>
#include<stack>
#include<string>
//using namespace magico;

#include<map>
#include<set>

MAGICO_CONCEPT(Test)
{

	template<class T>
	auto _r(T a)->decltype(-a);

	template<class T>
	auto require(T a)->decltype(&__Test_c::template _r<T>);

};

int main()
{

	magico::concepts::SequenceContainer<int>::value;
	return 0;
}


