﻿#include<iostream>
#include<Magico.hpp>
#include<stack>
using namespace magico;

//concept Stack<typename X>
//{
//	typename value_type;
//	void push(X&, const value_type&);
//	void pop(X&);
//	value_type top(const X&);
//	bool empty(const X&);
//};


///スタックコンセプト
MAGICO_CONCEPT(Stack)
{
	template<class X>
	using value_type = typename X::value_type;

	template<class X>
	auto require(X s, const X& cs, const value_type<X>& v)->decltype(
		valid_expr<void>((s.push(v), is_void)),
		valid_expr<void>((s.pop(), is_void)),
		valid_expr<value_type<X>>(cs.top()),
		valid_expr<bool>(cs.empty())
		);
};

template<class T>
struct concept_map<Stack<std::vector<T>>>:std::vector<T>
{
	using value_type = typename std::vector<T>::value_type;

	void push(const value_type& v)
	{
		this->emplace_back(v);
	}
	void pop()
	{
		this->pop_back(v);
	}
	value_type top()const
	{
		return this->back();
	}
	//vector has empty

	//operator = で割り当て
	decltype(auto) operator =(std::vector<T>& v)
	{
		return static_cast<concept_map&>(v);
	}
};

int main()
{

	static_assert(Stack<std::vector<int>>::value == false);

	//as_mappedを指定するとマッピング後で判定できる
	static_assert(Stack<as_mapped<std::vector<int>>>::value == true);

	//マッピング
	std::vector<int> v{ 1,2,3 };

	auto&&[_v] = concept_mapping<Stack>(v);

	_v.push(4);
	_v.top();

	auto&&_v2 = concept_mapping<Stack<std::vector<int>>>(v);

	return 0;
}




