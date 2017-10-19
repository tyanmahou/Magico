#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<set>
#include<stack>
#include<chrono>
#include"Concept.hpp"

struct _HasAdd
{
	template<class T>
	auto requires(T&& t, typename T::value_type v)->decltype(
		t.add(v)
		);
};
template<class T>
using HasAdd = tc::to_concept<_HasAdd, T>;

template<class T>
using HasAdd = tc::to_concept<_HasAdd, T>;

template<class T>
struct tc::concept_map<HasAdd<std::vector<T>>>
{
	struct W :protected std::vector<T>
	{
		using value_type = typename std::vector<T>::value_type;

		void add(value_type v)
		{
			this->emplace_back(v);
		}
	};
	W& operator =(std::vector<T>& v)
	{
		return static_cast<W&>(v);
	}
};
template<class T>
struct tc::concept_map<HasAdd<std::stack<T>>>
{
	struct W :protected std::stack<T>
	{
		using value_type = typename std::stack<T>::value_type;

		void add(value_type v)
		{
			this->push(v);
		}
	};
	W& operator =(std::stack<T>& v)
	{
		return static_cast<W&>(v);
	}
};
template<class T>
struct tc::concept_map<HasAdd<std::set<T>>>
{
	struct W :protected std::set<T>
	{
		using value_type = typename std::set<T>::value_type;

		void add(value_type v)
		{
			this->insert(v);
		}
	};
	W& operator =(std::set<T>& v)
	{
		return static_cast<W&>(v);
	}
};




template<class T, class V>
auto add(T& c, V v)->tc::where<void,HasAdd<T>>
{
	std::cout << "aaa" << std::endl;
	c.add(v);
}

struct TestA
{
	using value_type = int;

	void add(int a)
	{}
};

int main()
{
	std::vector<int> v;
	std::list<int> l;
	std::set<int>    st;
	std::stack<int>  sk;
	TestA ta;


	HasAdd<tc::as_mapped<std::vector<int>>>::value;
	add(v, 0); // v.push_back(3);
	add(st, 1); // st.insert(1);
	add(sk, 4); // sk.push(4);
	add(ta, 0);

	struct S { int a() { return 11; } };
	struct D :  S 
	{
		using S::a;
	};
	S s;
	D& ss = static_cast<D&>(s);
	std::cout<<ss.a();

	return 0;
}
