#include "Common/Logger/Log/include/base/Mutex.h"

namespace Logger
{
    MutexLock::MutexLock(void)
    {
#ifdef ON_WINDOWS
        ::InitializeCriticalSection(&mutex_);
#else
        pthread_mutex_init(&mutex_, NULL);
#endif
    }

    MutexLock::~MutexLock(void)
    {
#ifdef ON_WINDOWS
        ::DeleteCriticalSection(&mutex_);
#else

#endif
    }

    void MutexLock::Lock(void)
    {
#ifdef ON_WINDOWS
        ::EnterCriticalSection(&mutex_);
#else
        pthread_mutex_lock(&mutex_);
#endif

    }

    bool MutexLock::TryLock(void)
    {
#ifdef ON_WINDOWS
        return ::TryEnterCriticalSection(&mutex_) != 0;
#else
        return pthread_mutex_trylock(&mutex_) == 0;
#endif
    }

    void MutexLock::UnLock(void)
    {
#ifdef ON_WINDOWS
        ::LeaveCriticalSection(&mutex_);
#else
        ::pthread_mutex_unlock(&mutex_);
#endif
    }
}