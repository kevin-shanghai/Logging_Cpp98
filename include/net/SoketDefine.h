#ifndef SOCKET_DEFINE_H__
#define SOCKET_DEFINE_H__
#include "Common/Logger/Log/include/PlatformDefine.h"

#ifdef ON_WINDOWS
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#endif

#ifdef ON_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#endif

#ifdef ON_VXWORKS
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <types/vxTypesOld.h>
#endif



#ifdef ON_WINDOWS
typedef SOCKET socket_t;
typedef SOCKADDR_IN sockaddr_in_t;
typedef SOCKADDR sockaddr_t;
typedef IN_ADDR in_addr_tt;
typedef unsigned short port_t;
typedef fd_set fd_set_t;
#endif

#ifdef ON_LINUX
typedef int socket_t;
typedef sockaddr_in sockaddr_in_t;
typedef sockaddr sockaddr_t;
typedef in_addr in_addr_tt;
typedef unsigned short port_t;
typedef fd_set fd_set_t;
#endif

#ifdef ON_VXWORKS
typedef int socket_t;
typedef sockaddr_in sockaddr_in_t;
typedef sockaddr sockaddr_t;
typedef in_addr in_addr_tt;
typedef unsigned short port_t;
typedef fd_set fd_set_t;
#endif

#endif
