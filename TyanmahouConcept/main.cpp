#include<iostream>
#include"Concept.hpp"


//パターン1 構造体とメンバ関数　変数名を扱えるのがよい

struct _Addable
{
	template<class T , class U>
	auto requires(T&& a, U&& b)->decltype(
		a + b ,
		a - b ,
		a * b 
		);
};



//パターン2 エイリアステンプレート　declvalを使う表現が可読性ダウン？　行数は減る

template<class T, class U>
using _Addable2 = decltype(
	tc::val<T> + tc::val<U>,
	tc::val<T> - tc::val<U>, 
	tc::val<T> * tc::val<U>
	);



























//ああ
template<class T, class U = T>
using Addable2 = tc::alias_to_concept<_Addable2, T, U>;


struct A
{
	A operator +(const A& o)
	{
		return o;
	}
	A operator -(const A& o)
	{
		return o;
	}
};
int main()
{
	Addable2<int>::value;
	return 0;

}
