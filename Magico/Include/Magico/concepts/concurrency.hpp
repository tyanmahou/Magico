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
		MAGICO_CONCEPT(BasicLockable)
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
		MAGICO_CONCEPT(Lockable)
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
		MAGICO_CONCEPT(TimedLockable)
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
		MAGICO_CONCEPT(Mutex)
		{
			template<class Type>
			auto require()->decltype(
				extends<Lockable, DefaultConstructible, Destructible>::require<Type>(),
				magico::require<Not<CopyAssignable<Type>>, Not<MoveAssignable<Type>>>{}
			);
		};

		///<summary>
		///時限排他的なミューテックスか
		///</summary>
		MAGICO_CONCEPT(TimedMutex)
		{
			template<class Type>
			auto require()->decltype(
				extends<Mutex, TimedLockable>::require<Type>()
				);
		};

		///<summary>
		///共有ミューテックスか
		///</summary>
		MAGICO_CONCEPT(SharedMutex)
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
		MAGICO_CONCEPT(SharedTimedMutex)
		{
			template<class Type>
			auto require(Type m, std::chrono::seconds duration, std::chrono::system_clock::time_point time_limit)->decltype(
				extends<TimedMutex, SharedMutex>::require<Type>(),
				m.try_lock_shared_for(duration),
				m.try_lock_shared_until(time_limit)
				);
		};
	}//namespace concept
}//namespace magico
