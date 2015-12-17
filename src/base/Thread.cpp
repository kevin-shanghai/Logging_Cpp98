#include "Common/Logger/Log/include/base/Thread.h"

#ifdef ON_WINDOWS
#include <process.h>
#endif


namespace Logger
{
#if defined(ON_WINDOWS)
    static unsigned WINAPI RunInThread(void *arg)
#else
    static void* RunInThread(void *arg)
#endif
    {
		Thread* pThread = static_cast<Thread*>(arg);
		pThread->Run();
		return 0;
    }


    Thread::Thread(const std::string& threadName /* = "UnKnow" */)
        : threadName_(threadName)
    {
#if defined(ON_WINDOWS)
        _beginthreadex(0, 0, &RunInThread, this, 0, &threadID_);
#else
        if (pthread_create(&threadID_, NULL, &RunInThread, this) != 0)
            threadID_ = 0;
#endif
    }



}
