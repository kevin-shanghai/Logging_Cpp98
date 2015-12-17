#ifndef CONDITION_H__
#define CONDITION_H__

#include "Common/Logger/Log/include/Base/Mutex.h"

#include <assert.h>
#ifndef ON_WINDOWS
#include <errno.h>
#include <pthread.h>
#endif


namespace Logger
{
    class Condition : private Noncopyable
    {
    public:
        explicit Condition(MutexLock& mu) : mutex_(mu)
        {
#ifdef ON_WINDOWS
            InitializeConditionVariable(&cond_);
#else
            pthread_cond_init(&cond_, NULL);
#endif
        }

        ~Condition()
        {
#ifdef ON_WINDOWS
            //nothing
#else
            pthread_cond_destroy(&cond_);
#endif
        }

    public:
        void Wait()
        {
#ifdef ON_WINDOWS
            (void)SleepConditionVariableCS(&cond_, mutex_.GetMutex(), INFINITE);
#else
            (void)pthread_cond_wait(&cond_, mutex_.GetMutex());
#endif
        }

        bool WaitFor(int millisecond)
        {
            assert(millisecond >= 0);
#ifdef ON_WINDOWS
            bool bRet = SleepConditionVariableCS(&cond_, mutex_.GetMutex(), millisecond);
			return bRet == 0;
#else
            struct timespec abstime;
            clock_gettime(CLOCK_REALTIME, &abstime);
            abstime.tv_sec += millisecond/1000;
            return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.GetMutex(), &abstime);
#endif
        }

        void NotifyOne()
        {
#ifdef ON_WINDOWS
            WakeConditionVariable(&cond_);
#else
            pthread_cond_signal(&cond_);
#endif
        }

        void NotifyAll()
        {
#ifdef ON_WINDOWS
            WakeAllConditionVariable(&cond_);
#else
            pthread_cond_broadcast(&cond_);
#endif
        }

    private:
        MutexLock&    mutex_;
        ConditionType cond_;
    };
};

#endif
