#ifndef LOGIMPL_H__
#define LOGIMPL_H__

#include "LogStream.h"
#include "Common/Logger/Log/include/PlatformDefine.h"


namespace Logger
{
    enum LogLevel{ TRACE, DEBUG, INFO, WARN, EERROR, FATAL, LOG_LEVEL_NUM };
    enum LogType { LOCAL_FILE, REMOTE_NETWORK };
    enum LogId { LOG_CLIENT, LOG_SERVER };
    enum LogStrategy { SYNC, ASYNC };

	class LogImpl : Noncopyable
	{
	public:
		LogImpl(LogLevel logLevel, const char* source_file_name, uint32_t line_num);
		~LogImpl();
		LogStream& GetLogStream() const;
		void LogFileNameAndLineNumber();
		void LogFuncName(const char* funcName){ strFuncName_ = funcName; }
		LogLevel GetLocalLogLevel(){ return logLevel_; }

	private:
		LogStream logStream_;	 
		LogLevel logLevel_;
		std::string sourceFileName_;
		uint32_t lineNum_;
		std::string strFuncName_;
	};
}

#endif