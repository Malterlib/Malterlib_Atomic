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
	using EMemoryOrder = std::memory_order;

	static constexpr auto EMemoryOrder_Relaxed = std::memory_order_relaxed; ///< Relaxed ordering
	static constexpr auto EMemoryOrder_Consume = std::memory_order_consume; ///< Consume ordering
	static constexpr auto EMemoryOrder_Acquire = std::memory_order_acquire; ///< Acquire ordering
	static constexpr auto EMemoryOrder_Release = std::memory_order_release;	///< Release ordering
	static constexpr auto EMemoryOrder_AcquireRelease = std::memory_order_acq_rel;	///< Acquire release ordering
	static constexpr auto EMemoryOrder_SequentiallyConsistent = std::memory_order_seq_cst;	///< Sequentially consistent ordering

	template <typename t_CType>
	struct TCAtomicIndeterminate
	{
		constexpr TCAtomicIndeterminate(TCAtomicIndeterminate const &_Source) noexcept = default;
		constexpr TCAtomicIndeterminate &operator = (TCAtomicIndeterminate const &_Source) noexcept = default;
		constexpr TCAtomicIndeterminate() noexcept = default;

		constexpr TCAtomicIndeterminate(t_CType _Value) noexcept
			: m_Value(_Value)
		{
		}

		inline_always t_CType operator = (t_CType _Value) volatile noexcept
		{
			std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).store(_Value);
			return _Value;
		}

		inline_always t_CType operator = (t_CType _Value) noexcept
		{
			std::atomic_ref<t_CType>(m_Value).store(_Value);
			return _Value;
		}

		bool f_IsLockFree() const noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).is_lock_free();
		}
		bool f_IsLockFree() const volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).is_lock_free();
		}

		constexpr void f_Store(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			std::atomic_ref<t_CType>(m_Value).store(_Value, _Order);
		}
		constexpr void f_Store(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).store(_Value, _Order);
		}

		constexpr t_CType f_Load(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) const noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).load(_Order);
		}
		constexpr t_CType f_Load(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) const volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).load(_Order);
		}

		constexpr operator t_CType () const noexcept
		{
			return f_Load();
		}
		constexpr operator t_CType () const volatile noexcept
		{
			return f_Load();
		}

		t_CType f_Exchange(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).exchange(_Value, _Order);
		}
		t_CType f_Exchange(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).exchange(_Value, _Order);
		}

		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).compare_exchange_weak(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder ) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).compare_exchange_weak(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).compare_exchange_weak(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).compare_exchange_weak(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).compare_exchange_strong(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder ) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).compare_exchange_strong(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).compare_exchange_strong(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).compare_exchange_strong(_Expected, _Desired, _Order);
		}

		// Fetch add
		template <typename tf_CType>
		t_CType f_FetchAdd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_add(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchAdd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_add(_Value, _Order);
		}

		// Fetch sub
		template <typename tf_CType>
		t_CType f_FetchSub(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_sub(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchSub(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_sub(_Value, _Order);
		}

		// Fetch and
		template <typename tf_CType>
		t_CType f_FetchAnd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_and(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchAnd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_and(_Value, _Order);
		}

		// Fetch or
		template <typename tf_CType>
		t_CType f_FetchOr(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_or(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchOr(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_or(_Value, _Order);
		}

		// Fetch xor
		template <typename tf_CType>
		t_CType f_FetchXor(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_xor(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchXor(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_xor(_Value, _Order);
		}

		// Operators

		t_CType operator ++ () noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_add(1)+1;
		}
		t_CType operator ++ () volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_add(1)+1;
		}

		t_CType operator ++ (int) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_add(1);
		}
		t_CType operator ++ (int) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_add(1);
		}

		t_CType operator -- () noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_sub(1)-1;
		}
		t_CType operator -- () volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_sub(1)-1;
		}

		t_CType operator -- (int) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_sub(1);
		}
		t_CType operator -- (int) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_sub(1);
		}

		template <typename tf_CType>
		t_CType operator += (tf_CType _Value) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_add(_Value);
		}
		template <typename tf_CType>
		t_CType operator += (tf_CType _Value) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_add(_Value);
		}
		template <typename tf_CType>
		t_CType operator -= (tf_CType _Value) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_sub(_Value);
		}
		template <typename tf_CType>
		t_CType operator -= (tf_CType _Value) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_sub(_Value);
		}
		template <typename tf_CType>
		t_CType operator &= (tf_CType _Value) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_and(_Value);
		}
		template <typename tf_CType>
		t_CType operator &= (tf_CType _Value) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_and(_Value);
		}
		template <typename tf_CType>
		t_CType operator |= (tf_CType _Value) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_or(_Value);
		}
		template <typename tf_CType>
		t_CType operator |= (tf_CType _Value) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_or(_Value);
		}
		template <typename tf_CType>
		t_CType operator ^= (tf_CType _Value) noexcept
		{
			return std::atomic_ref<t_CType>(m_Value).fetch_xor(_Value);
		}
		template <typename tf_CType>
		t_CType operator ^= (tf_CType _Value) volatile noexcept
		{
			return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_xor(_Value);
		}

		t_CType &f_NonAtomic()
		{
			return m_Value;
		}

		t_CType const &f_NonAtomic() const
		{
			return m_Value;
		}

		template <typename tf_CFormatter>
		auto f_CreateStringFormatter(tf_CFormatter &_Formatter) const
		{
			return fg_CreateStringFormatter(_Formatter, this->f_Load());
		}

		static constexpr bool mc_bIsAlwaysLockFree = std::atomic_ref<t_CType>::is_always_lock_free;

		alignas(std::atomic_ref<t_CType>::required_alignment) t_CType m_Value;
	};

	template <typename t_CType>
	struct TCAtomic : public TCAtomicIndeterminate<t_CType>
	{
		using TCAtomicIndeterminate<t_CType>::TCAtomicIndeterminate;
		using TCAtomicIndeterminate<t_CType>::operator =;

		consteval TCAtomic(TCAtomic const &_Source) noexcept = default;
		constexpr TCAtomic(TCAtomicIndeterminate<t_CType> const &_Source) noexcept = delete;

		constexpr TCAtomic() noexcept(NTraits::cIsNothrowDefaultConstructible<t_CType>)
			: TCAtomicIndeterminate<t_CType>(t_CType())
		{
		}

		consteval TCAtomic &operator = (TCAtomic const &_Source) noexcept = default;
		constexpr TCAtomic &operator = (TCAtomicIndeterminate<t_CType> const &_Source) noexcept = delete;
	};

	class CAtomicFlag : protected std::atomic_flag
	{
		using CSuper = std::atomic_flag;
	public:
		constexpr inline_always CAtomicFlag() noexcept
			: CSuper{}
		{
		} // = default;

		inline_always bool f_TestAndSet(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_flag::test_and_set(_Order);
		}

		inline_always bool f_TestAndSet(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_flag::test_and_set(_Order);
		}
		inline_always void f_Clear(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return std::atomic_flag::clear(_Order);
		}
		inline_always void f_Clear(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return std::atomic_flag::clear(_Order);
		}
	};

	template <typename tf_CType>
	inline_always tf_CType fg_KillDependency(tf_CType _Value) noexcept
	{
		return std::kill_dependency(_Value);
	}

	inline_always void fg_MemoryFence(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
	{
		return std::atomic_thread_fence(_Order);
	}

	inline_always void fg_CompilerFence(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
	{
		return std::atomic_signal_fence(_Order);
	}
}

#ifndef DMibPNoShortCuts
		using namespace NMib::NAtomic;
#endif
