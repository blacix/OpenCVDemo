#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H

#include "ImageProcessor.h"

class MotionDetector : public ImageProcessor
{
public:
    MotionDetector();
    void process(cv::Mat& image) override;

    void enable(bool enabled) override;

private:

    void detectMotion(cv::Mat& image);
    bool imageChanged(const cv::Mat& image1, const cv::Mat& image2);

    cv::Mat mFirstFrameGray;
    cv::Mat mActualFrameGray;
};

#endif // MOTIONDETECTOR_H
