#include "Common/Logger/Log/include/FileOperation.h"
#include <stdio.h>
#include <assert.h>



namespace Logger
{
	FileOperation::FileOperation(const char* sFileName)
		: logFile_(::fopen(sFileName, "a+"))
		, iBytesWrited_(0)
	{
		assert(logFile_);
		::memset(buffer_, 0, sizeof buffer_);
		::setbuf(logFile_, buffer_);
	}

    FileOperation::~FileOperation()
    {
		::fclose(logFile_);
    }

	void FileOperation::AppendLogFile(const char* logline, uint64_t len)
	{
		uint64_t n = WriteLogFile(logline, len);
		uint64_t remain = len - n;
		while (remain > 0)
		{
			uint64_t x = WriteLogFile(logline + n, remain);
			if (x == 0)
			{
				int err = ferror(logFile_);
				if (err)
				{
					;
				}
				break;
			}
			n += x;
			remain = len - n; // remain -= x
		}
		iBytesWrited_.store(iBytesWrited_.load() + n);
	}


	uint64_t FileOperation::WriteLogFile(const char* msg, uint64_t msg_len)
	{
		#ifdef WIN32 
			return ::_fwrite_nolock(msg, 1, msg_len, logFile_);
		#elif defined(__linux__)
			return ::fwrite_unlocked(msg, 1, msg_len, logFile_);
		#else
         return ::fwrite(msg, 1, msg_len, logFile_);
		#endif
	}

	void FileOperation::FlushLogFile()
	{
		::fflush(logFile_);
	}

}
