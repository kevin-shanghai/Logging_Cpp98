#ifndef SOCKET_H__
#define SOCKET_H__
#include "Common/Logger/Log/include/base/Noncopyable.h"
#include "Common/Logger/Log/include/net/SocketApiWrapper.h"
#include "Common/Logger/Log/include/net/InternetAddress.h"

namespace Net
{
	class Socket : private Noncopyable
	{
	public:
		explicit Socket(socket_t sockfd);

		void BindAddress(const InternetAddress& localAddress);
		void Listen();
		socket_t Accept(InternetAddress& peerAddress);


	private:
		const socket_t socketfd_;
	};
}





#endif


