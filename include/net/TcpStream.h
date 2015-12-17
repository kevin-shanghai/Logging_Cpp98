#ifndef TCP_STREAM_H__
#define TCP_STREAM_H__
#include "Common/Logger/Log/include/net/InternetAddress.h"
#include "Common/Logger/Log/include/net/ChannelBuffer.h"
#include "Common/Logger/Log/include/base/Noncopyable.h"

#include <string>

namespace Net
{
	class EXPORTLOG TcpStream	: Noncopyable
	{
	public:
		TcpStream(const socket_t& sockFd, const InternetAddress& inetAddress);
		size_t Send(const char* buffer, size_t len);
		size_t ReceiveMsgAndLogFile();
		std::string GetPeerIP() const;
		std::string GetPeerIPAndPort() const;
		

	private:
		TcpStream(const TcpStream& rhs);
		//TcpStream(const TcpStream&& rhs);
		bool WaitForSocketReadEvent(uint32_t iTimeoutSeconds);

	private:
		socket_t sockFd_;
        const InternetAddress& address_;
        ChannelBuffer inputBuffer_;
	};

}



#endif
