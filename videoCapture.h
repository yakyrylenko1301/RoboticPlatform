#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

class videoCapture
{
private:
    cv::VideoCapture cature;
    cv::Mat image;
public:
    videoCapture() = default;
    videoCapture(const videoCapture&) = delete;
    videoCapture& operator=(const videoCapture&) = delete;
    videoCapture(int index, cv::VideoCaptureAPIs apiPreference, int width, int height);
    int getFrameSize(void);
    unsigned char* getPtrData(void);
    cv::Mat getFrame(void);
    bool isOpen(void);
    bool isEmpty(void);
};

#endif // VIDEO_CAPTURE_H
