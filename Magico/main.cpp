#include<iostream>
#include<Magico.hpp>
#include<stack>
#include<string>
using namespace magico;

template<class T>
auto Println(const T& v)->where<void,concepts::OutputStreamable<T>>
{
	std::cout << v << std::endl;
}

struct A{};

std::ostream& operator <<(std::ostream& os, const A& a)
{
	os << "A";
	return os;
}

int main()
{
	std::string s = "a";
	A a;
	Println(a);
	Println(s);
	return 0;
}


