#include<iostream>
#include<vector>

#include"Concept.hpp"

struct Test{};

template<>
struct tc::concept_map<tc::Concept::Abstract<Test>>
{
	struct Wrap:Test
	{
		virtual void a() = 0;
	};

	Wrap& operator=(Test& a)
	{
		return static_cast<Wrap&>(a);
	}

};

void main()
{
	static_assert(std::is_abstract<Test>::value==false);
	static_assert(tc::Concept::Abstract<Test>::value==true);
}
