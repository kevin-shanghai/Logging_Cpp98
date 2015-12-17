#ifndef TCP_CONNECTOR_H__
#define TCP_CONNECTOR_H__


#include "Common/Logger/Log/include/base/SmartPointer.h"
#include "Common/Logger/Log/include/net/InternetAddress.h"


namespace Net
{
    class TcpStream;
	class TcpConnector : private Noncopyable
	{
	public:
        TcpConnector(InternetAddress inetAddress);
		TcpStream* Connect();
        TcpStream* GetClientTcpStream() const;
		
    private:
        //only the SyncLoggingRemote can instance this class
        //friend class SyncLoggingRemote;
        

	private:
		InternetAddress inetAddress_;
        TcpStream* clientTcpStream_;
	};
}



#endif
