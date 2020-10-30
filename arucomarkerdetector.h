#ifndef ARUCOMARKERDETECTOR_H
#define ARUCOMARKERDETECTOR_H

#include "ImageProcessor.h"
#include <opencv2/aruco.hpp>

typedef std::function<void(std::vector<int>)> ArucoMarkerListCallback;

class ArucoMarkerDetector : public ImageProcessor
{
public:
    ArucoMarkerDetector();
    void process(cv::Mat& image);
    inline bool isMarkerDisplayEnabled() const {return mMarkerDislayEnabled;}
    inline void enableMarkerDisplay(bool enabled) {mMarkerDislayEnabled = enabled;}
    inline void setCallback(ArucoMarkerListCallback callback) {mCallback = callback;}

private:
    void detectMarkers(cv::Mat& image);

    cv::Ptr<cv::aruco::DetectorParameters> mMarkerDetectorParams;
    std::vector<int> mMarkerIds;

    std::vector<std::vector<cv::Point2f>> mMarkerCorners;
    std::vector<std::vector<cv::Point2f>> mRejectedMarkerCandidates;
    ArucoMarkerListCallback mCallback;

    bool mMarkerDislayEnabled;

};

#endif // ARUCOMARKERDETECTOR_H
