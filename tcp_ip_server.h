#ifndef TCP_IP_SERVER_H
#define TCP_IP_SERVER_H
#include <iostream>
#include <string>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include<netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>

namespace server_cfg
{
    enum server_err_code
    {
        SERVER_OK,
        SERVER_GOT_CONNECTION,
        SERVER_ERR_PORT,
        SERVER_ERR_CREATE_SOCKET,
        SERVER_ERR_BIND,
        SERVER_ERR_LISTEN,
        SERVER_ERR_CONNECTION_ARRIVED,
        SERVER_
    };

    const int nbr_connection = 1;
};

class tcp_ip_server
{
private:
    int port;
    std::string ip_addr;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct ifreq ifr;
    struct hostent *host_entry;
    char sZhostName[255];
    char sendBuff[1025];
    server_cfg::server_err_code err_code;
    struct sockaddr_in client;
    int clientLen = 0;
    pthread_t thread;
    static void* rcvData(void* fd);
    void* (rcvDataClbk)(void* fd);
    
public:
    tcp_ip_server() = delete;
    tcp_ip_server(const tcp_ip_server&) = delete;
    tcp_ip_server& operator=(const tcp_ip_server&) = delete;
    tcp_ip_server(int port);
    ~tcp_ip_server(void);
    std::string getIpAddr(void);
    int getPort();
    bool isOpened();
    bool waitConnectionClient(void);
    void open(void);
    void close_server(void);
    bool start_rcv_data(void* (rcvDataClbk)(void* fd));
    bool sendImage(unsigned char *buf, int nLen, int imgeType);
};



#endif