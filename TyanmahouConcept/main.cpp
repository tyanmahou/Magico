#include<iostream>
#include"Concept.hpp"

template<class T,TC_WHERE(tc::Concept::LessThanComparable<T>)>
bool Test(T&& a, T&& b)
{
	return a < b;
}

void main()
{
	struct A {};
	A a;

	Test(0, 0);

	static_assert(tc::Concept::LessThanComparable<A>::value==false,"A has not operator <");
}