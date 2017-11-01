#include<iostream>
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


//function
template<class T>
auto Func(T& a)->where<void,Stack<T>>
{
	a.push(10);
}

//class
template<class T>
struct Class
{
	MAGICO_CONCEPT_ASSERT(Stack<T>);
};
int main()
{
	std::vector<int> v;
	std::stack<int> s;

//	Func(v);  error
	Func(s);

//	Class<std::vector<int>> hogeV; error
	Class<std::stack<int>> hogeS;

	return 0;
}

//where_bool
template<class T>
auto Func2(T& a)->where_bool<void, Stack_v<T>>
{
	a.push(10);
}
//require
template<class T, require<Stack<T>> = nullptr>
void Func3(T& a)
{
	a.push(10);
}

