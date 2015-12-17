#include "Common/Logger/Log/include/net/Socket.h"
#include <assert.h>

namespace Net
{

	Socket::Socket(socket_t sockfd) : socketfd_(sockfd)
	{

	}

	void Socket::BindAddress(const InternetAddress& localAddress)
	{
		SocketsApi::Bind(socketfd_, localAddress.GetInetAddress());
	}

	void Socket::Listen()
	{
		SocketsApi::Listen(socketfd_);
	}

	socket_t Socket::Accept(InternetAddress& peerAddress)
	{
	    sockaddr_in_t addr;
		memset(&addr, 0, sizeof addr);
		socket_t connfd = SocketsApi::Accept(socketfd_, &addr);
		assert(connfd > 0);
		peerAddress.SetSockAddrInet(addr);
		return connfd;
	}
}