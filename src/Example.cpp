#include <iostream>
#include "Common/Logger/Log/include/Log.h"

//This log module can work in four modes
// 1. Sync log local file(local storage)
// 2. Sync log remote network through Tcp/ip (start the log server first)
// 3. Async log local file(front end workers put the log msg in buffer asynchronously,
//  another log thread will write the log msg to the local file)
// 4. Async log remote network (front end workers put the log msg in buffer asynchronously,
//  another log thread will send the log msg to the log server through Tcp/ip)
//


//log client example work with remote network and sync mode
int main()
{
	Logger::Log::LOG_CONFIG(Logger::LOG_CLIENT, Logger::REMOTE_NETWORK, Logger::SYNC, "127.0.0.1", 8888);
	std::cout << "Enter main...." << std::endl;
    while(1)
    {
        LOG_INFO << "log info..." ;
        LOG_WARN << "log warning...";
		LOG_ERROR << "log error....";
		::Sleep(1000);
        //yield the CPU
    }
	system("pause");
	return 0;
} 


//**********log server example work with sync mode ***********//
//#include "Common/Logger/Log/include/Log.h"
//#include "Common/Logger/Log/include/net/TcpAcceptor.h"
//#include "Common/Logger/Log/include/net/TcpStream.h"
//#include "boost/thread.hpp"
//#include "boost/bind.hpp"
//
//
//int main()
//{
//    Logger::Log::LOG_CONFIG(Logger::LOG_SERVER, Logger::LOCAL_FILE, Logger::SYNC, "127.0.0.1", 8888);
//    Net::TcpAcceptor* acceptor = new Net::TcpAcceptor(8888);
//    while(1)
//    {
//        Net::TcpStream* stream = acceptor->Accept();
//        assert (stream);
//        std::cout << "New connection from:" << stream->GetPeerIPAndPort() << std::endl;
//        boost::thread t(boost::bind(&Net::TcpStream::ReceiveMsgAndLogFile, stream));;   
//    }
//    system("Pause");
//}

