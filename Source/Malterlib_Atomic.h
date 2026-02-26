// Copyright © 2015 Hansoft AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Core/Platform>
#include <Mib/Type/Traits>

#include <atomic>

namespace NMib::NAtomic
{
	/// Memory order enum
	/**

	 */
	using CMemoryOrder = std::memory_order;

	static constexpr auto gc_MemoryOrder_Relaxed = std::memory_order_relaxed; ///< Relaxed ordering
	static constexpr auto gc_MemoryOrder_Consume = std::memory_order_consume; ///< Consume ordering
	static constexpr auto gc_MemoryOrder_Acquire = std::memory_order_acquire; ///< Acquire ordering
	static constexpr auto gc_MemoryOrder_Release = std::memory_order_release;	///< Release ordering
	static constexpr auto gc_MemoryOrder_AcquireRelease = std::memory_order_acq_rel;	///< Acquire release ordering
	static constexpr auto gc_MemoryOrder_SequentiallyConsistent = std::memory_order_seq_cst;	///< Sequentially consistent ordering

	template <typename t_CType>
	struct TCAtomicIndeterminate
	{
		constexpr TCAtomicIndeterminate(TCAtomicIndeterminate const &_Source) noexcept = default;
		constexpr TCAtomicIndeterminate &operator = (TCAtomicIndeterminate const &_Source) noexcept = default;
		constexpr TCAtomicIndeterminate() noexcept = default;
		constexpr TCAtomicIndeterminate(t_CType _Value) noexcept;

		bool f_IsLockFree() const noexcept;
		bool f_IsLockFree() const volatile noexcept;

		constexpr void f_Store(t_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		constexpr void f_Store(t_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		constexpr t_CType f_Load(CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) const noexcept;
		constexpr t_CType f_Load(CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) const volatile noexcept;

		t_CType f_Exchange(t_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		t_CType f_Exchange(t_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, CMemoryOrder _SuccessOrder, CMemoryOrder _FailureOrder) noexcept;
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, CMemoryOrder _SuccessOrder, CMemoryOrder _FailureOrder ) volatile noexcept;
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, CMemoryOrder _SuccessOrder, CMemoryOrder _FailureOrder) noexcept;
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, CMemoryOrder _SuccessOrder, CMemoryOrder _FailureOrder ) volatile noexcept;
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		template <typename tf_CType>
		t_CType f_FetchAdd(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		template <typename tf_CType>
		t_CType f_FetchAdd(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		template <typename tf_CType>
		t_CType f_FetchSub(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		template <typename tf_CType>
		t_CType f_FetchSub(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		template <typename tf_CType>
		t_CType f_FetchAnd(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		template <typename tf_CType>
		t_CType f_FetchAnd(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		template <typename tf_CType>
		t_CType f_FetchOr(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		template <typename tf_CType>
		t_CType f_FetchOr(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		template <typename tf_CType>
		t_CType f_FetchXor(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
		template <typename tf_CType>
		t_CType f_FetchXor(tf_CType _Value, CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;

		t_CType &f_NonAtomic();
		t_CType const &f_NonAtomic() const;

		template <typename tf_CFormatter>
		auto f_CreateStringFormatter(tf_CFormatter &_Formatter) const;

		inline_always t_CType operator = (t_CType _Value) volatile noexcept;
		inline_always t_CType operator = (t_CType _Value) noexcept;

		constexpr operator t_CType () const noexcept;
		constexpr operator t_CType () const volatile noexcept;

		t_CType operator ++ () noexcept;
		t_CType operator ++ () volatile noexcept;

		t_CType operator ++ (int) noexcept;
		t_CType operator ++ (int) volatile noexcept;

		t_CType operator -- () noexcept;
		t_CType operator -- () volatile noexcept;

		t_CType operator -- (int) noexcept;
		t_CType operator -- (int) volatile noexcept;

		template <typename tf_CType>
		t_CType operator += (tf_CType _Value) noexcept;
		template <typename tf_CType>
		t_CType operator += (tf_CType _Value) volatile noexcept;
		template <typename tf_CType>
		t_CType operator -= (tf_CType _Value) noexcept;
		template <typename tf_CType>
		t_CType operator -= (tf_CType _Value) volatile noexcept;
		template <typename tf_CType>
		t_CType operator &= (tf_CType _Value) noexcept;
		template <typename tf_CType>
		t_CType operator &= (tf_CType _Value) volatile noexcept;
		template <typename tf_CType>
		t_CType operator |= (tf_CType _Value) noexcept;
		template <typename tf_CType>
		t_CType operator |= (tf_CType _Value) volatile noexcept;
		template <typename tf_CType>
		t_CType operator ^= (tf_CType _Value) noexcept;
		template <typename tf_CType>
		t_CType operator ^= (tf_CType _Value) volatile noexcept;

		static constexpr bool mc_bIsAlwaysLockFree = std::atomic_ref<t_CType>::is_always_lock_free;

		alignas(std::atomic_ref<t_CType>::required_alignment) t_CType m_Value;
	};

	template <typename t_CType>
	struct TCAtomic : public TCAtomicIndeterminate<t_CType>
	{
		using TCAtomicIndeterminate<t_CType>::TCAtomicIndeterminate;
		using TCAtomicIndeterminate<t_CType>::operator =;

		constexpr TCAtomic() noexcept(NTraits::cIsNothrowDefaultConstructible<t_CType>);

		consteval TCAtomic(TCAtomic const &_Source) noexcept = default;
		constexpr TCAtomic(TCAtomicIndeterminate<t_CType> const &_Source) noexcept = delete;

		consteval TCAtomic &operator = (TCAtomic const &_Source) noexcept = default;
		constexpr TCAtomic &operator = (TCAtomicIndeterminate<t_CType> const &_Source) noexcept = delete;
	};

	class CAtomicFlag : protected std::atomic_flag
	{
		using CSuper = std::atomic_flag;
	public:
		constexpr inline_always CAtomicFlag() noexcept;

		inline_always bool f_TestAndSet(CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;
		inline_always bool f_TestAndSet(CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;

		inline_always void f_Clear(CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) volatile noexcept;
		inline_always void f_Clear(CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
	};

	template <typename tf_CType>
	inline_always tf_CType fg_KillDependency(tf_CType _Value) noexcept;

	inline_always void fg_MemoryFence(CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
	inline_always void fg_CompilerFence(CMemoryOrder _Order = gc_MemoryOrder_SequentiallyConsistent) noexcept;
}

#ifndef DMibPNoShortCuts
		using namespace NMib::NAtomic;
#endif

#include "Malterlib_Atomic.hpp"
