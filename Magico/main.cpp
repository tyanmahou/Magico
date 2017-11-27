#include<iostream>
#include<Magico.hpp>
#include<stack>
#include<string>
//using namespace magico;
#include<unordered_map>
#include<map>
#include<set>


MAGICO_CONCEPT(Test)
{
	template<class T>
	auto require(T t)->decltype(t);
};

MAGICO_CONCEPT_MAP_NONE_DEFAULT(Test);
namespace magico
{
	template<>
	struct concept_map<Test<int>>
	{
		static int& apply(int& a)
		{
			return a;
		}
	};
}


int main()
{
	Test<magico::as_mapped<int>>::value;

	return 0;
}


