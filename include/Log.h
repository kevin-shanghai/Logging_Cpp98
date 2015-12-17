#ifndef LOG_H__
#define LOG_H__

#include <stdint.h>
#include "Common/Logger/Log/include/LogImpl.h"
#include "Common/Logger/Log/include/LoggingStrategy.h"
#include "Common/Logger/Log/include/PlatformDefine.h"


namespace Logger
{
	extern const std::string& gLocalHost;
	extern std::string gLogServerIp;
	extern uint16_t gLogServerPort;
	extern LogStrategy gLogStrategy;
	extern LogId gLogID;
	extern LogType gLogType;


	class LoggingStrategy;


	 class EXPORTLOG Log 
	{
	public:
		Log(const char* file_name, uint32_t line_num);
		Log(const char* file_name, uint32_t line_num, LogLevel level);
		Log(const char* file_name, uint32_t line_num, LogLevel level, const char* func_name);
		Log(const char* file_name, uint32_t line_num, bool toAbort);
		~Log();

	public:
		static void SetGlobalLogLevel(LogLevel loglevel);

		static LogLevel GetGlobalLogLevel();

        static LoggingStrategy* GetLogStrategyInstance();

		static void LOG_CONFIG(LogId id = LOG_CLIENT,
			LogType type = LOCAL_FILE,
			LogStrategy strategy = SYNC,
			const std::string& logServerIP = "",
			const uint16_t& logServerPort = 0);

		static LoggingStrategy* CreateLogStrategyInstance();

		LogStream& GetLogStream() const;

        void DefaultOutput(const char* msg, uint64_t len);

        void DefaultFlush();
	


	private:
		// use the stack object in order to avoid the memory segmentation 
		LogImpl logImpl_;
        static LogLevel globalLogLevel_;
        static LoggingStrategy* logStrategyInstance_;
		
	};
}


#define LOG_TRACE if (Logger::Log::GetGlobalLogLevel() <= Logger::TRACE) \
	(Logger::Log(__FILE__, __LINE__, Logger::TRACE, __FUNCTION__).GetLogStream())

#define LOG_DEBUG if (Logger::Log::GetGlobalLogLevel() <= Logger::DEBUG) \
	Logger::Log(__FILE__, __LINE__, Logger::DEBUG, __FUNCTION__).GetLogStream()

#define LOG_INFO if (Logger::Log::GetGlobalLogLevel() <= Logger::INFO) \
	(Logger::Log(__FILE__, __LINE__).GetLogStream())

#define LOG_WARN Logger::Log(__FILE__, __LINE__, Logger::WARN).GetLogStream()
#define LOG_ERROR Logger::Log(__FILE__, __LINE__, Logger::EERROR).GetLogStream()
#define LOG_FATAL Logger::Log(__FILE__, __LINE__, Logger::FATAL).GetLogStream()
#define LOG_SYSERR Logger::Log(__FILE__, __LINE__, false).GetLogStream()
#define LOG_SYSFATAL Logger::Log(__FILE__, __LINE__, true).GetLogStream()


#endif
