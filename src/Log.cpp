#include "Common/Logger/Log/include/Log.h"
#include "Common/Logger/Log/include/LogImpl.h"
#include "Common/Logger/Log/include/SyncLoggingLocal.h"
#include "Common/Logger/Log/include/SyncLoggingRemote.h"
#include "Common/Logger/Log/include/AsyncLogging.h"
#include "Common/Logger/Log/include/base/SmartPointer.h"
#include <iostream>

namespace Logger
{
	LogLevel gLogLevel = TRACE;
	LogStrategy gLogStrategy = SYNC;
	const std::string& gLocalHost = "127.0.0.1";
	std::string gLogServerIp(gLocalHost);
	uint16_t gLogServerPort = 0;
	LogId gLogID = LOG_CLIENT;
	LogType gLogType = LOCAL_FILE;
	//LoggingStrategy* gLogStrategyInstance = NULL;
	

    LogLevel Log::globalLogLevel_ = TRACE;
    LoggingStrategy* Log::logStrategyInstance_ = NULL;

    Log::Log(const char* file_name, uint32_t line_num)
        : logImpl_(INFO, file_name, line_num)
    {
        
    }

    Log::Log(const char* file_name, uint32_t line_num, LogLevel level)
        : logImpl_(level, file_name, line_num)
    {

    }

    Log::Log(const char* file_name, uint32_t line_num, LogLevel level,const char* func_name)
        :logImpl_(level, file_name, line_num)
    {
		logImpl_.LogFuncName(func_name);
    }

    Log::Log(const char* file_name, uint32_t line_num, bool toAbort)
        :logImpl_(toAbort ? EERROR : FATAL, file_name, line_num)
    {

    }

    LogStream& Log::GetLogStream() const
    {
        return logImpl_.GetLogStream();
    }


    Log::~Log()
    {
        logImpl_.LogFileNameAndLineNumber();
        assert (logStrategyInstance_);
		logStrategyInstance_->Append(logImpl_.GetLogStream().GetLogBuffer().GetBufferData(),
			logImpl_.GetLogStream().GetLogBuffer().Length());
		if (logImpl_.GetLocalLogLevel() >= WARN)	   //write to stdout
		{
			DefaultOutput(logImpl_.GetLogStream().GetLogBuffer().GetBufferData(),
				logImpl_.GetLogStream().GetLogBuffer().Length());
			DefaultFlush();
		}
		logStrategyInstance_->Flush();
    }

	void Log::LOG_CONFIG(LogId id,
		LogType type,
		LogStrategy strategy ,
		const std::string& logServerIP,
		const uint16_t& logServerPort)
	{
		gLogID = id;
		gLogStrategy = strategy;
		gLogServerIp = logServerIP;
		gLogType = type;
		gLogServerPort = logServerPort;
		if (gLogID == LOG_SERVER)			// log server just log to the local file...
		{
			gLogType = LOCAL_FILE;
			gLogServerIp = gLocalHost;
		}
		logStrategyInstance_ = CreateLogStrategyInstance();
		assert(logStrategyInstance_);
	}

	LoggingStrategy* Log::CreateLogStrategyInstance()
	{
		if (gLogType == LOCAL_FILE && gLogStrategy == SYNC)
		{
			std::cout << "Syncing Logging local file...." << std::endl;
			return new SyncLoggingLocal();
		}


		if (gLogStrategy == ASYNC)
		{
			std::cout << "Async Logging...." << std::endl;
			return new AsyncLogging();
		}


		if (gLogType == REMOTE_NETWORK && gLogStrategy == SYNC)
		{
			std::cout << "Log remote network with sync strategy...." << std::endl;
			return new SyncLoggingRemote();
		}
        return NULL;
	}

     void Log::SetGlobalLogLevel(LogLevel loglevel)
    {
        globalLogLevel_ = loglevel;
    }

     LogLevel Log::GetGlobalLogLevel()
    {
        //return Logger::gLogLevel;
        return globalLogLevel_;
    }

     LoggingStrategy* Log::GetLogStrategyInstance()
     {
         return  logStrategyInstance_;
     }

    void Log::DefaultOutput(const char* msg, uint64_t len)
     {
         size_t n = fwrite(msg, 1, static_cast<size_t>(len), stdout);
         (void)n;
     }

     void Log::DefaultFlush() { fflush(stdout); }
}