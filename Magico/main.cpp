#include<iostream>
#include<Magico.hpp>
#include<stack>
#include<string>
//using namespace magico;

MAGICO_CONCEPT(Test)
{
private:
	template<class T, class U, class Ret>
	auto require_(T& a, U& b)->decltype(
		magico::valid_expr<Ret>(a + b)
		);
	template<class T, class U = T>
	auto require_(T& a, U& b)->decltype(
		a + b
		);
public:
	template<class T, class U = T, class... Ret>
	auto require(T&a, U& b)->decltype(
		this->require_<T, U, Ret...>(a, b)
		);
};

struct A {
	int operator+()
	{
		return 0;
	}
};
int main()
{
	int a;
	std::cout << magico::concepts::HasPlus<int, int&>::value;
	std::cout << magico::concepts::HasPlus<int, int&, int&>::value;
	std::cout << magico::concepts::HasPlus<int, int&, int>::value;

	return 0;
}


