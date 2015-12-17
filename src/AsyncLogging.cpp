#include "Common/Logger/Log/include/AsyncLogging.h"
#include "Common/Logger/Log/include/SyncLoggingLocal.h"
#include "Common/Logger/Log/include/SyncLoggingRemote.h"
#include "Common/Logger/Log/include/Log.h"

namespace Logger
{
	static const uint32_t kFlushIntervalMs = 3000;
	AsyncLogging::AsyncLogging()
		: bufferFullMutex_()
		, bufferFullCond_(bufferFullMutex_)
		, syncLoggingInstance_( (gLogType == REMOTE_NETWORK) ? 
        dynamic_cast<LoggingStrategy*> (new SyncLoggingRemote) : 
        dynamic_cast<LoggingStrategy*> (new SyncLoggingLocal) )
	{

	}

	AsyncLogging::~AsyncLogging()
	{

	}

	void AsyncLogging::Append(const char* msg, uint64_t len)
	{
		MutexLockGuard lk(mutex_);
		if (len < currentBuffer_.AvailableSize())
		{
			currentBuffer_.Append(msg, len); 
		}

		//current buffer is full, so put current buffer to the sync queue
		else	
		{
			logSyncQueue_.Put(currentBuffer_);
			currentBuffer_.Clear();
			bufferFullCond_.NotifyOne();
		}
	}

	void AsyncLogging::Flush()
	{

	}

	void AsyncLogging::Start()
	{
		asyncLogThreadStartFlag_.store(1);
	}

	void AsyncLogging::Run()
	{
		while (1)
		{
			MutexLockGuard lk(bufferFullMutex_);
			if (logSyncQueue_.Empty())
			{
				while (bufferFullCond_.WaitFor(kFlushIntervalMs))
				{
					if (currentBuffer_.Length())
					{
						MutexLockGuard lk(mutex_);
						syncLoggingInstance_->Append(currentBuffer_.GetBufferData(), currentBuffer_.Length());
						syncLoggingInstance_->Flush();
						currentBuffer_.Clear();
					}
				}
			}
			LogBufferType tmpBuffer(logSyncQueue_.Take());	 
            syncLoggingInstance_->Append(tmpBuffer.GetBufferData(), tmpBuffer.Length());
		}
	}
}


