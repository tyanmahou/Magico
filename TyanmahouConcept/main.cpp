#include<iostream>
#include<vector>
#include<stack>
#include<list>
#include<unordered_map>
#include<memory>
#include<chrono>

#include<Concept.hpp>


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

template<class It>
auto advance(It it)->tc::where<void, tc::concept::RandomAccessIterator<It>>
{

}
template<class It>
auto advance(It it)->tc::where<void, tc::concept::BidirectionalIterator<It>,
	tc::Not<tc::concept::RandomAccessIterator<It>>>
{

}
template<class It>
auto advance(It it)->tc::where<void, tc::concept::ForwardIterator<It>,
	tc::Not<tc::concept::BidirectionalIterator<It>>>
{

}
int main()
{
	int a;
	std::list<int> s;
	std::vector<int> v;

	advance(s.begin());
	advance(v.begin());

	return 0;
}


