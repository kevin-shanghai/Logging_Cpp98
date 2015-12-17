#include "Common/Logger/Log/include/net/TcpStream.h"
#include "Common/Logger/Log/include/Log.h"
#ifdef ON_VXWORKS
#include <strings.h>
#endif

namespace Net
{
	TcpStream::TcpStream(const socket_t& sockFd, const InternetAddress& inetAddress)
		: sockFd_(sockFd)
        , address_(inetAddress)
	{

	}
	
	TcpStream::TcpStream(const TcpStream& rhs)
		: sockFd_(rhs.sockFd_)
		, address_(rhs.address_)
	{
		
	}

	size_t TcpStream::Send(const char* buffer, size_t len)
	{
		return SocketsApi::Send(sockFd_, buffer, len);
	}

	size_t TcpStream::ReceiveMsgAndLogFile()
	{
        while (1)
        {
            int64_t read_bytes = inputBuffer_.ReadFromSocketFD(sockFd_);
            if (read_bytes > 0)
            {
                std::string readMsg(inputBuffer_.ReadContentsUntilLastEOF());
                Logger::Log::GetLogStrategyInstance()->Append(readMsg.c_str(), readMsg.size());
            }
            else if (read_bytes == 0)
            {
                //TODO log client disconnection handle
            }
            else
            {
                //TODO socket error handle
            }
        }
        return 0;
	}

	

	bool TcpStream::WaitForSocketReadEvent(uint32_t iTimeoutSeconds)
	{
		fd_set sock_read_sets;
		struct timeval tv;
		tv.tv_sec = iTimeoutSeconds;
		tv.tv_usec = 0;
		FD_ZERO(&sock_read_sets);
		FD_SET(sockFd_, &sock_read_sets);
		return 	select(sockFd_ + 1, &sock_read_sets, NULL, NULL, &tv) > 0;
	}

    std::string TcpStream::GetPeerIP() const
    {
       return address_.GetIp();
    }

    std::string TcpStream::GetPeerIPAndPort() const
    {
        return address_.GetIpAndPort();
    }

  
}
