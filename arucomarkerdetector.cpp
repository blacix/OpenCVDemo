#include "arucomarkerdetector.h"
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/aruco/dictionary.hpp>

static cv::Ptr<cv::aruco::Dictionary> MARKER_DICTIONARY = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);

ArucoMarkerDetector::ArucoMarkerDetector():
    mMarkerDislayEnabled(true)
{
    mMarkerDetectorParams = cv::aruco::DetectorParameters::create();
    mMarkerDetectorParams->cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX;
    mMarkerDetectorParams->minDistanceToBorder = 2;
//    mMarkerDetectorParams->detectInvertedMarker = true;

    mCallback = [&](std::vector<int> ids)->void {};
}

void ArucoMarkerDetector::process(cv::Mat &image)
{
    detectMarkers(image);
}

void ArucoMarkerDetector::detectMarkers(cv::Mat& image)
{
    if(!isEnabled())
        return;

    mMarkerIds.clear();
    mMarkerCorners.clear();
    mRejectedMarkerCandidates.clear();

    cv::aruco::detectMarkers(image, MARKER_DICTIONARY, mMarkerCorners, mMarkerIds, mMarkerDetectorParams, mRejectedMarkerCandidates);
    if(mMarkerDislayEnabled)
    {
        cv::aruco::drawDetectedMarkers(image, mMarkerCorners, mMarkerIds);
    }

    std::stringstream ss;
    ss << "marker ids: " << mMarkerIds.size();
    ss << " - candidates: " << mRejectedMarkerCandidates.size() << std::endl;

    // std::cout << ss.str();
    mCallback(mMarkerIds);
}
