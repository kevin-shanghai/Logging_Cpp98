#include "Common/Logger/Log/include/SyncLoggingRemote.h"
#include "Common/Logger/Log/include/Log.h"


namespace Logger
{
	SyncLoggingRemote::SyncLoggingRemote() 
        : tcpConnector_( new Net::TcpConnector
        (Net::InternetAddress(gLogServerIp, gLogServerPort)) )
        , tcpStream_(NULL)
	{
        tcpStream_ = tcpConnector_->GetClientTcpStream();
	}

	void SyncLoggingRemote::Append(const char* msg, uint64_t len)
	{
        // if connect to the log server failed, do nothing
	    if (tcpStream_)
	    {   
            tcpStream_->Send(msg, len);
	    }
	}

	void SyncLoggingRemote::Flush()
	{

	}
}