// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include <Mib/Atomic/Atomic>

namespace
{

	using namespace NMib::NAtomic;
	TCAtomicAggregate<bool> g_TestInit_bool = DMibAtomicInit(false);
	
	TCAtomicAggregate<int8> g_TestInit_int8 = DMibAtomicInit(1);
	TCAtomicAggregate<int16> g_TestInit_int16 = DMibAtomicInit(2);
	TCAtomicAggregate<int32> g_TestInit_int32 = DMibAtomicInit(3);
	TCAtomicAggregate<int64> g_TestInit_int64 = DMibAtomicInit(4);

	TCAtomicAggregate<uint8> g_TestInit_uint8 = DMibAtomicInit(5);
	TCAtomicAggregate<uint16> g_TestInit_uint16 = DMibAtomicInit(6);
	TCAtomicAggregate<uint32> g_TestInit_uint32 = DMibAtomicInit(7);
	TCAtomicAggregate<uint64> g_TestInit_uint64 = DMibAtomicInit(8);

	TCAtomicAggregate<ch8> g_TestInit_ch8 = DMibAtomicInit(9);
	TCAtomicAggregate<ch16> g_TestInit_ch16 = DMibAtomicInit(10);
	TCAtomicAggregate<ch32> g_TestInit_ch32 = DMibAtomicInit(11);

	TCAtomicAggregate<mint> g_TestInit_mint = DMibAtomicInit(12);
	TCAtomicAggregate<smint> g_TestInit_smint = DMibAtomicInit(13);
	TCAtomicAggregate<aint> g_TestInit_aint = DMibAtomicInit(14);
	TCAtomicAggregate<uaint> g_TestInit_uaint = DMibAtomicInit(15);
	
	CAtomicFlagAggregate g_TestInit_Flag = DMibAtomicFlagInit;
	
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
			{
				DMibTestPath("Aggregate");
				TCAtomicAggregate<tf_CType> Atomic0 = DMibAtomicInit(0);
				TCAtomicAggregate<tf_CType> Atomic1 = DMibAtomicInit(0);
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
				NMib::NContainer::TCSet<tf_CType> FinalResult2;

				auto iToCheck = ToCheck.f_GetIterator();
				Atomic.f_Store(*iToCheck);
				++iToCheck;

				DMibTest(DMibExpr(fg_AtomicIsLockFree(Atomic)));
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
									for (int i = 0; i < 64*1024; ++i)
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

				auto iToCheck2 = ToCheck2.f_GetIterator();
				fg_AtomicStore(Atomic2, *iToCheck2);
				++iToCheck2;

				for (; iToCheck2; ++iToCheck2)
				{
					auto This2 = *iToCheck2;
					Threads.f_Insert
						(
							NMib::NThread::CThreadObject::fs_StartThread
							(
								[&, This2](NMib::NThread::CThreadObject *_pThreadObject) -> aint
								{
									fg_CompilerFence();
									auto ThisLocal2 = This2;
									for (int i = 0; i < 64*1024; ++i)
									{
										ThisLocal2 = fg_AtomicExchange(Atomic2, ThisLocal2);
									}
									{
										DMibLock(Lock);
										FinalResult2[ThisLocal2];
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
				FinalResult2[fg_AtomicLoad(Atomic2)];

				DMibTest(DMibExpr(FinalResult) == DMibExpr(ToCheck));
				DMibTest(DMibExpr(FinalResult2) == DMibExpr(ToCheck2));
			}

			// Functions
			{
				DMibTestPath("Members");
				Atomic.f_Store(0);

				tf_CType Expected = (tf_CType)0;
				DMibTest(DMibExpr(Atomic.f_CompareExchangeWeak(Expected, (tf_CType)1)));
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
			{
				DMibTestPath("Global functions");
				Atomic.f_Store(0);

				tf_CType Expected = (tf_CType)0;
				DMibTest(DMibExpr(fg_AtomicCompareExchangeWeak(Atomic, Expected, (tf_CType)1)));
				DMibTest(DMibExpr(fg_AtomicLoad(Atomic)) == DMibExpr((tf_CType)1));
				DMibTest(DMibExpr(Expected) == DMibExpr((tf_CType)0));

				Expected = (tf_CType)0;
				DMibTest(!DMibExpr(fg_AtomicCompareExchangeWeak(Atomic, Expected, (tf_CType)0)));
				DMibTest(DMibExpr(fg_AtomicLoad(Atomic)) == DMibExpr((tf_CType)1) && DMibExpr("Second"));
				DMibTest(DMibExpr(Expected) == DMibExpr((tf_CType)1));

				Expected = (tf_CType)1;
				DMibTest(DMibExpr(fg_AtomicCompareExchangeStrong(Atomic, Expected, (tf_CType)0)));
				DMibTest(DMibExpr(fg_AtomicLoad(Atomic)) == DMibExpr((tf_CType)0));
				DMibTest(DMibExpr(Expected) == DMibExpr((tf_CType)1) && DMibExpr("Second"));

				Expected = (tf_CType)1;
				DMibTest(!DMibExpr(fg_AtomicCompareExchangeStrong(Atomic, Expected, (tf_CType)0)));
				DMibTest(DMibExpr(fg_AtomicLoad(Atomic)) == DMibExpr((tf_CType)0) && DMibExpr("Second"));
				DMibTest(DMibExpr(Expected) == DMibExpr((tf_CType)0) && DMibExpr("Third"));

			}
		}

		template <typename tf_CType>
		void f_TestIntegerAndPointer(NMib::NStr::CStr const &_Name)
		{
			{
				DMibTestPath("Normal");;
				typename NMib::NTraits::TCRemovePointer<tf_CType>::CType RawValues[4] = {};
				tf_CType Value;
				if constexpr (NMib::NTraits::TCIsPointer<tf_CType>::mc_Value)
					Value = RawValues;
				else
					Value = 0;

				TCAtomic<tf_CType> Atomic;
				f_TestIntegerAndPointerGeneric(_Name, Atomic, Value);
			}
			{
				DMibTestPath("Aggregate");
				typename NMib::NTraits::TCRemovePointer<tf_CType>::CType RawValues[4] = {};
				tf_CType Value;
				if constexpr (NMib::NTraits::TCIsPointer<tf_CType>::mc_Value)
					Value = RawValues;
				else
					Value = 0;

				TCAtomicAggregate<tf_CType> Atomic = DMibAtomicInit(0);
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
				fg_AtomicStore(Atomic, Value);

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
			{
				DMibTestPath("Global functions");
				auto Value = _Value;
				fg_AtomicStore(Atomic, Value);

				DMibTest(DMibExpr(fg_AtomicFetchAdd(Atomic, 1)) == DMibExpr(Value));
				Value += 1;
				DMibTest(DMibExpr(fg_AtomicFetchSub(Atomic, 1)) == DMibExpr(Value));

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
			{
				DMibTestPath("Aggregate");
				TCAtomicAggregate<tf_CType> Atomic = DMibAtomicInit(7);
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
			{
				DMibTestPath("Global functions");
				fg_AtomicExchange(Atomic, 7);

				DMibTest(DMibExpr(fg_AtomicFetchAnd(Atomic, 5)) == DMibExpr(7)); // = 5
				DMibTest(DMibExpr(fg_AtomicFetchOr(Atomic, 2)) == DMibExpr(5)); // = 7
				DMibTest(DMibExpr(fg_AtomicFetchXor(Atomic, 2)) == DMibExpr(7)); // = 5
				DMibTest(DMibExpr(fg_AtomicLoad(Atomic)) == DMibExpr(5));
			}
		}

		void f_TestAtomicFlag()
		{
			{
				DMibTestPath("Normal");
				CAtomicFlag Atomic;
				f_TestAtomicFlagGeneric(Atomic);
			}
			{
				DMibTestPath("Aggregate");
				CAtomicFlagAggregate Atomic = DMibAtomicFlagInit;
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
								for (int i = 0; i < 64*1024; ++i)
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

			auto iToCheck2 = ToCheck2.f_GetIterator();
			int8 Atomic2;
			Atomic2 = *iToCheck2;
			++iToCheck2;

			for (; iToCheck2; ++iToCheck2)
			{
				auto This2 = *iToCheck2;
				Threads.f_Insert
					(
						NMib::NThread::CThreadObject::fs_StartThread
						(
							[&, This2](NMib::NThread::CThreadObject *_pThreadObject) -> aint
							{
								fg_CompilerFence();
								auto ThisLocal2 = This2;
								for (int i = 0; i < 64*1024; ++i)
								{
									while (fg_AtomicFlagTestAndSet(Flag))
										NMib::NSys::fg_Thread_Yield();
									NMib::fg_Swap(ThisLocal2, Atomic2);
									fg_AtomicFlagClear(Flag);
								}
								{
									DMibLock(Lock);
									FinalResult2[ThisLocal2];
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
			FinalResult2[Atomic2];

			DMibTest(DMibExpr(FinalResult) == DMibExpr(ToCheck));
			DMibTest(DMibExpr(FinalResult2) == DMibExpr(ToCheck2));
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

