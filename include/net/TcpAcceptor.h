#ifndef TCP_ACCEPTOR_H__
#define TCP_ACCEPTOR_H__
#include "Common/Logger/Log/include/net/InternetAddress.h"
#include <map>


namespace Net
{
    class TcpStream;
	class EXPORTLOG TcpAcceptor
	{
	public:
		explicit TcpAcceptor(InternetAddress& InetAddress);
        explicit TcpAcceptor(port_t port);
		void Listen();
		TcpStream* Accept();

	private:
		const InternetAddress& inetAddr_;
		const socket_t listenSockFd_;
		std::map<socket_t, TcpStream*> serverConnfdTcpStreamMap_;
	};
}


#endif
