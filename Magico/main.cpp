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


int main()
{
	static_assert(Stack<std::vector<int>>::value == false);
	static_assert(Stack_v<std::stack<int>> == true);

	return 0;
}


