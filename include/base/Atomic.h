#ifndef ATOMIC_H__
#define ATOMIC_H__

#include <stdint.h>
#include "Common/Logger/Log/include/PlatformDefine.h"


#ifdef ON_VXWORKS
#include <vxWorks.h>
#include <stdint.h>
#include <vxAtomicLib.h>
typedef atomic32_t _ATOMIC32;
typedef atomic64_t _ATOMIC64;
#endif

#ifdef ON_LINUX   

#endif


namespace Logger
{
	template <class T>
	class Atomic
	{
	public:
		Atomic()
			: m_tValue(0)
		{

		}

		Atomic(T x)
			: m_tValue(x)
		{

		}

		//write the atomic value
		T store(T x)
		{
#ifdef ON_WINDOWS
			return InterlockedExchange((LONG*)&m_tValue, (LONG)x);
#endif

#ifdef ON_VXWORKS
			if (sizeof(T) == 4)  //for int32
			{
				return vxAtomic32Set((atomic32_t*)&m_tValue, x);
			}
			else    //for int64
			{
				return vxAtomic64Set((atomic64_t*)&m_tValue, x);
			}
#endif
		}

		//get the atomic value
		T load()
		{
#ifdef ON_WINDOWS
			return InterlockedCompareExchange((LONG*)&m_tValue, 0, 0);
#endif

#ifdef ON_VXWORKS
			if (sizeof(T) == 4)
			{
				return vxAtomic32Get((atomic32_t*)&m_tValue);
			}
			else
			{
				return vxAtomic64Get((atomic64_t*)&m_tValue);
			}
#endif
		}


		T getAndAdd(T x)
		{
#ifdef ON_WINDOWS
			return InterlockedExchangeAdd((LONG*)&m_tValue, (LONG)x);
#endif

#ifdef ON_VXWORKS
			if (sizeof(T) == 4)  //for int32
			{
				return vxAtomic32Add((atomic32_t*)&m_tValue, x);
			}
			else    //for int64
			{
				return vxAtomic64Add((atomic64_t*)&m_tValue, x);
			}
#endif

#ifdef ON_LINUX
			return  __sync_fetch_and_add(&m_tValue, x);
#endif

		}

		T addAndGet(T x)
		{
			return getAndAdd(x) + x;
		}

		T incrementAndGet()
		{
			return addAndGet(1);
		}

	private:
		volatile T m_tValue;
	};

	//LOGGER_END_NAMESPACE
	//   typedef Atomic<long> AtomicLong;
	typedef Atomic<int32_t> AtomicInt32;
	typedef Atomic<int64_t> AtomicInt64;

}

#endif // mcsf_logger_atomic_h__
