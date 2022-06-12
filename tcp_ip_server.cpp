#include "tcp_ip_server.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 

typedef struct TAG_MSG_PACK
{
	int nImageType;
	int nLen;
	char data[];
}MSG_PACK;

tcp_ip_server::tcp_ip_server(int port) : port(port)
{
    this->open();
}

void tcp_ip_server::open(void)
{
    err_code = server_cfg::server_err_code::SERVER_OK;
    if ((this->port > 999 && this->port < 9999) == false)
    {
        err_code = server_cfg::server_err_code::SERVER_ERR_PORT;
        return;
    }

    this->listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->listenfd == -1)
    {
        err_code = server_cfg::server_err_code::SERVER_ERR_CREATE_SOCKET;
        return;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(this->port);

    int res_bind = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (res_bind != 0)
    {
        err_code = server_cfg::server_err_code::SERVER_ERR_BIND;
        return;
    }

    int ret_listen = listen(listenfd, server_cfg::nbr_connection);

    if (ret_listen != 0)
    {
        err_code = server_cfg::server_err_code::SERVER_ERR_LISTEN;
        return;
    }

    ifr.ifr_addr.sa_family = AF_INET;
    char array[] = "eth0";
    strncpy(ifr.ifr_name , array , IFNAMSIZ - 1);
    ioctl(listenfd, SIOCGIFADDR, &ifr);
    this->ip_addr = std::string(inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr));
    printf("IP Address is %s - %s\n", array , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );
    printf("Listening on port %d\n",  ntohs(serv_addr.sin_port));
    clientLen = sizeof(struct sockaddr_in);
}

void tcp_ip_server::close_server(void)
{
    close(connfd);

}

bool tcp_ip_server::waitConnectionClient(void)
{
    printf("Waiting for incoming connections...\r\n");
    bool ret_val = true;
    connfd = accept(listenfd, (struct sockaddr*)&client, (socklen_t*)&clientLen);

    if(connfd == -1)
    {
        ret_val = false;
        err_code = server_cfg::server_err_code::SERVER_ERR_CONNECTION_ARRIVED;
    }

    return ret_val;
}

bool tcp_ip_server::isOpened()
{
    return (err_code == server_cfg::server_err_code::SERVER_OK);
}

bool tcp_ip_server::start_rcv_data(void* (rcvDataClbk)(void* fd))
{

    bool res_return = true;

    if(pthread_create(&thread, NULL, rcvDataClbk, &connfd) != 0)
    {
        res_return = false;
    }

    return res_return;
}

void* tcp_ip_server::rcvData(void* fd)
{
    int byte, nType, nLen;
    unsigned char char_recv[100] = {0};
    int sock = *((int*)fd);
    while(1)
    {
        recv(sock, char_recv, sizeof(int),0);
        nType = (int)char_recv[0];
        printf("Recv data 1 : %d \n",nType);

        recv(sock, char_recv, sizeof(int), 0);
        printf("Recv data 2 : %d \n",char_recv[0]);
        nLen = (int)char_recv[0];

        if(nType == 5)
        {
            printf("Client reply : \t");
            for(int j = 0; j < nLen; j++)
            {
                recv(sock, char_recv, sizeof(int), 0);
                printf("Recv data %d : %d \n", j, char_recv[0]);


            }
            printf("\r\n");
            memset(char_recv, 0, 100);
        }
    }
}

bool tcp_ip_server::sendImage(unsigned char *buf, int nLen, int imgeType)
{
    int nRes = 0;
    MSG_PACK *msgPack;
    char *pSZMsgPack = (char*)malloc(sizeof(MSG_PACK) + nLen);
    msgPack = (MSG_PACK *)pSZMsgPack;
    msgPack->nImageType = htonl(imgeType);
    msgPack->nLen = htonl(nLen);
    memcpy(pSZMsgPack + sizeof(MSG_PACK), buf, nLen);
    nRes = write(this->connfd, pSZMsgPack, sizeof(MSG_PACK)+nLen);
    printf("Write imageBuf: %d\n",nRes);
    free(pSZMsgPack);

    if(nRes == (nLen + sizeof(MSG_PACK)))
    {
        printf("Ok -send\n");
        return nLen+sizeof(MSG_PACK);
    }

    return 0;

}

tcp_ip_server::~tcp_ip_server(void)
{
    close(connfd);
    pthread_cancel(this->thread);
}

