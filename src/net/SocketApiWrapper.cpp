#include "Common/Logger/Log/include/net/SocketApiWrapper.h"
#include "Common/Logger/Log/include/Log.h"
#include <assert.h>
#include <iostream>


namespace Net
{

	socket_t SocketsApi::CreateNonBlockingSocket()
	{
#ifdef ON_WINDOWS
		WORD wsaword;
		WSADATA wsadata;
		wsaword = MAKEWORD(2,2);
		uint64_t iError = ::WSAStartup(wsaword, &wsadata);
		if (iError != NOERROR)
		{
			return 0;
		}
		if ((2 != LOBYTE(wsadata.wVersion)) || (2 != LOBYTE(wsadata.wHighVersion)))
		{
			::WSACleanup();
			return 0;
		}
		socket_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == INVALID_SOCKET)
		{
			::WSACleanup();
			return 0;
		}

#elif defined ON_VXWORKS
		socket_t sockfd = ::socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);
		
#else
		socket_t sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP )
#endif
		if (sockfd < 0)
		{
			std::cout << "sockets::createNonblockingOrDie" << std::endl;;
		}
        return sockfd;
	}

	socket_t SocketsApi::Connect(socket_t sockfd, const sockaddr_in_t& addr)
	{
		return ::connect(sockfd, (sockaddr_t*)&addr, static_cast<socklen_t>(sizeof addr));
	}

	void SocketsApi::Bind(socket_t sockfd, const sockaddr_in_t& addr)
	{
		int ret = ::bind(sockfd, (sockaddr_t*)(&addr), static_cast<socklen_t>(sizeof addr));
		if (ret < 0)
		{
			std::cout << "Error, SocketsApi::bind error." << std::endl;
            exit(-1);
		}
	}


	void SocketsApi::Listen(socket_t sockfd)
	{
		int listenRet = ::listen(sockfd, SOMAXCONN);
		if (listenRet < 0)
		{
			std::cout  << "Error, sockets::listenOrDie" << std::endl;
            exit(-1);
		}
	}

	socket_t SocketsApi::Accept(socket_t listenSockfd, sockaddr_in_t* addr)
	{
		socklen_t addrLen = static_cast<socklen_t>(sizeof *addr);
#ifdef ON_WINDOWS
		socket_t connSockFd = ::accept(listenSockfd, (sockaddr_t*)addr, &addrLen);
		if (connSockFd >= 0)
		{
			return connSockFd;
		}
		::closesocket(listenSockfd);
		::WSACleanup();

#else
		socket_t connSockFd = ::accept(listenSockfd, (sockaddr_t*)(addr), &addrLen);
#endif
        return connSockFd;
	}

	size_t SocketsApi::Receive(socket_t sockfd, void *buf, size_t count)
	{
		return ::recv(sockfd, static_cast<char*>(buf), count, 0);
	}

	////ssize_t readv(socket_t sockfd, const struct iovec *iov, int iovcnt);

	size_t SocketsApi::Send(socket_t sockfd, const void *buf, uint32_t count)
	{
		return ::send(sockfd, static_cast<const char*>(buf), count, 0) ;
	}

	void SocketsApi::Close(socket_t sockfd)
	{
#ifdef ON_WINDOWS
		if (::closesocket(sockfd) < 0)
		{
			
            std::cout  << "Error, sockets::close" << std::endl;
            exit(-1);
		}
#else
		if (::close(sockfd) < 0)
		{
            std::cout  << "Error, sockets::close" << std::endl;
            exit(-1);
		}
#endif
	}

	void SocketsApi::ShutdownWrite(socket_t sockfd)
	{
#ifdef ON_WINDOWS
		if (::shutdown(sockfd, SD_SEND) < 0)
		{
            std::cout  << "sockets::shutdownWrite" << std::endl;
            exit(-1);
		}
#else
		if (::shutdown(sockfd, SHUT_WR) < 0)
		{
            std::cout  << "sockets::shutdownWrite" << std::endl;
            exit(-1);
		}
#endif	
	}

	void SocketsApi::ToIpPort(char* buf, size_t size, const sockaddr_in_t& addr)
	{
#ifdef ON_WINDOWS
#define snprintf sprintf_s
#endif
		assert(size >= INET_ADDRSTRLEN);
		::inet_ntop(AF_INET, (void*)&addr.sin_addr, buf, static_cast<socklen_t>(size));   
		size_t end = ::strlen(buf);

		//uint16_t port = sockets::networkToHost16(addr.sin_port);
		uint16_t port = ntohs(addr.sin_port);
		//uint16_t port = ::be16toh(addr.sin_port);
		assert(size > end);
		snprintf(buf + end, size - end, ":%u", port);
	}

	void SocketsApi::ToIp(char* buf, size_t size, const struct sockaddr_in& addr)
	{
		assert(size >= INET_ADDRSTRLEN);
		::inet_ntop(AF_INET, (void*)&addr.sin_addr, buf, static_cast<socklen_t>(size));
	}

	void SocketsApi::FromIpPort(const char* ip, uint16_t port, sockaddr_in_t* addr)
	{
		addr->sin_family = AF_INET;
		//addr->sin_port = hostToNetwork16(port);
		addr->sin_port = htons(port);
		if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
		{
            std::cout  << " Error, sockets::fromIpPort" << std::endl;
            exit(-1);
		}
	}

	//int GetSocketError(socket_t sockfd);

	sockaddr_in_t SocketsApi::GetLocalAddr(socket_t sockfd)
	{
		sockaddr_in_t localaddr;
		memset(&localaddr, 0, sizeof(localaddr));
		socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
		if (::getsockname(sockfd, (sockaddr_t*)(&localaddr), &addrlen) < 0)
		{
            std::cout  << " Error, sockets::getLocalAddr" << std::endl;
            exit(-1);
		}
		return localaddr;
	}

	sockaddr_in_t SocketsApi::GetPeerAddr(socket_t sockfd)
	{
		sockaddr_in_t peeraddr;
		memset(&peeraddr, 0, sizeof(peeraddr));
		socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
		if (::getpeername(sockfd, (sockaddr_t*)(&peeraddr), &addrlen) < 0)
		{
            std::cout  << " Error, sockets::GetPeerAddr" << std::endl;
            exit(-1);
		}
		return peeraddr;
	}

	bool SocketsApi::IsSelfConnect(socket_t sockfd)
	{
		struct sockaddr_in localaddr = GetLocalAddr(sockfd);
		struct sockaddr_in peeraddr = GetPeerAddr(sockfd);
		return localaddr.sin_port == peeraddr.sin_port
			&& localaddr.sin_addr.s_addr == peeraddr.sin_addr.s_addr;
	}

}
