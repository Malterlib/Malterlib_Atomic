// Copyright © 2015 Hansoft AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include <Mib/Atomic/Atomic>

namespace
{

	using namespace NMib::NAtomic;
	constinit TCAtomic<bool> g_TestInit_bool{false};
	static_assert(sizeof(g_TestInit_bool) == 1);

	constinit TCAtomic<int8> g_TestInit_int8{1};
	static_assert(sizeof(g_TestInit_int8) == 1);

	constinit TCAtomic<int16> g_TestInit_int16{2};
	static_assert(sizeof(g_TestInit_int16) == 2);

	constinit TCAtomic<int32> g_TestInit_int32{3};
	static_assert(sizeof(g_TestInit_int32) == 4);

	constinit TCAtomic<int64> g_TestInit_int64{4};
	static_assert(sizeof(g_TestInit_int64) == 8);

	constinit TCAtomic<uint8> g_TestInit_uint8{5};
	static_assert(sizeof(g_TestInit_uint8) == 1);

	constinit TCAtomic<uint16> g_TestInit_uint16{6};
	static_assert(sizeof(g_TestInit_uint16) == 2);

	constinit TCAtomic<uint32> g_TestInit_uint32{7};
	static_assert(sizeof(g_TestInit_uint32) == 4);

	constinit TCAtomic<uint64> g_TestInit_uint64{8};
	static_assert(sizeof(g_TestInit_uint64) == 8);

	constinit TCAtomic<ch8> g_TestInit_ch8{9};
	static_assert(sizeof(g_TestInit_ch8) == 1);

	constinit TCAtomic<ch16> g_TestInit_ch16{10};
	static_assert(sizeof(g_TestInit_ch16) == 2);

	constinit TCAtomic<ch32> g_TestInit_ch32{11};
	static_assert(sizeof(g_TestInit_ch32) == 4);

	constinit TCAtomic<mint> g_TestInit_mint{12};
	static_assert(sizeof(g_TestInit_mint) == sizeof(mint));

	constinit TCAtomic<smint> g_TestInit_smint{13};
	static_assert(sizeof(g_TestInit_smint) == sizeof(smint));

	constinit TCAtomic<aint> g_TestInit_aint{14};
	static_assert(sizeof(g_TestInit_aint) == sizeof(aint));

	constinit TCAtomic<uaint> g_TestInit_uaint{15};
	static_assert(sizeof(g_TestInit_uaint) == sizeof(uaint));

	constinit CAtomicFlag g_TestInit_Flag{};

	void fg_ReferenceVars()
	{
		(void)g_TestInit_bool;

		(void)g_TestInit_int8;
		(void)g_TestInit_int16;
		(void)g_TestInit_int32;
		(void)g_TestInit_int64;

		(void)g_TestInit_uint8;
		(void)g_TestInit_uint16;
		(void)g_TestInit_uint32;
		(void)g_TestInit_uint64;

		(void)g_TestInit_ch8;
		(void)g_TestInit_ch16;
		(void)g_TestInit_ch32;

		(void)g_TestInit_mint;
		(void)g_TestInit_smint;
		(void)g_TestInit_aint;
		(void)g_TestInit_uaint;

		(void)g_TestInit_Flag;
	}

#ifdef DMibSanitizerEnabled
	static constexpr mint gc_Iterations = 8 * 1024;
#else
	static constexpr mint gc_Iterations = 64 * 1024;
#endif

	class CAtomic_Tests : public NMib::NTest::CTest
	{
	public:

		template <typename tf_CType>
		void f_TestShared(NMib::NStr::CStr const &_Name)
		{
			fg_ReferenceVars();
			{
				DMibTestPath("Normal");
				TCAtomic<tf_CType> Atomic0;
				TCAtomic<tf_CType> Atomic1;
				f_TestSharedGeneric<tf_CType>(_Name, Atomic0, Atomic1);
			}
		}

		template <typename tf_CType, typename tf_CAtomicType>
		void f_TestSharedGeneric(NMib::NStr::CStr const &_Name, tf_CAtomicType &_Atomic0, tf_CAtomicType &_Atomic1)
		{
			tf_CAtomicType &Atomic = _Atomic0;
			tf_CAtomicType &Atomic2 = _Atomic1;
			DMibTestPath(_Name);
			{
				NMib::NContainer::TCVector<NMib::NStorage::TCUniquePointer<NMib::NThread::CThreadObject>> Threads;

				NMib::NContainer::TCSet<tf_CType> ToCheck;
				for (int i = 0; i < 32; ++i)
					ToCheck[(tf_CType)(mint)NMib::NMisc::fg_GetRandom()];

				NMib::NContainer::TCSet<tf_CType> ToCheck2;
				for (int i = 0; i < 32; ++i)
					ToCheck2[(tf_CType)(mint)NMib::NMisc::fg_GetRandom()];

				NMib::NThread::CMutual Lock;

				NMib::NContainer::TCSet<tf_CType> FinalResult;

				auto iToCheck = ToCheck.f_GetIterator();
				Atomic.f_Store(*iToCheck);
				++iToCheck;

				DMibTest(DMibExpr(Atomic.f_IsLockFree()));

				for (; iToCheck; ++iToCheck)
				{
					auto This = *iToCheck;
					Threads.f_Insert
						(
							NMib::NThread::CThreadObject::fs_StartThread
							(
								[&, This](NMib::NThread::CThreadObject *_pThreadObject) -> aint
								{
									NMib::NAtomic::fg_MemoryFence();
									auto ThisLocal = This;
									for (int i = 0; i < gc_Iterations; ++i)
									{
										ThisLocal = Atomic.f_Exchange(ThisLocal);
									}
									ThisLocal = fg_KillDependency(ThisLocal);
									{
										DMibLock(Lock);
										FinalResult[ThisLocal];
									}
									return 0;
								}
								, "TestThread"
							)
						)
					;
				}

				Threads.f_Clear();

				FinalResult[Atomic.f_Load()];

				DMibTest(DMibExpr(FinalResult) == DMibExpr(ToCheck));
			}

			// Functions
			{
				DMibTestPath("Members");
				Atomic.f_Store(0);

				tf_CType Expected = (tf_CType)0;
				while (!Atomic.f_CompareExchangeWeak(Expected, (tf_CType)1))
					;
				DMibTest(DMibExpr(Atomic.f_Load()) == DMibExpr((tf_CType)1));
				DMibTest(DMibExpr(Expected) == DMibExpr((tf_CType)0));

				Expected = (tf_CType)0;
				DMibTest(!DMibExpr(Atomic.f_CompareExchangeWeak(Expected, (tf_CType)0)));
				DMibTest(DMibExpr(Atomic.f_Load()) == DMibExpr((tf_CType)1) && DMibExpr("Second"));
				DMibTest(DMibExpr(Expected) == DMibExpr((tf_CType)1));

				Expected = (tf_CType)1;
				DMibTest(DMibExpr(Atomic.f_CompareExchangeStrong(Expected, (tf_CType)0)));
				DMibTest(DMibExpr(Atomic.f_Load()) == DMibExpr((tf_CType)0));
				DMibTest(DMibExpr(Expected) == DMibExpr((tf_CType)1) && DMibExpr("Second"));

				Expected = (tf_CType)1;
				DMibTest(!DMibExpr(Atomic.f_CompareExchangeStrong(Expected, (tf_CType)0)));
				DMibTest(DMibExpr(Atomic.f_Load()) == DMibExpr((tf_CType)0) && DMibExpr("Second"));
				DMibTest(DMibExpr(Expected) == DMibExpr((tf_CType)0) && DMibExpr("Third"));

			}
		}

		template <typename tf_CType>
		void f_TestIntegerAndPointer(NMib::NStr::CStr const &_Name)
		{
			{
				DMibTestPath("Normal");;
				NMib::NTraits::TCRemovePointer<tf_CType> RawValues[4] = {};
				tf_CType Value;
				if constexpr (NMib::NTraits::cIsPointer<tf_CType>)
					Value = RawValues;
				else
					Value = 0;

				TCAtomic<tf_CType> Atomic;
				f_TestIntegerAndPointerGeneric(_Name, Atomic, Value);
			}
		}

		template <typename tf_CType, typename tf_CAtomicType>
		void f_TestIntegerAndPointerGeneric(NMib::NStr::CStr const &_Name, tf_CAtomicType &_Atomic0, tf_CType &_Value)
		{
			auto &Atomic = _Atomic0;
			DMibTestPath(_Name);
			// Functions
			{
				DMibTestPath("Members");
				auto Value = _Value;
				Atomic.f_Store(Value);

				DMibTest(DMibExpr(Atomic.f_FetchAdd(1)) == DMibExpr(Value));
				Value += 1;
				DMibTest(DMibExpr(Atomic.f_FetchSub(1)) == DMibExpr(Value));
				DMibTest(DMibExpr(++Atomic) == DMibExpr(Value));
				DMibTest(DMibExpr(Atomic++) == DMibExpr(Value));
				DMibTest(DMibExpr(--Atomic) == DMibExpr(Value));
				DMibTest(DMibExpr(Atomic--) == DMibExpr(Value));
				--Value;
				DMibTest(DMibExpr(Atomic) == DMibExpr(Value));
				DMibTest(DMibExpr(Atomic += 2) == DMibExpr(Value));
				Value += 2;
				DMibTest(DMibExpr(Atomic -= 1) == DMibExpr(Value));
				Value -= 2;

			}
		}

		template <typename tf_CType>
		void f_TestInteger(NMib::NStr::CStr const &_Name)
		{
			{
				DMibTestPath("Normal");
				TCAtomic<tf_CType> Atomic(7);
				f_TestIntegerGeneric<tf_CType>(_Name, Atomic);
			}
		}

		template <typename tf_CType, typename tf_CAtomicType>
		void f_TestIntegerGeneric(NMib::NStr::CStr const &_Name, tf_CAtomicType &_Atomic0)
		{
			auto &Atomic = _Atomic0;
			DMibTestPath(_Name);
			// Functions
			{
				DMibTestPath("Members");

				DMibTest(DMibExpr(Atomic.f_FetchAnd(5)) == DMibExpr(7)); // = 5
				DMibTest(DMibExpr(Atomic.f_FetchOr(2)) == DMibExpr(5)); // = 7
				DMibTest(DMibExpr(Atomic.f_FetchXor(2)) == DMibExpr(7)); // = 5
				DMibTest(DMibExpr(Atomic.f_Load()) == DMibExpr(5));
			}
			{
				DMibTestPath("Member operators");
				Atomic.f_Exchange(7);

				DMibTest(DMibExpr(Atomic &= 5) == DMibExpr(7)); // = 5
				DMibTest(DMibExpr(Atomic |= 2) == DMibExpr(5)); // = 7
				DMibTest(DMibExpr(Atomic ^= 2) == DMibExpr(7)); // = 5
				DMibTest(DMibExpr(Atomic.f_Load()) == DMibExpr(5));
			}
		}

		void f_TestAtomicFlag()
		{
			{
				DMibTestPath("Normal");
				CAtomicFlag Atomic;
				f_TestAtomicFlagGeneric(Atomic);
			}
		}

		template <typename tf_CAtomicType>
		void f_TestAtomicFlagGeneric(tf_CAtomicType &_Atomic0)
		{
			auto &Flag = _Atomic0;

			DMibTestPath("Atomic flag");

			NMib::NContainer::TCVector<NMib::NStorage::TCUniquePointer<NMib::NThread::CThreadObject>> Threads;

			NMib::NContainer::TCSet<int8> ToCheck;
			for (int i = 0; i < 32; ++i)
				ToCheck[(int8)NMib::NMisc::fg_GetRandom()];

			NMib::NContainer::TCSet<int8> ToCheck2;
			for (int i = 0; i < 32; ++i)
				ToCheck2[(int8)NMib::NMisc::fg_GetRandom()];

			NMib::NThread::CMutual Lock;

			NMib::NContainer::TCSet<int8> FinalResult;
			NMib::NContainer::TCSet<int8> FinalResult2;

			auto iToCheck = ToCheck.f_GetIterator();
			int8 Atomic = 0;
			Atomic = *iToCheck;
			++iToCheck;

			for (; iToCheck; ++iToCheck)
			{
				auto This = *iToCheck;
				Threads.f_Insert
					(
						NMib::NThread::CThreadObject::fs_StartThread
						(
							[&, This](NMib::NThread::CThreadObject *_pThreadObject) -> aint
							{
								NMib::NAtomic::fg_MemoryFence();
								auto ThisLocal = This;
								for (int i = 0; i < gc_Iterations; ++i)
								{
									while (Flag.f_TestAndSet())
										NMib::NSys::fg_Thread_Yield();
									NMib::fg_Swap(Atomic, ThisLocal);
									Flag.f_Clear();
								}
								ThisLocal = fg_KillDependency(ThisLocal);
								{
									DMibLock(Lock);
									FinalResult[ThisLocal];
								}
								return 0;
							}
							, "TestThread"
						)
					)
				;
			}

			Threads.f_Clear();

			FinalResult[Atomic];

			DMibTest(DMibExpr(FinalResult) == DMibExpr(ToCheck));
		}

		void f_DoTests()
		{
			DMibTestSuite("AtomicFlag")
			{
				f_TestAtomicFlag();
			};
			DMibTestSuite("Shared")
			{
				f_TestShared<bool>("bool");
				f_TestShared<int8>("int8");
				f_TestShared<int16>("int16");
				f_TestShared<int32>("int32");
				f_TestShared<int64>("int64");
				f_TestShared<uint8>("uint8");
				f_TestShared<uint16>("uint16");
				f_TestShared<uint32>("uint32");
				f_TestShared<uint64>("uint64");

				f_TestShared<ch8>("ch8");
				f_TestShared<ch16>("ch16");
				f_TestShared<ch32>("ch32");

				f_TestShared<mint>("mint");
				f_TestShared<smint>("smint");
				f_TestShared<aint>("aint");
				f_TestShared<uaint>("uaint");

				f_TestShared<int *>("int *");
			};
			DMibTestSuite("IntegerAndPointer")
			{
				f_TestIntegerAndPointer<int8>("int8");
				f_TestIntegerAndPointer<int16>("int16");
				f_TestIntegerAndPointer<int32>("int32");
				f_TestIntegerAndPointer<int64>("int64");
				f_TestIntegerAndPointer<uint8>("uint8");
				f_TestIntegerAndPointer<uint16>("uint16");
				f_TestIntegerAndPointer<uint32>("uint32");
				f_TestIntegerAndPointer<uint64>("uint64");

				f_TestIntegerAndPointer<ch8>("ch8");
				f_TestIntegerAndPointer<ch16>("ch16");
				f_TestIntegerAndPointer<ch32>("ch32");

				f_TestIntegerAndPointer<mint>("mint");
				f_TestIntegerAndPointer<smint>("smint");
				f_TestIntegerAndPointer<aint>("aint");
				f_TestIntegerAndPointer<uaint>("uaint");
				f_TestIntegerAndPointer<int *>("int *");
			};
			DMibTestSuite("Integer")
			{
				f_TestInteger<int8>("int8");
				f_TestInteger<int16>("int16");
				f_TestInteger<int32>("int32");
				f_TestInteger<int64>("int64");
				f_TestInteger<uint8>("uint8");
				f_TestInteger<uint16>("uint16");
				f_TestInteger<uint32>("uint32");
				f_TestInteger<uint64>("uint64");

				f_TestInteger<ch8>("ch8");
				f_TestInteger<ch16>("ch16");
				f_TestInteger<ch32>("ch32");

				f_TestInteger<mint>("mint");
				f_TestInteger<smint>("smint");
				f_TestInteger<aint>("aint");
				f_TestInteger<uaint>("uaint");
			};
		}
	};

	DMibTestRegister(CAtomic_Tests, Malterlib::Atomic);
}

