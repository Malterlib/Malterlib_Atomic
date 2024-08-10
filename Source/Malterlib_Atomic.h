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
	struct TCAtomicAggregate
	{
		inline_always std::atomic<t_CType> &fp_Atomic()
		{
			return *(reinterpret_cast<std::atomic<t_CType> *>(&m_Storage));
		}
		inline_always std::atomic<t_CType> volatile &fp_Atomic() volatile
		{
			return *(reinterpret_cast<std::atomic<t_CType> volatile *>(&m_Storage));
		}

		inline_always std::atomic<t_CType> const &fp_Atomic() const
		{
			return *(reinterpret_cast<std::atomic<t_CType> const *>(&m_Storage));
		}
		inline_always std::atomic<t_CType> const volatile &fp_Atomic() const volatile
		{
			return *(reinterpret_cast<std::atomic<t_CType> const volatile *>(&m_Storage));
		}

		typedef typename NMib::NTraits::TCUnsigned<typename NMib::NTraits::TCIntFromSize<sizeof(std::atomic<t_CType>)>::CType>::CType CStorage;

		static constexpr bool mc_bIsAlwaysLockFree = std::atomic<t_CType>::is_always_lock_free;
	public:
		CStorage m_Storage;

		inline_always t_CType operator = (t_CType _Value) volatile noexcept
		{
			fp_Atomic().store(_Value);
			return _Value;
		}

		inline_always t_CType operator = (t_CType _Value) noexcept
		{
			fp_Atomic().store(_Value);
			return _Value;
		}

		bool f_IsLockFree() const noexcept
		{
			return fp_Atomic().is_lock_free();
		}
		bool f_IsLockFree() const volatile noexcept
		{
			return fp_Atomic().is_lock_free();
		}

		void f_Store(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().store(_Value, _Order);
		}
		void f_Store(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().store(_Value, _Order);
		}

		t_CType f_Load(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) const noexcept
		{
			return fp_Atomic().load(_Order);
		}
		t_CType f_Load(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) const volatile noexcept
		{
			return fp_Atomic().load(_Order);
		}

		operator t_CType () const
		{
			return f_Load();
		}
		operator t_CType () const volatile
		{
			return f_Load();
		}

		t_CType f_Exchange(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().exchange(_Value, _Order);
		}
		t_CType f_Exchange(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().exchange(_Value, _Order);
		}

		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder) noexcept
		{
			return fp_Atomic().compare_exchange_weak(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder ) volatile noexcept
		{
			return fp_Atomic().compare_exchange_weak(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().compare_exchange_weak(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().compare_exchange_weak(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder) noexcept
		{
			return fp_Atomic().compare_exchange_strong(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder ) volatile noexcept
		{
			return fp_Atomic().compare_exchange_strong(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().compare_exchange_strong(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().compare_exchange_strong(_Expected, _Desired, _Order);
		}

		// Fetch add
		template <typename tf_CType>
		t_CType f_FetchAdd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().fetch_add(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchAdd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().fetch_add(_Value, _Order);
		}

		// Fetch sub
		template <typename tf_CType>
		t_CType f_FetchSub(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().fetch_sub(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchSub(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().fetch_sub(_Value, _Order);
		}

		// Fetch and
		template <typename tf_CType>
		t_CType f_FetchAnd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().fetch_and(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchAnd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().fetch_and(_Value, _Order);
		}

		// Fetch or
		template <typename tf_CType>
		t_CType f_FetchOr(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().fetch_or(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchOr(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().fetch_or(_Value, _Order);
		}

		// Fetch xor
		template <typename tf_CType>
		t_CType f_FetchXor(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().fetch_xor(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchXor(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().fetch_xor(_Value, _Order);
		}

		// Operators

		t_CType operator ++ () noexcept
		{
			return fp_Atomic().fetch_add(1)+1;
		}
		t_CType operator ++ () volatile noexcept
		{
			return fp_Atomic().fetch_add(1)+1;
		}

		t_CType operator ++ (int) noexcept
		{
			return fp_Atomic().fetch_add(1);
		}
		t_CType operator ++ (int) volatile noexcept
		{
			return fp_Atomic().fetch_add(1);
		}

		t_CType operator -- () noexcept
		{
			return fp_Atomic().fetch_sub(1)-1;
		}
		t_CType operator -- () volatile noexcept
		{
			return fp_Atomic().fetch_sub(1)-1;
		}

		t_CType operator -- (int) noexcept
		{
			return fp_Atomic().fetch_sub(1);
		}
		t_CType operator -- (int) volatile noexcept
		{
			return fp_Atomic().fetch_sub(1);
		}

		template <typename tf_CType>
		t_CType operator += (tf_CType _Value) noexcept
		{
			return fp_Atomic().fetch_add(_Value);
		}
		template <typename tf_CType>
		t_CType operator += (tf_CType _Value) volatile noexcept
		{
			return fp_Atomic().fetch_add(_Value);
		}
		template <typename tf_CType>
		t_CType operator -= (tf_CType _Value) noexcept
		{
			return fp_Atomic().fetch_sub(_Value);
		}
		template <typename tf_CType>
		t_CType operator -= (tf_CType _Value) volatile noexcept
		{
			return fp_Atomic().fetch_sub(_Value);
		}
		template <typename tf_CType>
		t_CType operator &= (tf_CType _Value) noexcept
		{
			return fp_Atomic().fetch_and(_Value);
		}
		template <typename tf_CType>
		t_CType operator &= (tf_CType _Value) volatile noexcept
		{
			return fp_Atomic().fetch_and(_Value);
		}
		template <typename tf_CType>
		t_CType operator |= (tf_CType _Value) noexcept
		{
			return fp_Atomic().fetch_or(_Value);
		}
		template <typename tf_CType>
		t_CType operator |= (tf_CType _Value) volatile noexcept
		{
			return fp_Atomic().fetch_or(_Value);
		}
		template <typename tf_CType>
		t_CType operator ^= (tf_CType _Value) noexcept
		{
			return fp_Atomic().fetch_xor(_Value);
		}
		template <typename tf_CType>
		t_CType operator ^= (tf_CType _Value) volatile noexcept
		{
			return fp_Atomic().fetch_xor(_Value);
		}

	};

	template <typename t_CType>
	class TCAtomic : std::atomic<t_CType>
	{
		TCAtomic(TCAtomic const &);
		TCAtomic &operator = (TCAtomic const &);
	public:

		static constexpr bool mc_bIsAlwaysLockFree = std::atomic<t_CType>::is_always_lock_free;

		typedef std::atomic<t_CType> CSuper;

		constexpr TCAtomic() noexcept // = default;
			: CSuper(0)
		{
		}

		constexpr TCAtomic(t_CType _Value) noexcept
			: CSuper(_Value)
		{
		}

		inline_always t_CType operator = (t_CType _Value) volatile noexcept
		{
			CSuper::store(_Value);
			return _Value;
		}

		inline_always t_CType operator = (t_CType _Value) noexcept
		{
			CSuper::store(_Value);
			return _Value;
		}

		bool f_IsLockFree() const noexcept
		{
			return CSuper::is_lock_free();
		}
		bool f_IsLockFree() const volatile noexcept
		{
			return CSuper::is_lock_free();
		}

		void f_Store(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::store(_Value, _Order);
		}
		void f_Store(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::store(_Value, _Order);
		}

		t_CType f_Load(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) const noexcept
		{
			return CSuper::load(_Order);
		}
		t_CType f_Load(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) const volatile noexcept
		{
			return CSuper::load(_Order);
		}

		operator t_CType () const noexcept
		{
			return f_Load();
		}
		operator t_CType () const volatile noexcept
		{
			return f_Load();
		}

		t_CType f_Exchange(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::exchange(_Value, _Order);
		}
		t_CType f_Exchange(t_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::exchange(_Value, _Order);
		}

		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder) noexcept
		{
			return CSuper::compare_exchange_weak(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder ) volatile noexcept
		{
			return CSuper::compare_exchange_weak(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::compare_exchange_weak(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::compare_exchange_weak(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder) noexcept
		{
			return CSuper::compare_exchange_strong(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder ) volatile noexcept
		{
			return CSuper::compare_exchange_strong(_Expected, _Desired, _SuccessOrder, _FailureOrder);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::compare_exchange_strong(_Expected, _Desired, _Order);
		}
		bool f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::compare_exchange_strong(_Expected, _Desired, _Order);
		}

		// Fetch add
		template <typename tf_CType>
		t_CType f_FetchAdd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::fetch_add(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchAdd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::fetch_add(_Value, _Order);
		}

		// Fetch sub
		template <typename tf_CType>
		t_CType f_FetchSub(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::fetch_sub(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchSub(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::fetch_sub(_Value, _Order);
		}

		// Fetch and
		template <typename tf_CType>
		t_CType f_FetchAnd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::fetch_and(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchAnd(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::fetch_and(_Value, _Order);
		}

		// Fetch or
		template <typename tf_CType>
		t_CType f_FetchOr(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::fetch_or(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchOr(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::fetch_or(_Value, _Order);
		}

		// Fetch xor
		template <typename tf_CType>
		t_CType f_FetchXor(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return CSuper::fetch_xor(_Value, _Order);
		}

		template <typename tf_CType>
		t_CType f_FetchXor(tf_CType _Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return CSuper::fetch_xor(_Value, _Order);
		}

		// Operators

		t_CType operator ++ () noexcept
		{
			return CSuper::fetch_add(1)+1;
		}
		t_CType operator ++ () volatile noexcept
		{
			return CSuper::fetch_add(1)+1;
		}

		t_CType operator ++ (int) noexcept
		{
			return CSuper::fetch_add(1);
		}
		t_CType operator ++ (int) volatile noexcept
		{
			return CSuper::fetch_add(1);
		}

		t_CType operator -- () noexcept
		{
			return CSuper::fetch_sub(1)-1;
		}
		t_CType operator -- () volatile noexcept
		{
			return CSuper::fetch_sub(1)-1;
		}

		t_CType operator -- (int) noexcept
		{
			return CSuper::fetch_sub(1);
		}
		t_CType operator -- (int) volatile noexcept
		{
			return CSuper::fetch_sub(1);
		}

		template <typename tf_CType>
		t_CType operator += (tf_CType _Value) noexcept
		{
			return CSuper::fetch_add(_Value);
		}
		template <typename tf_CType>
		t_CType operator += (tf_CType _Value) volatile noexcept
		{
			return CSuper::fetch_add(_Value);
		}
		template <typename tf_CType>
		t_CType operator -= (tf_CType _Value) noexcept
		{
			return CSuper::fetch_sub(_Value);
		}
		template <typename tf_CType>
		t_CType operator -= (tf_CType _Value) volatile noexcept
		{
			return CSuper::fetch_sub(_Value);
		}
		template <typename tf_CType>
		t_CType operator &= (tf_CType _Value) noexcept
		{
			return CSuper::fetch_and(_Value);
		}
		template <typename tf_CType>
		t_CType operator &= (tf_CType _Value) volatile noexcept
		{
			return CSuper::fetch_and(_Value);
		}
		template <typename tf_CType>
		t_CType operator |= (tf_CType _Value) noexcept
		{
			return CSuper::fetch_or(_Value);
		}
		template <typename tf_CType>
		t_CType operator |= (tf_CType _Value) volatile noexcept
		{
			return CSuper::fetch_or(_Value);
		}
		template <typename tf_CType>
		t_CType operator ^= (tf_CType _Value) noexcept
		{
			return CSuper::fetch_xor(_Value);
		}
		template <typename tf_CType>
		t_CType operator ^= (tf_CType _Value) volatile noexcept
		{
			return CSuper::fetch_xor(_Value);
		}

		template <typename tf_CFormatter>
		auto f_CreateStringFormatter(tf_CFormatter &_Formatter) const
		{
			return fg_CreateStringFormatter(_Formatter, this->f_Load());
		}
	};

	class CAtomicFlagAggregate
	{
		inline_always std::atomic_flag &fp_Atomic()
		{
			return *(reinterpret_cast<std::atomic_flag *>(&m_Storage));
		}
		inline_always std::atomic_flag volatile &fp_Atomic() volatile
		{
			return *(reinterpret_cast<std::atomic_flag volatile *>(&m_Storage));
		}

		inline_always std::atomic_flag const &fp_Atomic() const
		{
			return *(reinterpret_cast<std::atomic_flag const *>(&m_Storage));
		}
		inline_always std::atomic_flag const volatile &fp_Atomic() const volatile
		{
			return *(reinterpret_cast<std::atomic_flag const volatile *>(&m_Storage));
		}

		typedef NMib::NTraits::TCUnsigned<NMib::NTraits::TCIntFromSize<sizeof(std::atomic_flag)>::CType>::CType CStorage;
	public:
		CStorage m_Storage;

		inline_always bool f_TestAndSet(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().test_and_set(_Order);
		}

		inline_always bool f_TestAndSet(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().test_and_set(_Order);
		}
		inline_always void f_Clear(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) volatile noexcept
		{
			return fp_Atomic().clear(_Order);
		}
		inline_always void f_Clear(EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
		{
			return fp_Atomic().clear(_Order);
		}

	};

	class CAtomicFlag : protected std::atomic_flag
	{
		typedef std::atomic_flag CSuper;
	public:
		inline_always CAtomicFlag() noexcept
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

	// Functions
	template <typename tf_CAtomic, typename tf_CValue>
	inline_always void fg_AtomicStore(tf_CAtomic &_AtomicValue, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept
	{
		return _AtomicValue.f_Store(fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic>
	inline_always auto fg_AtomicLoad(tf_CAtomic &_AtomicValue, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_Load(_Order))
	{
		return _AtomicValue.f_Load(_Order);
	}

	template <typename tf_CAtomic, typename tf_CValue>
	inline_always auto fg_AtomicExchange(tf_CAtomic &_AtomicValue, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_Exchange(fg_Forward<tf_CValue>(_Value), _Order))
	{
		return _AtomicValue.f_Exchange(fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic, typename tf_CExpected, typename tf_CValue>
	inline_always auto fg_AtomicCompareExchangeWeak(tf_CAtomic &_AtomicValue, tf_CExpected &_Expected, tf_CValue &&_Value, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder) noexcept -> decltype(_AtomicValue.f_CompareExchangeWeak(_Expected, fg_Forward<tf_CValue>(_Value), _SuccessOrder, _FailureOrder))
	{
		return _AtomicValue.f_CompareExchangeWeak(_Expected, fg_Forward<tf_CValue>(_Value), _SuccessOrder, _FailureOrder);
	}

	template <typename tf_CAtomic, typename tf_CExpected, typename tf_CValue>
	inline_always auto fg_AtomicCompareExchangeStrong(tf_CAtomic &_AtomicValue, tf_CExpected &_Expected, tf_CValue &&_Value, EMemoryOrder _SuccessOrder, EMemoryOrder _FailureOrder) noexcept -> decltype(_AtomicValue.f_CompareExchangeStrong(_Expected, fg_Forward<tf_CValue>(_Value), _SuccessOrder, _FailureOrder))
	{
		return _AtomicValue.f_CompareExchangeStrong(_Expected, fg_Forward<tf_CValue>(_Value), _SuccessOrder, _FailureOrder);
	}

	template <typename tf_CAtomic, typename tf_CExpected, typename tf_CValue>
	inline_always auto fg_AtomicCompareExchangeWeak(tf_CAtomic &_AtomicValue, tf_CExpected &_Expected, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_CompareExchangeWeak(_Expected, fg_Forward<tf_CValue>(_Value), _Order))
	{
		return _AtomicValue.f_CompareExchangeWeak(_Expected, fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic, typename tf_CExpected, typename tf_CValue>
	inline_always auto fg_AtomicCompareExchangeStrong(tf_CAtomic &_AtomicValue, tf_CExpected &_Expected, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_CompareExchangeStrong(_Expected, fg_Forward<tf_CValue>(_Value), _Order))
	{
		return _AtomicValue.f_CompareExchangeStrong(_Expected, fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic, typename tf_CValue>
	inline_always auto fg_AtomicFetchAdd(tf_CAtomic &_AtomicValue, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_FetchAdd(fg_Forward<tf_CValue>(_Value), _Order))
	{
		return _AtomicValue.f_FetchAdd(fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic, typename tf_CValue>
	inline_always auto fg_AtomicFetchSub(tf_CAtomic &_AtomicValue, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_FetchSub(fg_Forward<tf_CValue>(_Value), _Order))
	{
		return _AtomicValue.f_FetchSub(fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic, typename tf_CValue>
	inline_always auto fg_AtomicFetchAnd(tf_CAtomic &_AtomicValue, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_FetchAnd(fg_Forward<tf_CValue>(_Value), _Order))
	{
		return _AtomicValue.f_FetchAnd(fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic, typename tf_CValue>
	inline_always auto fg_AtomicFetchOr(tf_CAtomic &_AtomicValue, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_FetchOr(fg_Forward<tf_CValue>(_Value), _Order))
	{
		return _AtomicValue.f_FetchOr(fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic, typename tf_CValue>
	inline_always auto fg_AtomicFetchXor(tf_CAtomic &_AtomicValue, tf_CValue &&_Value, EMemoryOrder _Order = EMemoryOrder_SequentiallyConsistent) noexcept -> decltype(_AtomicValue.f_FetchXor(fg_Forward<tf_CValue>(_Value), _Order))
	{
		return _AtomicValue.f_FetchXor(fg_Forward<tf_CValue>(_Value), _Order);
	}

	template <typename tf_CAtomic>
	inline_always auto fg_AtomicIsLockFree(tf_CAtomic &_AtomicValue) noexcept -> decltype(_AtomicValue.f_IsLockFree())
	{
		return _AtomicValue.f_IsLockFree();
	}

	template <typename tf_CAtomic>
	inline_always auto fg_AtomicFlagTestAndSet(tf_CAtomic &_AtomicValue) noexcept -> decltype(_AtomicValue.f_TestAndSet())
	{
		return _AtomicValue.f_TestAndSet();
	}

	template <typename tf_CAtomic>
	inline_always auto fg_AtomicFlagClear(tf_CAtomic &_AtomicValue) noexcept -> decltype(_AtomicValue.f_Clear())
	{
		return _AtomicValue.f_Clear();
	}
}

#define DMibAtomicInit(d_Value) ATOMIC_VAR_INIT(d_Value)
#define DMibAtomicFlagInit ATOMIC_FLAG_INIT

#ifndef DMibPNoShortCuts
		using namespace NMib::NAtomic;
#endif
