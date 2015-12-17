#ifndef TCPSERVER_H__
#define TCPSERVER_H__


namespace Net
{
    class TcpAcceptor;
    class TcpServer
    {
    public:
        TcpServer();

    private:
        TcpAcceptor* acceptor_;
    };
}

#endif