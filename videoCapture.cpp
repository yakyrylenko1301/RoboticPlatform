#include "videoCapture.h"

videoCapture::videoCapture(int index, cv::VideoCaptureAPIs apiPreference, int width, int height)
{
    this->cature.open(index, apiPreference); 
    this->cature.set(cv::CAP_PROP_FRAME_WIDTH, width);
    this->cature.set(cv::CAP_PROP_FRAME_HEIGHT, height);
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

bool videoCapture::isEmpty(void)
{
    return this->image.empty();
}
