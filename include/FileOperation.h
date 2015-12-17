#ifndef LOG_FILE_UTIL_H__
#define LOG_FILE_UTIL_H__
#include <stdint.h>
#include <stdio.h>

#include "Common/Logger/Log/include/base/SmartPointer.h"
#include "Common/Logger/Log/include/base/Noncopyable.h"
#include "Common/Logger/Log/include/base/Mutex.h"
#include "Common/Logger/Log/include/base/Atomic.h"
#include <string>


namespace Logger
{
	//internal class, ecapsulate the io operation
	class FileOperation : Noncopyable
	{
	public:
		FileOperation(const char* sFileName);
        ~FileOperation();
		void AppendLogFile(const char* logline, uint64_t len);
		void FlushLogFile();
	    int64_t GetWrittenBytes()  { return iBytesWrited_.load(); }

	private:
		uint64_t WriteLogFile(const char* msg, uint64_t msg_len);
		std::string GetLogFileName();


	private:
		FILE* logFile_;
		MutexLock mutex_; 
		AtomicInt64 iBytesWrited_; 
		char buffer_[64*1024];

	};
}



#endif
