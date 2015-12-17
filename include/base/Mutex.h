#ifndef MUTEX_H_
#define MUTEX_H_

#include "Common/Logger/Log/include/PlatformDefine.h"
#include "Common/Logger/Log/include/Base/Noncopyable.h"

namespace Logger
{
    class MutexLock : private Noncopyable
    {
    public:
        MutexLock(void);

        ~MutexLock(void);

        void Lock(void);

        bool TryLock(void);

        void UnLock(void);

        MutexType* GetMutex()
        {
            return &mutex_;
        }
    private:
        MutexType mutex_;
};


    class MutexLockGuard : private Noncopyable
    {
    public:
        explicit MutexLockGuard(MutexLock& mutex)
            : mutex_(mutex)
        {
            mutex_.Lock();
        }
        ~MutexLockGuard()
        {
            mutex_.UnLock();
        }
    private:
        MutexLock& mutex_;
    };

}

#endif
