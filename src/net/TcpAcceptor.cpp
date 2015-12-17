#include "Common/Logger/Log/include/net/TcpAcceptor.h"
#include "Common/Logger/Log/include/net/TcpStream.h"
#include <assert.h>
#include <iostream>


namespace Net
{
	TcpAcceptor::TcpAcceptor(InternetAddress& inetAddress) 
		: inetAddr_(inetAddress)
		, listenSockFd_(SocketsApi::CreateNonBlockingSocket())
	{
		SocketsApi::Bind(listenSockFd_, inetAddr_.GetInetAddress());
		Listen();
	}

    TcpAcceptor::TcpAcceptor(port_t port) 
        : inetAddr_(InternetAddress(port))
        , listenSockFd_(SocketsApi::CreateNonBlockingSocket())
    {
        SocketsApi::Bind(listenSockFd_, inetAddr_.GetInetAddress());
        Listen();
    }

	void TcpAcceptor::Listen()
	{
		SocketsApi::Listen(listenSockFd_);
	}

	TcpStream* TcpAcceptor::Accept()
	{
		socket_t connFd = SocketsApi::Accept(listenSockFd_, &(inetAddr_.GetInetAddress()));
        TcpStream* newTcpStream = new TcpStream(connFd, inetAddr_);
        assert (newTcpStream);
        serverConnfdTcpStreamMap_[connFd] = newTcpStream;
        return newTcpStream;
	}
}