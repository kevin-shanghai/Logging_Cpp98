#ifndef SOCKET_API_WRAPPER_H__
#define SOCKET_API_WRAPPER_H__
#include <stdint.h>
#include "Common/Logger/Log/include/net/SoketDefine.h"
#include "Common/Logger/Log/include/base/Noncopyable.h"


namespace Net
{
	class SocketsApi : private Noncopyable
	{
	public:
		static socket_t CreateNonBlockingSocket();

		static socket_t Connect(socket_t sockfd, const sockaddr_in_t& addr);

		static void Bind(socket_t sockfd, const sockaddr_in_t& addr);

		static void Listen(socket_t sockfd);

		static socket_t  Accept(socket_t sockfd, struct sockaddr_in* addr);

		static size_t Receive(socket_t sockfd, void *buf, size_t count);

		//ssize_t readv(socket_t sockfd, const struct iovec *iov, int iovcnt);

		static size_t Send(socket_t sockfd, const void *buf, uint32_t count);

		static void Close(socket_t sockfd);

		static void ShutdownWrite(socket_t sockfd);

		static void ToIpPort(char* buf, size_t size, const sockaddr_in& addr);

		static void ToIp(char* buf, size_t size, const struct sockaddr_in& addr);

		static void FromIpPort(const char* ip, uint16_t port, sockaddr_in_t* addr);

		static int GetSocketError(socket_t sockfd);

		static sockaddr_in_t GetLocalAddr(socket_t sockfd);

		static sockaddr_in_t GetPeerAddr(socket_t sockfd);

		static bool IsSelfConnect(socket_t sockfd);
	};
}




#endif