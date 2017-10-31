#pragma once

namespace magico
{
	namespace detail
	{
		template<unsigned N>
		struct priority_impl :priority_impl<N - 1>
		{};

		template<>
		struct priority_impl<0>
		{};
	}

	///<summary>
	///オーバーロード優先度クラス
	///</summary>
	template<unsigned N>
	using priority = detail::priority_impl<N>*;

	///<summary>
	///オーバーロード優先度　オブジェクト
	///</summary>
	template<unsigned N>
	constexpr priority<N> priority_v = nullptr;


}//namespace magico
