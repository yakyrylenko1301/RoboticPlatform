#ifndef STEREO_CAM_H
#define STEREO_CAM_H
#include "videoCapture.h"

struct stereoCamData
{
    int index;
    cv::VideoCaptureAPIs apiPreference;
    int frameWidth;
    int frameHeight;
};


class stereoCam
{

private:
    videoCapture* captureLeft;
    videoCapture* captureRight;
    cv::Mat frameLeft;
    cv::Mat frameRight;
    bool isOpen;

public:
    stereoCam() = default;
    stereoCam(const videoCapture&) = delete;
    stereoCam& operator=(const stereoCam&) = delete;
    stereoCam(struct stereoCamData stereoCamLeft, struct stereoCamData stereoCamRight);
    ~stereoCam(void);
    bool saveStereoFrame(void);
    cv::Mat getLeftFrame(void);
    cv::Mat getRightFrame(void);
    bool isOpened(void);
    bool isEmpty(void);
    int getLeftSize(void);
    int getRightSize(void);
};

#endif
