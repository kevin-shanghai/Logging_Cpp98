#ifndef  SYNC_LOGGING_H__
#define SYNC_LOGGING_H__
#include "Common/Logger/Log/include/SyncLoggingLocal.h"
#include "Common/Logger/Log/include/FileOperation.h"
#include "Common/Logger/Log/include/LoggingStrategy.h"


namespace Logger
{
	class SyncLoggingLocal : public LoggingStrategy
	{
	public:
		SyncLoggingLocal();
		~SyncLoggingLocal();

		virtual void Append(const char* msg, uint64_t len);
		virtual void Flush();

	private:
		void CreateLogFileDynamiclly();

	private:
		shared_ptr<FileOperation> fileOperation_;
		MutexLock ioMutex_;
		const uint64_t logFileRollSize_;
	};
}


#endif