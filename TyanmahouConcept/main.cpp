#include<iostream>
#include<vector>
#include<list>
#include"Concept.hpp"

template<class T>
using Drawble_c = tc::constraint<
	decltype(tc::val<T>.draw(), tc::val<T>.a)
>;

template<class T>
TC_TO_CONCEPT(Drawble, Drawble_c, T);

struct Square {};
struct Circle { int a = 10; void draw() { std::cout << "Circle" << std::endl;; } };

template<class T>
struct tc::concept_map<Drawble<T>>
{
	struct SWrap :Square
	{
		int a = 2;
		void draw()
		{
			std::cout << "Square" << std::endl;
		}
	};

	template<class U>
	struct VWrap :std::vector<U>
	{
		__declspec(property(get = getA, put = setA)) int a;
		int& getA()
		{
			return this->back();
		}
		void setA(int a)
		{
			this->back() = a;
		}
		void draw()
		{
			for (auto&& elm : *this)
			{
				std::cout << elm ;
			}
			std::cout << std::endl;
		}
	};


	SWrap& operator=(Square& s)
	{
		return static_cast<SWrap&>(s);
	}

	template<class U>
	VWrap<U>& operator=(std::vector<U>& v)
	{
		return static_cast<VWrap<U>&>(v);
	}
};

#include<functional>
namespace test
{
	struct D :tc::concept_any<Drawble>
	{

	
		template<class T,TC_WHERE(Drawble<T>)>
		D(T&& v):tc::concept_any<Drawble>(v),
			a(this->get<T>().a)
		{
			draw_impl = [&]() {this->get<T>().draw(); };
		}

		void draw()
		{
			draw_impl();
		}
		int& a;
	private:
		std::function<void()> draw_impl;
	};
}


void f(test::D draw)
{
	draw.draw();
	draw.a = 20;
}

void main()
{

	f(Square{});
	Circle c;
	f(c);
	std::cout << c.a << std::endl;
	std::vector<int> a{ 1,2,3 };
	f(a);

	std::cout << a[2];
}
