#ifndef SYNC_LOGGING_REMOTE
#define SYNC_LOGGING_REMOTE

#include "Common/Logger/Log/include/LoggingStrategy.h"
#include "Common/Logger/Log/include/net/TcpStream.h"
#include "Common/Logger/Log/include/net/TcpConnector.h"


namespace Logger
{
	class SyncLoggingRemote : public LoggingStrategy
	{
	public:
		SyncLoggingRemote();

		~SyncLoggingRemote(){  }; 

		virtual void Append(const char* msg, uint64_t len);

		virtual void Flush();

	private:
		Net::TcpStream* tcpStream_;
        Net::TcpConnector* tcpConnector_;

	};
}


#endif