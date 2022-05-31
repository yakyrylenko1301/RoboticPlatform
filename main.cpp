#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
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


#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

#include "stereoCam.h"

using namespace cv;
using namespace std;
/*
Run the utility:
v4l2-ctl --list-devices
*/
void start_information(void)
{
    std::cout << "Start TCP/IP server" << std::endl;
    cout << "Built with OpenCV " << CV_VERSION << endl;
}

typedef struct TAG_MSG_PACK
{
	int nImageType;
	int nLen;
	char data[];
}MSG_PACK;

const stereoCamData camLeft =
{
    .index = 0,
    .apiPreference = CAP_V4L2,
    .frameWidth = 320,
    .frameHeight = 240,
};

const stereoCamData camRight =
{
    .index = 2,
    .apiPreference = CAP_V4L2,
    .frameWidth = 320,
    .frameHeight = 240,
};


void *RcvData(void *fd);

int  SendImage(int sock, unsigned char *buf, int nLen, int imgeType)
{
	int nRes = 0;
	MSG_PACK *msgPack;
	char *pSZMsgPack = (char*)malloc(sizeof(MSG_PACK)+nLen);
	msgPack = (MSG_PACK *)pSZMsgPack;
	msgPack->nImageType = htonl(imgeType);
	msgPack->nLen = htonl(nLen);
	memcpy(pSZMsgPack+sizeof(MSG_PACK), buf, nLen);
	nRes = write(sock, pSZMsgPack, sizeof(MSG_PACK)+nLen);
	printf("Write imageBuf: %d\n",nRes);
	free(pSZMsgPack);
	if(nRes == nLen+sizeof(MSG_PACK))
	{
        printf("Ok -send\n");
		return nLen+sizeof(MSG_PACK);
	}
	return 0;
}

int main()
{
    start_information();
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct ifreq ifr;
    struct hostent *host_entry;
    char sZhostName[255];
    gethostname(sZhostName,255);
    host_entry = gethostbyname(sZhostName);

    char sendBuff[1025];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    if (listenfd != -1)
    {
        printf("1.SUCCESS: Create a new socket, id =  %d\r\n", listenfd);
    }
    else
    {
        printf("1.ERROE: Create a new socket, id =  %d\r\n", listenfd);
        exit(1);
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5029);

    int ret_bind = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret_bind == 0)
    {
        printf("2.SUCCESS: Bind a name to a socket, ret val =  %d\r\n", ret_bind);
    }
    else
    {
        printf("2.ERROR: Bind a name to a socket, ret val =  %d\r\n", ret_bind);   
        exit(1); 
    }

    int ret_listen = listen(listenfd, 10);
    if(ret_listen == 0)
    {
        printf("3.SUCCESS: listen for connections on a socket, ret val =  %d\r\n", ret_listen);    
    }
    else
    {
        printf("3.ERROR: listen for connections on a socket, ret val =  %d\r\n", ret_listen);
        exit(1);
    }
    char array[] = "eth0";
    strncpy(ifr.ifr_name , array , IFNAMSIZ - 1);
    ioctl(listenfd, SIOCGIFADDR, &ifr);
    printf("IP Address is %s - %s\n", array , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );
    printf("Listening on port %d\n",  ntohs(serv_addr.sin_port));

    struct sockaddr_in client;
    int clientLen = 0;
    clientLen = sizeof(struct sockaddr_in);

    printf("Waiting for incoming connections...\r\n");
    connfd = accept(listenfd, (struct sockaddr*)&client, (socklen_t*)&clientLen);
    if(connfd != -1)
    {
        printf("4.SUCCESS: connection arrived, ret val =  %d\r\n", connfd);  
    }
    else
    {
        printf("4.ERROR: connection arrived, ret val =  %d\r\n", connfd); 
        exit(1);  
    }

    stereoCam stereoCamera(camLeft, camRight);
    if (!stereoCamera.isOpened())
    {
        close(connfd);    
    }
    else
    {
        cout << "Stereo camera opened!!" << endl;
    }

    //Create a thread to receive client message
    pthread_t thread;
    if(pthread_create(&thread, NULL, RcvData, &connfd) != 0)
	{	
        printf("5.Error: Can not create the thread\r\n");
        exit(1); 
	}
    else
    {
        printf("5.SUCCESS: Thread Created\r\n");      
    }


    cout << "captureLeft is opened" << endl;
    for(;;)
    {
        bool res = stereoCamera.saveStereoFrame();
        if (res != true)
        {
            cout << "Image is empty" << endl;
            break;    
        }

        unsigned char* dataMatLeft = stereoCamera.getLeftFrame().data;
        unsigned char* dataMatRight = stereoCamera.getRightFrame().data;

        SendImage(connfd, dataMatLeft, stereoCamera.getLeftSize(), 1);//Send original RGB image,type 1
        SendImage(connfd, dataMatRight, stereoCamera.getRightSize(), 2);//Send original RGB image,type 1
    }

    cout << "Close conection with " << endl;
    close(connfd);
    pthread_cancel(thread);



    while(1) 
    {
        sleep(1);
    }
	return 0;
}

void *RcvData(void *fd)
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

