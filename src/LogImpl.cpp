
#include "Common/Logger/Log/include/LogImpl.h"
#include "Common/Logger/Log/include/Log.h"
#include <string>
#include <iomanip>
#include <ctime>
#include <map>
#include <sstream>
#include <string.h> //for strrchr in linux

#include "Common/Logger/Log/include/base/TimeStamp.h"
#include "Common/Logger/Log/include/base/Thread.h"

namespace Logger
{
	namespace details
	{

		const char* GetFileBaseName(const char* sourceFileName)
		{
			#ifdef ON_WINDOWS 
			 const char file_path_split = '\\';
			#else
			 const char file_path_split = '/';
			#endif
			const char* slash = ::strrchr(sourceFileName, file_path_split);
			return slash ? slash + 1 : NULL;
		}
	}


    std::string LogLevelName[LOG_LEVEL_NUM] = {
        "TRACE ",
        "DEBUG ",
        "INFO  ",
        "WARN  ",
        "ERROR ",
        "FATAL ", 
    };

	LogImpl::LogImpl(LogLevel logLevel, const char* source_file_name, uint32_t line_num)
		: logLevel_(logLevel)
		, sourceFileName_(source_file_name)
		, lineNum_(line_num)
		, strFuncName_("")
		, logStream_()
	{
		std::string currentTime = TimeStamp::Now().ToCalenderTime();
        assert (logLevel <= LOG_LEVEL_NUM);
        logStream_ << LogLevelName[logLevel] << " " 
            << currentTime << " " 
            << this_thread::get_id() << " ";
	}

	LogImpl::~LogImpl()
	{
	}

	LogStream& LogImpl::GetLogStream() const
	{
		return const_cast<LogStream&>(logStream_);
	}

	void LogImpl::LogFileNameAndLineNumber()
	{
		if (strFuncName_.empty())
			logStream_ << " "<< details::GetFileBaseName(sourceFileName_.c_str()) 
					   << ":" << lineNum_ 
					   << "\n";
		else
			logStream_ <<  " " << strFuncName_ 
					   << " " << details::GetFileBaseName(sourceFileName_.c_str()) 
					   << ":" << lineNum_ 
					   << "\n";
	}
}
