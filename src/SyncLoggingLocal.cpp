#include "Common/Logger/Log/include/SyncLoggingLocal.h"
#include "Common/Logger/Log/include/FileOperation.h"
#include "Common/Logger/Log/include/base/TimeStamp.h"


namespace Logger
{
	namespace details
	{
		std::string GetLogFileName()
		{
			return TimeStamp::GetCurrentLocalTime() + ".log";
		}
	}

	static const uint64_t kSingleLogFileSize = 10 * 1024 * 1024;
	SyncLoggingLocal::SyncLoggingLocal() : logFileRollSize_(kSingleLogFileSize)
	{
		CreateLogFileDynamiclly();
	}

	SyncLoggingLocal::~SyncLoggingLocal()
	{
        //std::cout << "SyncLoggingLocal dtor..." << std::endl;
	}

	void SyncLoggingLocal::Append(const char* msg, uint64_t len)
	{
		MutexLockGuard lk(ioMutex_);
		if (fileOperation_->GetWrittenBytes() >= logFileRollSize_)
		{
			fileOperation_->FlushLogFile();
			CreateLogFileDynamiclly();
		}
		fileOperation_->AppendLogFile(msg, len);
	}

	void SyncLoggingLocal::Flush()
	{
		MutexLockGuard lk(ioMutex_);
		fileOperation_->FlushLogFile();
	}

	void SyncLoggingLocal::CreateLogFileDynamiclly()
	{
		fileOperation_.reset(new FileOperation(details::GetLogFileName().c_str()));
	}
}
