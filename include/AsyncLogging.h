#ifndef ASYNC_LOGGING_H__
#define ASYNC_LOGGING_H__

#include <stdint.h>
#include "Common/Logger/Log/include/base/Atomic.h"
#include "Common/Logger/Log/include/base/SmartPointer.h"
#include "Common/Logger/Log/include/base/Condition.h"
#include "Common/Logger/Log/include/base/Noncopyable.h"
#include "Common/Logger/Log/include/base/ASyncQueue.h"
#include "Common/Logger/Log/include/Buffer.h"
#include "Common/Logger/Log/include/AsyncLogging.h"
#include "Common/Logger/Log/include/Net/TcpStream.h"
#include "Common/Logger/Log/include/LoggingStrategy.h"
#include "Common/Logger/Log/include/base/Thread.h"



namespace Logger
{
	class AsyncLogging  : public LoggingStrategy
						, public Thread
	{
	public:
		AsyncLogging();

		~AsyncLogging();

		virtual void Append(const char* msg, uint64_t len);

		virtual void Flush();

		void Start();

		void SetTcpStream(shared_ptr<Net::TcpStream> tcpStream);

	private:
		void Run();

	private:
		//std::thread loggingThread_;
		Buffer<kLargeBuffer> currentBuffer_;
		Buffer<kLargeBuffer> nextBuffer_;
		AtomicInt32 currentBufferFullFlag_; //{ false };
		AtomicInt32 asyncLogThreadStartFlag_; //{ false };
		Condition bufferFullCond_;
		MutexLock bufferFullMutex_;
		MutexLock mutex_;
		//chrono::system_clock::time_point lastFlushTime_;
		typedef Buffer<kLargeBuffer> LogBufferType;
		typedef AsyncQueue<LogBufferType> LogQueueType;
		LogQueueType logSyncQueue_;
		LoggingStrategy* syncLoggingInstance_;
	};
}


#endif
