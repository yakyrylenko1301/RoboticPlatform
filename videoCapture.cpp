#include "videoCapture.h"

videoCapture::videoCapture(int index, cv::VideoCaptureAPIs apiPreference)
{
    this->cature.open(index, apiPreference);    
}

int videoCapture::getFrameSize(void)
{
    return this->image.total() + this->image.elemSize();
}

unsigned char* videoCapture::getPtrData(void)
{
    return this->image.data;
}

cv::Mat videoCapture::getFrame(void)
{   
    this->cature >> this->image;
    return this->image;
}

bool videoCapture::isOpen(void)
{
    return this->cature.isOpened();
}
