#include "Common/Logger/Log/include/net/TcpConnector.h"
#include "Common/Logger/Log/include/net/TcpStream.h"
#include "Common/Logger/Log/include/Log.h"
#include <iostream>




namespace Net
{
	TcpConnector::TcpConnector(InternetAddress inetAddress)
		: inetAddress_(inetAddress)
        , clientTcpStream_(NULL)
	{
        clientTcpStream_ = Connect();
	}

	TcpStream* TcpConnector::Connect()
	{
		socket_t sockfd = SocketsApi::CreateNonBlockingSocket();
		if (SocketsApi::Connect(sockfd, inetAddress_.GetInetAddress()) != 0)
		{
			SocketsApi::Close(sockfd);
			//LOG_SYSFATAL << "Can not connected to the server";
            return NULL;
		}
        std::cout << "connect log server successfully..." << std::endl;;
		clientTcpStream_ = new TcpStream(sockfd, inetAddress_);
        assert (clientTcpStream_);
        return clientTcpStream_;
	}


    TcpStream* TcpConnector::GetClientTcpStream() const
    {
        return clientTcpStream_; 
    }
}
