#include "arucomarkerdetector.h"
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/aruco/dictionary.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

static cv::Ptr<cv::aruco::Dictionary> MARKER_DICTIONARY = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);

ArucoMarkerDetector::ArucoMarkerDetector():
    mMarkerDislayEnabled(true)
{
    MARKER_DICTIONARY = cv::aruco::generateCustomDictionary(50, 3);

    mMarkerDetectorParams = cv::aruco::DetectorParameters::create();
    mMarkerDetectorParams->cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX;
    mMarkerDetectorParams->minDistanceToBorder = 2;
//    mMarkerDetectorParams->detectInvertedMarker = true;

    mCallback = [&](std::vector<int> ids)->void {};

//    generateMarkerImages();
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

void ArucoMarkerDetector::generateMarkerImages() const
{
    cv::Mat img;
    for(int i = 0; i < 50; ++i)
    {
        std::string fileName("aruco_3x3_50_");
        cv::aruco::drawMarker(MARKER_DICTIONARY, i, 200, img);
        if(i < 10)
            fileName.append("0");
        fileName.append(std::to_string(i)).append(".jpg");

        cv::imwrite(fileName.c_str(), img);
    }
}
