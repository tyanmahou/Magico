#include<iostream>
#include<vector>
#include<stack>
#include<list>
#include<unordered_map>
#include<memory>
#include<chrono>
#include<string>
#include<Concept.hpp>

using namespace tc;
template<class T>
TC_CONCEPT(Stack, T)
{
	template<class T>
	auto require(T t)->decltype(
		tc::associated_type<typename T::value_type>(),
		t.pop()
		);
};


template<class T>
struct tc::concept_map<Stack<std::vector<T>>>
	:std::vector<T>
{
	auto pop()
	{
		this->pop_back();
	}

	auto& operator=(std::vector<T>& a)
	{
		return static_cast<tc::concept_map<Stack<std::vector<T>>>&>(a);
	}
};

namespace test
{
	template<class It>
	auto advance(It it, priority<2>)
		->tc::where<void, tc::concept::RandomAccessIterator<It>>
	{
		std::cout << "Random";
	}
	template<class It>
	auto advance(It it, priority<1>)
		->tc::where<void, tc::concept::BidirectionalIterator<It>>
	{
		std::cout << "Bidirectional";

	}
	template<class It>
	auto advance(It it, priority<0>)
		->tc::where<void, tc::concept::ForwardIterator<It>>
	{
		std::cout << "Forward";

	}
	template<class It>
	auto advance(It it)->tc::where<void, tc::concept::ForwardIterator<It>>
	{
		test::advance(it, priority_v<2>);
	}
}
struct Test
{
	int a;
	Test(int) :a(0) {}

};


int main()
{
	std::list<int> s;
	std::vector<int> v;
	test::advance(s.begin());
	test::advance(v.begin());

	return 0;
}


