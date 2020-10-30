#ifndef MOBILENETSSD_H
#define MOBILENETSSD_H
#include <opencv2/core/core.hpp>

class MobilenetSSD
{
public:
    MobilenetSSD();

    void doIt(const cv::Mat& image);
};

#endif // MOBILENETSSD_H
