#include "stereoCam.h"

stereoCam::stereoCam(struct stereoCamData stereoCamLeft, struct stereoCamData stereoCamRight)
{
    captureLeft = new videoCapture(stereoCamLeft.index, stereoCamLeft.apiPreference,
        stereoCamLeft.frameWidth, stereoCamLeft.frameHeight);
    captureRight = new videoCapture(stereoCamRight.index, stereoCamRight.apiPreference,
        stereoCamRight.frameWidth, stereoCamRight.frameHeight);
    isOpen = captureLeft->isOpen();
    isOpen = isOpen && captureRight->isOpen();    
}

stereoCam::~stereoCam(void)
{
    delete this->captureLeft;
    delete this->captureRight; 
}

bool stereoCam::isOpened(void)
{
    return this->isOpen;
}

bool stereoCam::isEmpty(void)
{
    return this->captureLeft->isEmpty() || this->captureRight->isEmpty();
}

cv::Mat stereoCam::getLeftFrame(void)
{
    cvtColor(this->frameLeft, this->frameLeft, cv::COLOR_BGR2RGB);
    cv::rotate(this->frameLeft, this->frameLeft,cv::ROTATE_180);
    return this->frameLeft;
}

cv::Mat stereoCam::getRightFrame(void)
{
    cvtColor(this->frameRight, this->frameRight, cv::COLOR_BGR2RGB);
    cv::rotate(this->frameRight, this->frameRight,cv::ROTATE_180);
    return this->frameRight;
}

bool stereoCam::saveStereoFrame(void)
{
    this->frameLeft = this->captureLeft->getFrame();
    this->frameRight = this->captureRight->getFrame();
    bool isEmpty = this->frameLeft.empty();
    isEmpty = isEmpty || this->frameRight.empty();
    return isEmpty ? false : true; 
}

int stereoCam::getLeftSize(void)
{
    return this->frameLeft.total() * this->frameLeft.elemSize();
}

int stereoCam::getRightSize(void)
{
    return this->frameRight.total() * this->frameRight.elemSize();
}

