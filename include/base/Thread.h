#ifndef THREAD_H__
#define THREAD_H__

#include "Common/Logger/Log/include/PlatformDefine.h"
#include "Common/Logger/Log/include/base/Noncopyable.h"
#include <string>
#include <stdint.h>

#if defined(ON_WINDOWS)
typedef unsigned int ThreadIDType;
#else
#include <unistd.h>
#include <errno.h>
typedef pthread_t ThreadIDType;
#endif

namespace Logger
{
	class Thread : private Noncopyable
	{
	public:
		explicit Thread(const std::string& threadName = "UnKnow");
		ThreadIDType GetThreadID() { return threadID_; }
		virtual void Run() = 0;

	private:
		const std::string& threadName_;
		ThreadIDType threadID_;
	};
}

namespace this_thread
{
	inline int64_t get_id()
	{
		int64_t threadID = 0;
#ifdef ON_LINUX
		threadID = (int64_t)syscall(SYS_gettid);
#elif defined ON_WINDOWS
		threadID = ::GetCurrentThreadId();
#else
		threadID = (int64_t)pthread_self();
#endif
		return threadID;

	}
}


#endif
