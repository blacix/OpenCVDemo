#include "ImageProcessing.h"

#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui/highgui_c.h>

#include <iostream>
#include <ctype.h>



static const char* WINDOW_MAIN = "main";
static const char* WINDOW_RAW = "raw";
static const char* WINDOW_CALIBRATION = "calibration";

static const char* CALIBRATION_CONF_PATH = "config_camera_calib.xml";
const char* ImageProcessing::APP_CONF_PATH = "config_app.xml";


ImageProcessing::ImageProcessing():
    mDeviceID(0), mRawDisplay(false),mRunning(false)
{
    loadConfig(APP_CONF_PATH);
}

void ImageProcessing::loadConfig(const std::string& appConfigPath)
{
    cv::FileStorage fs( appConfigPath.c_str(), cv::FileStorage::READ );
    if(!fs.isOpened())
    {
        std::cout << "cannot load config: " << appConfigPath << std::endl;
        return;
    }

    bool markerDetection, motionDetection;
    fs["camera_id"] >> mDeviceID;
    fs["marker_detection"] >> markerDetection;
    fs["motion_detection"] >> motionDetection;

    mMarkerDetector.enable(markerDetection);
    mMotionDetector.enable(motionDetection);

    fs.release();
}

void ImageProcessing::loadCameraParams(const std::string& path)
{
    loadCameraParams(path, mCameraMatrix, mDistCoeffs);
}

void ImageProcessing::loadCameraParams(const std::string& fileName, cv::Mat& cameraMatrixOut, cv::Mat& distCoeffsOut)
{
    cv::FileStorage fs( fileName.c_str(), cv::FileStorage::READ );
    fs["camera_matrix"] >> cameraMatrixOut;
    fs["distortion_coefficients"] >> distCoeffsOut;
    fs.release();
}

void ImageProcessing::calibrateCamera(CalibrationResultCallback callback)
{
    stop();
    auto trheadFunc = [=]()->void
    {
        int result = cameraCalibrationMain(CALIBRATION_CONF_PATH, WINDOW_CALIBRATION, mCameraMatrix, mDistCoeffs);
        callback(result == CALIBRATED);
        destroyWindow(WINDOW_CALIBRATION);
    };

    mThread = std::thread(trheadFunc);
}

void ImageProcessing::enableMotionDetection(bool enabled)
{
    mMotionDetector.enable(enabled);
}

void ImageProcessing::enableMarkerDetection(bool enabled)
{
    mMarkerDetector.enable(enabled);
}

void ImageProcessing::enableRawDisplay(bool enabled)
{
    mRawDisplay = enabled;
    if(!mRawDisplay)
    {
        destroyWindow(WINDOW_RAW);
    }
}

void ImageProcessing::enableMarkerDisplay(bool enabled)
{
    mMarkerDetector.enableMarkerDisplay(enabled);
}

void ImageProcessing::start(ArucoMarkerListCallback callback)
{
    stop();

    mMarkerDetector.setCallback(callback);
    mRunning = true;
    mThread = std::thread(&ImageProcessing::processStream, this);
}

void ImageProcessing::stop()
{
    mRunning = false;
    destroyAllWindow();
    if(mThread.joinable())
    {
        std::cout << "joining thread" << std::endl;
        mThread.join();
    }
    destroyAllWindow();
}

bool ImageProcessing::isCameraCalibrated() const
{
    return !mCameraMatrix.empty() && !mDistCoeffs.empty();
}

void ImageProcessing::processStream()
{
    std::cout << "processStream ..." << std::endl;
    mVideoCapture.open(mDeviceID, cv::CAP_ANY);
    if (!mVideoCapture.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera\n";
        mRunning = false;
        return;
    }

    // start capturing
    while(mRunning)
    {
        mVideoCapture.read(mCurrentImage);
        if (mCurrentImage.empty()) {
            continue;
        }

        if(mRawDisplay)
        {
            showImage(WINDOW_RAW, mCurrentImage);
        }

        processImage(mCurrentImage);

        showImage(WINDOW_MAIN, mCurrentImage);
        if (cv::waitKey(100) >= 0)
            mRunning = false;

    }

    mVideoCapture.release();
    cv::destroyWindow(WINDOW_MAIN);
    cv::destroyWindow(WINDOW_RAW);
    std::cout << "thread exited" << std::endl;
}

void ImageProcessing::processImage(cv::Mat& image)
{
    if(isCameraCalibrated())
    {
        image = undistort(image);
    }
    mFutures.clear();
    mFutures.push_back(std::async(std::launch::async, [&]{ mMarkerDetector.process(image); }));
    mFutures.push_back(std::async(std::launch::async, [&]{ mMotionDetector.process(image); }));
    mFutures.push_back(std::async(std::launch::async, [&]{ mSSD.doIt(image); }));

    std::for_each (mFutures.begin(), mFutures.end(), [](auto& future){ future.wait(); });
}

cv::Mat ImageProcessing::undistort(const cv::Mat& image) const
{
    cv::Mat undistortedIMage;
    if(isCameraCalibrated())
    {
        cv::Mat map1, map2;
        cv::initUndistortRectifyMap(
            mCameraMatrix, mDistCoeffs, cv::Mat(),
            cv::getOptimalNewCameraMatrix(mCameraMatrix, mDistCoeffs, image.size(), 1, image.size(), 0), image.size(),
            CV_16SC2, map1, map2);

        cv::remap(image, undistortedIMage, map1, map2, cv::INTER_LINEAR);
    }
    else
    {
        undistortedIMage = image.clone();
    }

    return undistortedIMage;
}

cv::Mat ImageProcessing::undistort(const cv::Mat&& image) const
{
    return undistort(image);
}

void ImageProcessing::showImage(const std::string& winTitle, const cv::Mat& image) const
{
    if(mRunning)
        cv::imshow(winTitle.c_str(), image);
}

void ImageProcessing::destroyWindow(const std::string& winTitle) const
{
    if(cvGetWindowHandle(winTitle.c_str()))
    {
        cv::destroyWindow(winTitle.c_str());
    }
}

void ImageProcessing::destroyAllWindow() const
{
    destroyWindow(WINDOW_MAIN);
    destroyWindow(WINDOW_RAW);
    destroyWindow(WINDOW_CALIBRATION);
}
