#pragma once
#include<exception>
#include"../utility.hpp"
#include<string>
namespace magico
{
	class axiom_exception : public std::exception
	{
	private:
		const char*const m_message;
	public:

		axiom_exception()noexcept :
			m_message("no satisfy axiom")
		{}
		axiom_exception(const char* const message) noexcept :
			m_message(message)
		{}
		const char*  what() const noexcept override
		{
			return m_message;
		}
	};

	namespace detail
	{

		template<class Concept, class... Args>
		using HasAxiom_impl = decltype(&Concept::template axiom<Args...>);

		template<class Concept, class... Args>
		using HasAxiom = magico::is_detected<HasAxiom_impl, Concept, Args...>;

		template<template<class...>class Concept, class... Types>
		class axiom_check
		{
			using _Concept = Concept<Types...>;

			using Constraint = typename _Concept::constraint_t;
		public:
			template<class... Args>
			axiom_check(Args&&...args)
			{
				static bool result = false;
				if (result)
					return;

				if constexpr(HasAxiom<Constraint, Types...>::value)
				{
					if constexpr(_Concept::value)
					{
						result = Constraint{}.template axiom<Types...>(args...);
						if (!result)
						{
							static const std::string message = std::string(typeid(_Concept).name()) += " dose't satisfy axion";
							throw magico::axiom_exception(message.c_str());
						}
					}
					else
					{
						static const std::string message = std::string(typeid(_Concept).name()) += " dose't satisfy require";
						throw magico::axiom_exception(message.c_str());
					}
				}
			}
		};
	}//namespace detail

}//namespace magico
