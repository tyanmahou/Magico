#pragma once
#include<exception>
#include"utility.hpp"
#include<string>
namespace tc
{
	namespace detail
	{

		template<class Concept, class... Args>
		using HasAxiom_impl = decltype(&Concept::template axiom<Args...>);

		template<class Concept, class... Args>
		using HasAxiom = tc::is_detected<HasAxiom_impl, Concept, Args...>;

		template<class Concept, class... Types>
		class axiom_check
		{
			using Constraint = typename Concept::constraint_t;
		public:
			template<class... Args>
			axiom_check(Args&&...args)
			{
				if constexpr(HasAxiom<Constraint, Types...>::value)
				{
					if constexpr(Concept::value)
					{
						if (!Constraint{}.axiom<Types...>(args...))
						{
							static std::string message = std::string(typeid(Concept).name()) += " dose't satisfy axion";
							throw tc::axiom_exception(message.c_str());
						}
					}
					else
					{
						static std::string message = std::string(typeid(Concept).name()) += " dose't satisfy require";
						throw tc::axiom_exception(message.c_str());
					}
				}
			}
		};
	}//namespace detail

	class axiom_exception : public std::exception
	{
	public:

		axiom_exception()noexcept :
			std::exception("no satisfy axiom", 1)
		{}
		axiom_exception(const char* const message) noexcept :
			std::exception(message, 1)
		{}
	};
}//namespace tc