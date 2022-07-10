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
#include "tcp_ip_server.h"
#include <bcm2835.h>
#include "motor.h"
#include "speed_sensor.h"
#include "electricDrive.h"

#include <chrono>
#include<signal.h>
#include<unistd.h>
#include <ctime>

#include "fdrm_stbc_agm01.h"
#include <wiringPiI2C.h>

using namespace cv;
using namespace std;

ElectricDrive* pEd = nullptr;
tcp_ip_server* p_server = nullptr;


void setEd(ElectricDrive* thisEd);

ElectricDrive* getEd(void);

void setServer(tcp_ip_server* thisServer);

tcp_ip_server* getServer(void);

/*
Run the utility:
v4l2-ctl --list-devices
*/
void start_information(void)
{
    std::cout << "Start TCP/IP server" << std::endl;
    cout << "Built with OpenCV " << CV_VERSION << endl;
}

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

namespace cmdToEdCtrl
{
   const int moveForward    = 34;
   const int moveBack       = 35;
   const int moveLeft       = 36;
   const int moveRight      = 37;
   const int moveStop       = 38;
};

void setEd(ElectricDrive* thisEd)
{
    pEd = thisEd;
}

ElectricDrive* getEd(void)
{
    return pEd;
}

void setServer(tcp_ip_server* thisServer)
{
    p_server = thisServer;
}

tcp_ip_server* getServer(void)
{
    return p_server;;    
}


void* rcvData(void* fd)
{
    int byte, nType, nLen;
    unsigned char char_recv[100] = {0};
    int sock = *((int*)fd);
    ElectricDrive* ed = getEd();
    while(1)
    {
        recv(sock, char_recv, sizeof(int),0);
        nType = (int)char_recv[0];
        printf("Recv data 1 : %d \n",nType);

        // recv(sock, char_recv, sizeof(int), 0);
        // printf("Recv data 2 : %d \n",char_recv[0]);
        // nLen = (int)char_recv[0];

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
        else if (nType == cmdToEdCtrl::moveForward)
        {
            printf("move forward\n");
            ed->moveForward();
        }
        else if (nType == cmdToEdCtrl::moveBack)
        {
            printf("move back\n");
            ed->moveBack();
        }
        else if (nType == cmdToEdCtrl::moveLeft)
        {
            printf("move left\n");
            ed->moveLeft();
        }
        else if(nType == cmdToEdCtrl::moveRight)
        {
            printf("move right\n");
            ed->moveRight();
        }
        else if(nType == cmdToEdCtrl::moveStop)
        {
            printf("move right\n");
            ed->moveStop();
        }
    }
}
// clock_t prev_time = 0;;
// void sig_handler(int signum){
//     tcp_ip_server* p_server = getServer();
//     if (p_server != nullptr)
//     {
//         if(p_server->isReadyToCommunication())
//         {
//             ElectricDrive* ed = getEd();
//             //int rpmFL, int rpmFR, int rpmBL, int rpmBR
//             p_server->sendRPM(ed->getFrontLeftRPM(), ed->getFrontRightRPM(),
//                 ed->getBackLeftRPM(), ed->getBackRightRPM());
//             // std::cout << "FRONT: "<< ed->getFrontLeftRPM() << "," << ed->getFrontRightRPM() << std::endl;
//             // std::cout << "BACK:" << ed->getBackLeftRPM()  << "," <<  ed->getBackRightRPM()<< std::endl;

//         }
//     }
//     alarm(1);
// }


pthread_t thread_X;

void* sendRPMToServer(void* arg)
{
    std::cout << "start thread sendRPMToServer" << std::endl;
    for(;;)
    {
        tcp_ip_server* p_server = getServer();
        if (p_server != nullptr)
        {
            if(p_server->isReadyToCommunication())
            {
                ElectricDrive* ed = getEd();
                //int rpmFL, int rpmFR, int rpmBL, int rpmBR
                p_server->sendRPM(ed->getFrontLeftRPM(), ed->getFrontRightRPM(),
                    21, ed->getBackRightRPM());
            }
        }
        usleep(10000);       
    }
}

int main()
{

    // int fd = wiringPiI2CSetup(FXOS8700CQ_SLAVE_ADDR);

    // close(fd);


//     //start_information();
// /*
//     // tcp_ip_server server(5002);

//     // ElectricDrive ed(5);

//     // setEd(&ed);
//     // setServer(&server);

//     // if (server.isOpened())
//     // {
//     //     cout << "INFO:Sever created" << endl;
//     // }
//     // else
//     // {
//     //     cout << "ERROR:Sever did not creat" << endl;
//     //     return EXIT_FAILURE;
//     // }

//     // if (server.waitConnectionClient())
//     // {
//     //     cout << "INFO:Client connected to server" << endl;    
//     // }
//     // else
//     // {
//     //     cout << "ERROT:Client did not connect to server" << endl;
//     //     return EXIT_FAILURE; 
//     // }

//     // // stereoCam stereoCamera(camLeft, camRight);
//     // // if (!stereoCamera.isOpened())
//     // // {
//     // //     server.close_server();
//     // //     return EXIT_FAILURE;
//     // // }
//     // // else
//     // // {
//     // //     cout << "Stereo camera opened!!" << endl;
//     // // }

//     // if (!server.start_rcv_data(rcvData))
//     // {
//     //     cout << "ERROT:Can not start rcv data" << endl;
//     //     return EXIT_FAILURE; 
//     // }

//     // if(pthread_create(&thread_X, NULL, sendRPMToServer, NULL) != 0)
//     // {
//     //     cout << "ERROT:Can not start rcv data" << endl;
//     //     return EXIT_FAILURE;    
//     // } 
// */  
    FXOS8700CQ sensorAccelMag(FXOS8700CQ_SLAVE_ADDR);
    if (sensorAccelMag.isOpen() == false)
    {
        std::cout << "ERROR: CAN NOT OPEN SENSOR" << std::endl;
        return -1;
    }
    
    sensorAccelMag.init();

    while (1)
    {
        sensorAccelMag.readAccelData();
        sensorAccelMag.readMagData();
        /* code */
        // Print out the data
        // Accelerometer
        cout << "Accel ";
        cout << "X: ";
        cout << (int)sensorAccelMag.accelData.x;
        cout << " Y: ";
        cout << (int)sensorAccelMag.accelData.y;
        cout << " Z: ";
        cout << (int)sensorAccelMag.accelData.z << endl;

        // Magnometer
        cout << "Mag ";
        cout << "X: ";
        cout << (int)sensorAccelMag.magData.x;
        cout << " Y: ";
        cout << (int)sensorAccelMag.magData.y;
        cout << " Z: ";
        cout << (int)sensorAccelMag.magData.z << endl;

        sleep(1);
    }
    

	return 0;
}


