#pragma once
#include"../concept.hpp"
#include"basic.hpp"
#include<chrono>
namespace magico {
	namespace concepts {

		//************************************************************************************************
		//
		//Concurrency
		//
		//************************************************************************************************

		///<summary>
		///スレッドの排他制御を提供するか
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(BasicLockable, Type)
		{
			template<class Type>
			auto require(Type m)->decltype(
				m.lock(),
				m.unlock()
				);
		};

		///<summary>
		///試行されたロックをサポートする排他制御を提供するか
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(Lockable, Type)
		{
			template<class Type>
			auto require(Type m)->decltype(
				extends<BasicLockable>::require<Type>(),
				m.try_lock()
				);
		};

		///<summary>
		///時限排他的な制御を提供するか
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(TimedLockable, Type)
		{
			template<class Type>
			auto require(Type m, std::chrono::seconds duration, std::chrono::system_clock::time_point time_limit)->decltype(
				extends<Lockable>::require<Type>(),
				m.try_lock_for(duration),
				m.try_lock_until(time_limit)
				);
		};

		///<summary>
		///ミューテックスか
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(Mutex, Type)
		{
			template<class Type>
			auto require()->decltype(
				extends<Lockable,DefaultConstructible,Destructible>::require<Type>(),
				magico::require<Not<CopyAssignable<Type>>, Not<MoveAssignable<Type>>>{}
				);
		};

		///<summary>
		///時限排他的なミューテックスか
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(TimedMutex, Type)
		{
			template<class Type>
			auto require()->decltype(
				extends<Mutex,TimedLockable>::require<Type>()
				);
		};

		///<summary>
		///共有ミューテックスか
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(SharedMutex, Type)
		{
			template<class Type>
			auto require(Type m)->decltype(
				extends<Mutex>::require<Type>(),
				m.lock_shared(),
				m.try_lock_shared(),
				m.unlock_shared()
			);
		};

		///<summary>
		///時限排他的な共有ミューテックスか
		///</summary>
		template<class Type>
		MAGICO_CONCEPT(SharedTimedMutex, Type)
		{
			template<class Type>
			auto require(Type m, std::chrono::seconds duration, std::chrono::system_clock::time_point time_limit)->decltype(
				extends<TimedMutex,SharedMutex>::require<Type>(),
				m.try_lock_shared_for(duration),
				m.try_lock_shared_until(time_limit)
				);
		};
	}//namespace concept
}//namespace magico
