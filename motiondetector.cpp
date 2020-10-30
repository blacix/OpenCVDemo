#include "motiondetector.h"

#include <opencv2/tracking.hpp>


MotionDetector::MotionDetector()
{

}

void MotionDetector::process(cv::Mat &image)
{
    detectMotion(image);
}

void MotionDetector::enable(bool enabled)
{
    if(enabled)
        mFirstFrameGray = cv::Mat();

    ImageProcessor::enable(enabled);
}

// https://gist.github.com/ImAnllama/c38cdf5944a6823d548a4e88271a5790
void MotionDetector::detectMotion(cv::Mat& frame)
{
    if(!isEnabled())
        return;

    cv::Mat frameDelta, thresh;
    std::vector<std::vector<cv::Point> > cnts;

    //convert to grayscale and set the first frame
    if(mFirstFrameGray.empty())
    {
        cv::cvtColor(frame, mFirstFrameGray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(mFirstFrameGray, mFirstFrameGray, cv::Size(21, 21), 0);
        return;
    }

    //convert to grayscale
    cv::cvtColor(frame, mActualFrameGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(mActualFrameGray, mActualFrameGray, cv::Size(21, 21), 0);

    //compute difference between first frame and current frame
    cv::absdiff(mFirstFrameGray, mActualFrameGray, frameDelta);
    cv::threshold(frameDelta, thresh, 25, 255, cv::THRESH_BINARY);

    cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1,-1), 2);
    cv::findContours(thresh, cnts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1);


    for(size_t i = 0; i < cnts.size(); i++)
    {
        if(cv::contourArea(cnts[i]) < frame.size().area() / 500)  // cv::contourArea(cnts[i]) <  5000
        {
            continue;
        }

        cv::Scalar color = cv::Scalar( 0, 255, 255);
        std::vector<std::vector<cv::Point> > contours_poly( cnts.size() );
        cv::approxPolyDP( cv::Mat(cnts[i]), contours_poly[i], 3, true );
        cv::Rect boundRect = boundingRect( cv::Mat(contours_poly[i]) );
        cv::rectangle( frame, boundRect.tl(),boundRect.br(), color, 2, cv::LINE_8, 0 );
//        cv::putText(frame, "Motion Detected", cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,255),2);
    }
}

//void ImageProcessing::detectMotion()
//{
//    if(!mMotionDetection)
//        return;

//    if (!mPrevImage.empty())
//    {
//        if(imageChanged(mPrevImage, mCurrentImage))
//        {
//            std::cout << "image changed" << std::endl;
//        }
//    }
//}

bool MotionDetector::imageChanged(const cv::Mat& image1, const cv::Mat& image2)
{
    int h_bins = 50, s_bins = 60;
    int histSize[] = { h_bins, s_bins };
    int channels[] = { 0, 1 };
    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    const float* ranges[] = { h_ranges, s_ranges };

    cv::Mat hist1, hist2;
    cv::Mat hvsImage1, hvsImage2;
    // Convert to HSV format
    cv::cvtColor(image1, hvsImage1, cv::COLOR_BGR2HSV);
    cv::cvtColor(image2, hvsImage2, cv::COLOR_BGR2HSV);

    cv::calcHist(&hvsImage1, 1, channels, cv::Mat(), hist1, 2, histSize, ranges, true, false);
    cv::normalize(hist1, hist1, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    cv::calcHist(&hvsImage2, 1, channels, cv::Mat(), hist2, 2, histSize, ranges, true, false);
    cv::normalize(hist2, hist2, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    double histCompareResult = compareHist(hist1, hist2, cv::HISTCMP_CHISQR);
    //std::cout << " hist compare: " << histCompareResult << std::endl;
    if (histCompareResult > 3.5f) {
        std::cout << "image changed" << std::endl;
        return true;
    }

    return false;
}
