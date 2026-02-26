#pragma once

namespace NMib::NAtomic
{
	template <typename t_CType>
	constexpr TCAtomicIndeterminate<t_CType>::TCAtomicIndeterminate(t_CType _Value) noexcept
		: m_Value(_Value)
	{
	}

	template <typename t_CType>
	inline_always t_CType TCAtomicIndeterminate<t_CType>::operator = (t_CType _Value) volatile noexcept
	{
		std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).store(_Value);
		return _Value;
	}

	template <typename t_CType>
	inline_always t_CType TCAtomicIndeterminate<t_CType>::operator = (t_CType _Value) noexcept
	{
		std::atomic_ref<t_CType>(m_Value).store(_Value);
		return _Value;
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_IsLockFree() const noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).is_lock_free();
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_IsLockFree() const volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).is_lock_free();
	}

	template <typename t_CType>
	constexpr void TCAtomicIndeterminate<t_CType>::f_Store(t_CType _Value, CMemoryOrder _Order) noexcept
	{
		std::atomic_ref<t_CType>(m_Value).store(_Value, _Order);
	}

	template <typename t_CType>
	constexpr void TCAtomicIndeterminate<t_CType>::f_Store(t_CType _Value, CMemoryOrder _Order) volatile noexcept
	{
		std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).store(_Value, _Order);
	}

	template <typename t_CType>
	constexpr t_CType TCAtomicIndeterminate<t_CType>::f_Load(CMemoryOrder _Order) const noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).load(_Order);
	}

	template <typename t_CType>
	constexpr t_CType TCAtomicIndeterminate<t_CType>::f_Load(CMemoryOrder _Order) const volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).load(_Order);
	}

	template <typename t_CType>
	constexpr TCAtomicIndeterminate<t_CType>::operator t_CType () const noexcept
	{
		return f_Load();
	}

	template <typename t_CType>
	constexpr TCAtomicIndeterminate<t_CType>::operator t_CType () const volatile noexcept
	{
		return f_Load();
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_Exchange(t_CType _Value, CMemoryOrder _Order) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).exchange(_Value, _Order);
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_Exchange(t_CType _Value, CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).exchange(_Value, _Order);
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, CMemoryOrder _SuccessOrder, CMemoryOrder _FailureOrder) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).compare_exchange_weak(_Expected, _Desired, _SuccessOrder, _FailureOrder);
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, CMemoryOrder _SuccessOrder, CMemoryOrder _FailureOrder ) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).compare_exchange_weak(_Expected, _Desired, _SuccessOrder, _FailureOrder);
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, CMemoryOrder _Order) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).compare_exchange_weak(_Expected, _Desired, _Order);
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_CompareExchangeWeak(t_CType &_Expected, t_CType _Desired, CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).compare_exchange_weak(_Expected, _Desired, _Order);
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, CMemoryOrder _SuccessOrder, CMemoryOrder _FailureOrder) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).compare_exchange_strong(_Expected, _Desired, _SuccessOrder, _FailureOrder);
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, CMemoryOrder _SuccessOrder, CMemoryOrder _FailureOrder ) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).compare_exchange_strong(_Expected, _Desired, _SuccessOrder, _FailureOrder);
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, CMemoryOrder _Order) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).compare_exchange_strong(_Expected, _Desired, _Order);
	}

	template <typename t_CType>
	bool TCAtomicIndeterminate<t_CType>::f_CompareExchangeStrong(t_CType &_Expected, t_CType _Desired, CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).compare_exchange_strong(_Expected, _Desired, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchAdd(tf_CType _Value, CMemoryOrder _Order) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_add(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchAdd(tf_CType _Value, CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_add(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchSub(tf_CType _Value, CMemoryOrder _Order) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_sub(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchSub(tf_CType _Value, CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_sub(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchAnd(tf_CType _Value, CMemoryOrder _Order) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_and(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchAnd(tf_CType _Value, CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_and(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchOr(tf_CType _Value, CMemoryOrder _Order) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_or(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchOr(tf_CType _Value, CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_or(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchXor(tf_CType _Value, CMemoryOrder _Order) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_xor(_Value, _Order);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::f_FetchXor(tf_CType _Value, CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_xor(_Value, _Order);
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator ++ () noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_add(1)+1;
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator ++ () volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_add(1)+1;
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator ++ (int) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_add(1);
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator ++ (int) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_add(1);
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator -- () noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_sub(1)-1;
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator -- () volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_sub(1)-1;
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator -- (int) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_sub(1);
	}

	template <typename t_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator -- (int) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_sub(1);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator += (tf_CType _Value) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_add(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator += (tf_CType _Value) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_add(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator -= (tf_CType _Value) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_sub(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator -= (tf_CType _Value) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_sub(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator &= (tf_CType _Value) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_and(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator &= (tf_CType _Value) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_and(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator |= (tf_CType _Value) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_or(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator |= (tf_CType _Value) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_or(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator ^= (tf_CType _Value) noexcept
	{
		return std::atomic_ref<t_CType>(m_Value).fetch_xor(_Value);
	}

	template <typename t_CType>
	template <typename tf_CType>
	t_CType TCAtomicIndeterminate<t_CType>::operator ^= (tf_CType _Value) volatile noexcept
	{
		return std::atomic_ref<t_CType>(const_cast<t_CType &>(m_Value)).fetch_xor(_Value);
	}

	template <typename t_CType>
	t_CType &TCAtomicIndeterminate<t_CType>::f_NonAtomic()
	{
		return m_Value;
	}

	template <typename t_CType>
	t_CType const &TCAtomicIndeterminate<t_CType>::f_NonAtomic() const
	{
		return m_Value;
	}

	template <typename t_CType>
	template <typename tf_CFormatter>
	auto TCAtomicIndeterminate<t_CType>::f_CreateStringFormatter(tf_CFormatter &_Formatter) const
	{
		return fg_CreateStringFormatter(_Formatter, this->f_Load());
	}

	template <typename t_CType>
	constexpr TCAtomic<t_CType>::TCAtomic() noexcept(NTraits::cIsNothrowDefaultConstructible<t_CType>)
		: TCAtomicIndeterminate<t_CType>(t_CType())
	{
	}

	constexpr inline_always CAtomicFlag::CAtomicFlag() noexcept
		: CSuper{}
	{
	}

	inline_always bool CAtomicFlag::f_TestAndSet(CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_flag::test_and_set(_Order);
	}

	inline_always bool CAtomicFlag::f_TestAndSet(CMemoryOrder _Order) noexcept
	{
		return std::atomic_flag::test_and_set(_Order);
	}
	inline_always void CAtomicFlag::f_Clear(CMemoryOrder _Order) volatile noexcept
	{
		return std::atomic_flag::clear(_Order);
	}
	inline_always void CAtomicFlag::f_Clear(CMemoryOrder _Order) noexcept
	{
		return std::atomic_flag::clear(_Order);
	}

	template <typename tf_CType>
	inline_always tf_CType fg_KillDependency(tf_CType _Value) noexcept
	{
		return std::kill_dependency(_Value);
	}

	inline_always void fg_MemoryFence(CMemoryOrder _Order) noexcept
	{
		return std::atomic_thread_fence(_Order);
	}

	inline_always void fg_CompilerFence(CMemoryOrder _Order) noexcept
	{
		return std::atomic_signal_fence(_Order);
	}
}
