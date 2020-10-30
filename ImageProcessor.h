#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <opencv2/core/core.hpp>

class ImageProcessor
{
public:
    virtual void process(cv::Mat& image) = 0;
    virtual inline void enable(bool enabled) {mEnabled = enabled;}

    inline bool isEnabled() const {return mEnabled; }
private:
    bool mEnabled;

};

#endif // IMAGEPROCESSOR_H
