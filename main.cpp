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

int main()
{
    start_information();
/*    tcp_ip_server server(5000);

    if (server.isOpened())
    {
        cout << "INFO:Sever created" << endl;
    }
    else
    {
        cout << "ERROR:Sever did not creat" << endl;
    }

    if (server.waitConnectionClient())
    {
        cout << "INFO:Client connected to server" << endl;    
    }
    else
    {
        cout << "ERROT:Client did not connect to server" << endl;    
    }

    stereoCam stereoCamera(camLeft, camRight);
    if (!stereoCamera.isOpened())
    {
        server.close_server(); 
    }
    else
    {
        cout << "Stereo camera opened!!" << endl;
    }

    if (!server.start_rcv_data())
    {
        cout << "ERROT:Can not start rcv data" << endl;         
    }

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

        server.sendImage(dataMatLeft, stereoCamera.getLeftSize(), 1);
        server.sendImage(dataMatRight, stereoCamera.getRightSize(), 2);
    }*/

    //front motor right En1 = Gpio 22, In1 =  Gpio 27, In2 = Gpio 17

    MotorCtrl motor1(2,0,3);
    motor1.forward();
    motor1.speedUp(10);
    sleep(5);
    motor1.stop();
    while(1)
    {

    }
	return 0;
}


