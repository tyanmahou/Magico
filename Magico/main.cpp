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
MAGICO_CONCEPT_NON_DEFAULT(Stack)
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

	static_assert(Stack_v<as_mapped<std::stack<int>>> == false);

	//as_mappedを指定するとマッピング後で判定できる
	static_assert(Stack_v<as_mapped<std::vector<int>>> == true);


	//マッピング
	std::vector<int> v{ 1,2,3 };

	auto&&[_v] = concept_mapping<Stack>(v);//関数の型引数
	_v.push(4);

	auto&&_v2 = concept_mapping<Stack<std::vector<int>>>(v);


	//auto&&[_a,_b] = concept_mapping<HasPlus>(0,0.5f);
	//
	//auto&& _a2 = concept_mapping<HasPlus<int,float>>(a);
	//auto&& _b2 = concept_mapping<HasPlus<int,float>>(b);


	return 0;
}


