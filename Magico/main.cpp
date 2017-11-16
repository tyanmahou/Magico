#include<iostream>
#include<Magico.hpp>
#include<stack>
#include<string>
//using namespace magico;
#include<unordered_map>
#include<map>
#include<set>

template<class T>
T clone(const T& a)
{
	return a;
}

template<class T>
auto Test(const T& a)
{
	return magico::concept_map<magico::concepts::HasSubscript<T,std::string>>()=a;
}
template<class T>
using AAAA = magico::concepts::HasSubscript<T, std::string>;
template<class >
struct A{};
decltype(auto) aaaa(magico::concept_any<AAAA>& a)
{
	using T = std::unordered_map<std::string, std::size_t>;
	return magico::concept_map<magico::concepts::HasSubscript<T, std::string>>() = a.get_origin<T>();
}
int main()
{

	magico::concept_any<AAAA> hs;
	std::vector<int> v;
	std::unordered_map<std::string, std::size_t> map
	{
		{ "A",334 }
	};
	std::size_t s;
	hs =map;

	auto a = Test(map);
	//auto&&[a, b] =  magico::concept_mapping<magico::concepts::HasSubscript>(map, "a");
	std::cout<<a.at("A");
	auto&& ref = hs.get<std::unordered_map<std::string, std::size_t>&>();
	ref["B"] = 10;
	std::cout << hs.get<std::unordered_map<std::string, std::size_t>>().at("B");
	return 0;
}


